#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::initCurrentFiles( vector< string>& duplicatesFileNames ){

	DIR *dir = opendir(_uploadTargetDirectory.c_str());

	if (dir == NULL)
	{
		Logger::getInstance().log(ERROR, "Failing Openning Directory in ResponseBuilder::initCurrentFiles");
		return;
	}

	struct dirent *listing;

	while ((listing = readdir(dir)) != NULL)
	{
		string curFile = listing->d_name;
		if (!isFileIgnored(curFile))
		{
			// DT_REG == is the extracted data a regular file 
			if (listing->d_type == DT_REG)
			{
				curFile.insert(0, _uploadTargetDirectory);
				duplicatesFileNames.push_back(curFile);
			}
		}
	}
}

void	ResponseBuilder::initBoundaryTokens( void ){

	// Create both tokens
	_tokenDelim = _client->headerRequest.getWebToken();
	_tokenDelim.insert(0, "--");

	_tokenEnd = _tokenDelim + "--";

	// Append traling HTTP_SEPARATOR for easier parsing
	_tokenDelim += "\r\n";
	_tokenEnd += "\r\n";

}

void ResponseBuilder::determineSeparator(std::string &separator, size_t &separatorLength, vector<char>& curLine)
{
	if (_writeReady)
	{
		separator = _tokenEnd;
		separatorLength = _tokenEnd.size();

		if (searchSeparator(curLine, separator, separatorLength) == curLine.end())
		{
			separator = _tokenDelim;
			separatorLength = _tokenDelim.size();
		}
	}
	else
	{
		separator = HTTP_HEADER_SEPARATOR;
		separatorLength = 2;
	}
}

vector<char>::iterator ResponseBuilder::searchSeparator(vector<char>& curLine, string &separator, size_t &separatorLength){

    vector<char>::iterator it = std::search(curLine.begin(), curLine.end(),
		separator.c_str(),	separator.c_str() + separatorLength);

	return it;
}

bool ResponseBuilder::isLineDelim(vector<char>& curLine, vector<char>& nextLine)
{
	#define itVec vector<char>::iterator

	string separator;
	size_t separatorLength;

	determineSeparator(separator, separatorLength, curLine);

	// Look for "\r\n" or _tokens in curLine
	itVec it = searchSeparator(curLine, separator, separatorLength);

    if (it == curLine.end())
        return false;

    // Does the curLine end with a trailing \r\n OR { token + "\r\n" } ONLY
    if (it + static_cast<long>(separatorLength) == curLine.end())
	{
		if (_writeReady)
		{
			nextLine.insert(nextLine.end(), it, curLine.end());
			curLine.erase(it , curLine.end());
		}
        return true;
	}

    // In the opposite case, we need to trim the curLine and append the rest to nextLine
	if (_writeReady)
		separatorLength = 0;
    nextLine.insert(nextLine.end(), it + static_cast<long>(separatorLength), curLine.end());
    curLine.erase(it + static_cast<long>(separatorLength), curLine.end());

    return true;
}


void	ResponseBuilder::extractFileBodyName( vector< char >& curLine, vector< string >&duplicatesFileNames ){

	string temp(curLine.begin(), curLine.end());

	string needle = "filename=\"";

	size_t startPos = temp.find(needle);
	size_t endPos;

	if (startPos != std::string::npos)
	{
		startPos += needle.length();

		endPos = temp.find("\"", startPos);

		_fileStreamName = temp.substr(startPos, endPos - startPos);

		_fileStreamName.insert(0, _uploadTargetDirectory);

		/*
			If the current _fileStreamName is found in the little "database" of duplicatesFileName
			we append_a random 10 length string at the end, then save it
		*/
		if (std::find(duplicatesFileNames.begin(), duplicatesFileNames.end(), _fileStreamName) != duplicatesFileNames.end())
		{
			_fileStreamName.insert(_fileStreamName.find_last_of("."), generateRandomString(10, true));
		}
		
		// Save for later calls
		duplicatesFileNames.push_back(_fileStreamName);
	}
}

