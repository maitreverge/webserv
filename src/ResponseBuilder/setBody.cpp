#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::initBoundaryTokens( void ){

	// To replace with Dan future parsed token.
	string parsedBoundary = _client->headerRequest.getWebToken();
	// string parsedBoundary = "----WebKitFormBoundary1XN99skGpOHP8Og8";

	_tokenDelim = parsedBoundary;
	_tokenDelim.insert(0, "--");

	_tokenEnd = _tokenDelim + "--";

	_parsedBoundaryToken = true;
}

bool ResponseBuilder::isLineDelim(vector<char>& curLine, vector<char>& nextLine)
{
	char* separator = "\r\n";
    size_t separatorLength = 2;

	// Look for "\r\n" in curLine.
    vector< char >::iterator it = std::search(curLine.begin(), curLine.end(), separator, separator + separatorLength);

    if (it == curLine.end())
        return false;

    // Does the curLine end with a trailing \r\n ONLY
    if (it + separatorLength == curLine.end())
        return true;

    // In the opposite case, we need to trim the curLine and append the rest to nextLine
    nextLine.insert(nextLine.end(), it + separatorLength, curLine.end());
    curLine.erase(it + separatorLength, curLine.end());

    return true;
}

void	ResponseBuilder::extractFileBodyName( vector< char >& curLine ){

	string temp(curLine.begin(), curLine.end());

	string needle = "filename=\"";

	size_t startPos = temp.find(needle);
	size_t endPos;

	// TODO : edge case file names
	if (startPos != std::string::npos)
	{
		startPos += needle.length();

		endPos = temp.find("\"", startPos);

		_fileStreamName = temp.substr(startPos, endPos - startPos);

		string uploadFolder = _myconfig.uploadDirectory + "/";

		// TODO : check the uploadDirectory authorizations
		_fileStreamName.insert(0, uploadFolder);
	}
}

ResponseBuilder::e_lineNature ResponseBuilder::processCurrentLine(vector< char >& curLine) {

	if (curLine.size() > 2)
	{
        curLine.erase(curLine.end() - 2, curLine.end());
    }

	string temp(curLine.begin(), curLine.end());
	// Trimm last two trailing character from the current line

	if (_writeReady)
		return BINARY_DATA;
	else if (temp == _tokenEnd)
		return TOKEN_END;
	else if (temp == _tokenDelim)
		return TOKEN_DELIM;
	else if (temp == HTTP_HEADER_SEPARATOR)
		return LINE_SEPARATOR;
	else if (temp.rfind("Content-Disposition: ", 0) == 0) // does the beggining of the line starts with the needle
		return CONTENT_DISPOSITION;
	return OTHER;
}

// This is now the SetBody only for MultiPart form Data
void	ResponseBuilder::setMultiPartPost( Client & client ){

	printColor(BOLD_HIGH_INTENSITY_BLUE, "FUNCTION CALED");

	static vector< char > curLine;
	static vector< char > nextLine;
	e_lineNature lineNature;

	if (not _parsedBoundaryToken) // skip useless stack calls for each HTTP package
		initBoundaryTokens();
	
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

	// Clear the buffer from the client
	client.messageRecv.clear();
	
	// While we didn't process a whole line, we write it within the buffer
	if (not isLineDelim(curLine, nextLine))
	{
		// printColor(BOLD_CYAN, "Unfinished line");
		return;
	}

	string curent(curLine.begin(), curLine.end());
    printColor(BOLD_HIGH_INTENSITY_GREEN, "CURRENT LINE = " + curent);
	sleep(6);
		
	lineNature = processCurrentLine(curLine);

	// ! WIP NEEDLE 🪡🪡🪡🪡🪡🪡🪡🪡
	switch (lineNature)
	{
		case TOKEN_DELIM:
		case TOKEN_END: // end of previous stream
			if (_ofs.is_open())
				_ofs.close();
			printColor(BOLD_CYAN, "Token Delim or END detected, closing stream");
			// curLine.clear(); // put in another scope to avoid boilerplate code
			_fileStreamName.clear();
			break;

		case CONTENT_DISPOSITION:
			printColor(BOLD_CYAN, "Content Disposition Detected");
			extractFileBodyName(curLine);
			printColor(BOLD_CYAN, "_fileStreamName =" + _fileStreamName);
			// curLine.clear();
			break;
		
		case OTHER:
			printColor(BOLD_CYAN, "Other Line detected");
			// curLine.clear();
			break;
		
		case LINE_SEPARATOR: // next packages need to be the bnary data
			printColor(BOLD_CYAN, "Line separator detected");
			if (!this->_ofs.is_open())
			{
				this->_ofs.open(_fileStreamName.c_str(), std::ios::binary);
			}
			_writeReady = true; // TODO : empty file names or shitty ones ?
			// curLine.clear();
			break;

		case BINARY_DATA:
			printColor(BOLD_CYAN, "Binary data detected, writting");
			// this->_ofs.seekp(0, std::ios::end);
			_ofs.write(curLine.data(), static_cast<std::streamsize>(curLine.size()));
			if (!_ofs)
			{
				// error CODE_500 ??
				//Utile de rappeller getHeader ou renvoyer une exception a Seb pour qu'il puisse me rappeller avec un getHeader(.., .., CODE_500)
			}
			_writeReady = false;
			_fileStreamName.clear();
			break;

		default:
			break;
	}

	curLine.clear();

}

// This is now the regular setPost without Multipart Writting
void	ResponseBuilder::setRegularPost( Client & client ){

    Logger::getInstance().log(DEBUG, "setRegularPost");

    // static std::ofstream ofs("uploads/image_upload.jpeg", std::ios::binary);
	if (!this->_ofs.is_open())
	{	
		Logger::getInstance().log(INFO, _realURI.c_str());	
		
		string targetToWrite =	_uploadTargetDirectory
								+ generateFileName()
								+ _setBodyExtension;
		
		this->_ofs.open(targetToWrite.c_str(), std::ios::binary);
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
	else if (_myconfig.uploadAllowed == true and !_isMultipart)
		setRegularPost(client);
	else
		setMultiPartPost(client);
}