ResponseBuilder::e_lineNature ResponseBuilder::processCurrentLine(vector< char >& curLine) {

	// Trimm last two trailing character from the current line only if it's not 
	
	if (_writeReady)
		return BINARY_DATA;
	
	string temp(curLine.begin(), curLine.end());
	
	if (temp == HTTP_HEADER_SEPARATOR)
		return LINE_SEPARATOR;
	else
	{
		curLine.erase(curLine.end() - 2, curLine.end());
		temp.erase(temp.end() - 2, temp.end());
	}

	if (temp == _tokenEnd)
		return TOKEN_END;
	else if (temp == _tokenDelim)
		return TOKEN_DELIM;
	else if (temp.rfind("Content-Disposition: ", 0) == 0) // does the beggining of the line starts with the needle
		return CONTENT_DISPOSITION;
	return OTHER;
}

void	ResponseBuilder::setMultiPartPost( Client & client ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::setMultiPartPost");

	static vector< char > curLine;
	static vector< char > nextLine;
	static vector< string> duplicatesFileNames;
	e_lineNature lineNature;

	// skip useless stack calls for each HTTP package
	if (not _parsedBoundaryToken)
	{
		initBoundaryTokens();
		initCurrentFiles(duplicatesFileNames);
		_parsedBoundaryToken = true;
	}
	
	// Copy the nextLine content within the currentLine
	if (!nextLine.empty())
	{
		curLine.insert(curLine.end(), nextLine.begin(), nextLine.end());
		nextLine.clear();
	}

	// Assign the current response...
	vector< char > recVector2(client.messageRecv);

	// ... and append it to the end of curLine
	curLine.insert(curLine.end(), recVector2.begin(), recVector2.end());

	// Clear the buffer from the client // TODO : See if it's okay to clean with SEB
	client.messageRecv.clear();
	
	// While we didn't process a whole line, we write it within the buffer
	if (not isLineDelim(curLine, nextLine))
		return;
	
	// Main loop for treating one byte at a time or all the body
	do
	{
		lineNature = processCurrentLine(curLine);

		switch (lineNature)
		{
			case TOKEN_DELIM:
				_fileStreamName.clear();
				break;

			case TOKEN_END:
				_fileStreamName.clear();
				duplicatesFileNames.clear(); // reset the value for calling on the next request of the same client
				_parsedBoundaryToken = false; // reset the value for calling on the next request of the same client
				break;

			case CONTENT_DISPOSITION:
				extractFileBodyName(curLine, duplicatesFileNames);
				break;
			
			case OTHER: // No incidence
				break;
			
			case LINE_SEPARATOR: // next processed lines will be the binary data

				if (!this->_ofs.is_open())
				{
					this->_ofs.open(_fileStreamName.c_str(), std::ios::binary);
				}
				_writeReady = true;
				break;

			case BINARY_DATA:
				// Writting actual data
				this->_ofs.write(curLine.data(), static_cast<std::streamsize>(curLine.size()));

				if (this->_ofs.is_open())
					this->_ofs.close();
				
				_writeReady = false;
				_fileStreamName.clear();
				break;

			default:
				break;
		}
		
		// Reset buffers
		curLine.clear();
		curLine = nextLine;
		nextLine.clear();

	} while (isLineDelim(curLine, nextLine));
}

// This is now the regular setPost without Multipart Writting
void	ResponseBuilder::setRegularPost( Client & client ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::setRegularPost");

	if (!this->_ofs.is_open())
	{	
		Logger::getInstance().log(INFO, _realURI.c_str());	
		
		string targetToWrite =	_uploadTargetDirectory
								+ generateFileName()
								+ _setBodyExtension;
		
		this->_ofs.open(targetToWrite.c_str(), std::ios::binary | std::ios::app); // need to oppen it in append mode for seekp 
	}

	// _ofs.clear();//!
    if (_ofs.is_open())
	{
		this->_ofs.seekp(0, std::ios::end);
		// this->_ofs.end();
        _ofs.write(client.messageRecv.data(), static_cast<std::streamsize>(client.messageRecv.size())); 
        _ofs.flush();
		if (!_ofs)
		{
			std::cout << "Erreur decriture dans le fichier." << std::endl;
			//! GERER L ERREUR 
		}
    }
	else
	{
        std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }

	client.messageRecv.clear();
}

// Set Body is now a Articulating function for better clarity
void	ResponseBuilder::setBody( Client & client, bool eof ){

	if (this->_isCGI)	
		return this->_cgi.setBody(client, eof);
	else if (_isMultipart)
		setMultiPartPost(client);
	else
		setRegularPost(client);
}
