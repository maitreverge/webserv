#include "ResponseBuilder.hpp"
#include "Logger.hpp"

/**
 * @brief Initializes the list of current files in the upload target directory.
 *
 * This function opens the specified upload target directory and iterates through its contents.
 * It filters out ignored files and adds the paths of regular files to the provided vector of duplicate file names.
 *
 * @param duplicatesFileNames A reference to a vector of strings where the paths of duplicate files will be stored.
 */
void	ResponseBuilder::initCurrentFiles( vector< string>& duplicatesFileNames ){

	DIR *dir = opendir(_uploadTargetDirectory.c_str());

	if (dir == NULL)
	{
		Logger::getInstance().log(ERROR, "Failing Openning Directory in ResponseBuilder::initCurrentFiles");
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
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

/**
 * @brief Initializes the boundary tokens used for multipart HTTP responses.
 *
 * This function creates and initializes the boundary tokens (_tokenDelim and _tokenEnd)
 * used to delimit parts in a multipart HTTP response. The tokens are derived from the
 * client's web token and are appended with specific HTTP separators for easier parsing.
 */
void	ResponseBuilder::initBoundaryTokens( void ){

	// Create both tokens
	_tokenDelim = _client->headerRequest.getWebToken();
	_tokenDelim.insert(0, "--");

	_tokenEnd = _tokenDelim + "--";

	// Append traling HTTP_SEPARATOR for easier parsing
	_tokenDelim += HTTP_HEADER_SEPARATOR;
	_tokenEnd += HTTP_HEADER_SEPARATOR;
}

/**
 * @brief Determines the appropriate separator for the response body.
 *
 * This function sets the separator and its length based on the current state of the ResponseBuilder.
 * If the response is ready to be written, it first tries to use the token end separator. If the token end
 * separator is not found in the current line, it falls back to using the token delimiter separator.
 * If the response is not ready to be written, it uses the HTTP header separator.
 *
 * @param separator A reference to the string where the determined separator will be stored.
 * @param separatorLength A reference to the size_t where the length of the determined separator will be stored.
 * @param curLine A vector of characters representing the current line being processed.
 */
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

/**
 * @brief Searches for a specified separator within a given line.
 *
 * This function searches for the first occurrence of a specified separator
 * within a vector of characters and returns an iterator to the beginning
 * of the found sequence.
 *
 * @param curLine The vector of characters to search within.
 * @param separator The string representing the separator to search for.
 * @param separatorLength The length of the separator string.
 * @return An iterator to the beginning of the found separator sequence,
 *         or curLine.end() if the separator is not found.
 */
vector<char>::iterator ResponseBuilder::searchSeparator(vector<char>& curLine, string &separator, size_t &separatorLength){

    vector<char>::iterator it = std::search(curLine.begin(), curLine.end(),
		separator.c_str(),	separator.c_str() + separatorLength);

	return it;
}

/**
 * @brief Checks if the current line contains a line delimiter and processes it accordingly.
 *
 * This function searches for a line delimiter (e.g., "\r\n" or a custom token) within the given
 * current line. If a delimiter is found, it processes the line based on the delimiter's position
 * and updates the next line if necessary.
 *
 * @param curLine The current line being processed.
 * @param nextLine The next line to be updated if a delimiter is found.
 * @return true if a line delimiter is found and processed, false otherwise.
 */
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

/**
 * @brief Extracts the filename from the given line and constructs the full file path.
 *
 * This function searches for the filename in the provided line, constructs the full file path
 * by appending the upload target directory, and handles duplicate filenames by appending a 
 * random string if necessary. The resulting filename is stored in the _fileStreamName member 
 * and also added to the duplicatesFileNames vector to keep track of used filenames.
 *
 * @param curLine The current line being processed, represented as a vector of characters.
 * @param duplicatesFileNames A vector of strings to keep track of filenames that have already been used.
 */
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

		_fileStreamName.insert(0, "/");

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

/**
 * @brief Processes the current line and determines its nature.
 *
 * This function analyzes the given line to identify its type based on specific criteria.
 * It trims the last two trailing characters from the line if necessary and checks for
 * various conditions to classify the line as one of the predefined types.
 *
 * @param curLine A vector of characters representing the current line to be processed.
 * @return An enumeration value of type ResponseBuilder::e_lineNature indicating the nature of the line.
 */
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

/**
 * @brief Processes a multipart/form-data POST request from the client.
 *
 * This function handles the multipart/form-data POST request by reading the client's message,
 * parsing the boundary tokens, and processing each line of the request. It manages the state
 * of the current line, next line, and the nature of each line (e.g., token delimiter, content
 * disposition, binary data). It also handles writing binary data to a file stream.
 *
 * @param client Reference to the Client object containing the received message.
 */
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
				duplicatesFileNames.clear();
				_parsedBoundaryToken = false;
				break;

			case CONTENT_DISPOSITION:
				extractFileBodyName(curLine, duplicatesFileNames);
				break;
			
			case OTHER:
				break;
			
			case LINE_SEPARATOR: // Next processed lines will be the binary data

				this->_ofs.open(_fileStreamName.c_str(), std::ios::binary);

				if (!this->_ofs.is_open())
				{
					Logger::getInstance().log(ERROR, "ResponseBuilder::setMultiPartPost : Stream Failed to open", client);
					throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
				}

				_writeReady = true;
				break;

			case BINARY_DATA: // Writting actual data

				this->_ofs.write(curLine.data(), static_cast<std::streamsize>(curLine.size()));

				if (this->_ofs.is_open())
					this->_ofs.close();
				
				if (this->_ofs.is_open())
				{
					Logger::getInstance().log(ERROR, "ResponseBuilder::setMultiPartPost : Stream Failed to close", client);
					throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
				}
				
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

/**
 * @brief Handles the regular POST request by writing the received message to a file.
 * 
 * This function checks if the output file stream (_ofs) is open. If not, it constructs
 * the target file path using the upload target directory, a generated file name, and
 * a file extension, then opens the file in binary append mode. It writes the received
 * message from the client to the file and flushes the stream. If the file cannot be
 * opened or written to, it logs an error message.
 * 
 * @param client Reference to the Client object containing the received message.
 */
void	ResponseBuilder::setRegularPost( Client & client ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::setRegularPost", client);

	if (!this->_ofs.is_open())
	{	
		Logger::getInstance().log(INFO, _realURI.c_str(), client, L_VRB);	
		
		string targetToWrite =	_uploadTargetDirectory
								+ generateFileName()
								+ _setBodyExtension;
		
		// need to oppen it in append mode for seekp 
		this->_ofs.open(targetToWrite.c_str(), std::ios::binary | std::ios::app);
	}

    if (_ofs.is_open())
	{
		this->_ofs.seekp(0, std::ios::end);
        _ofs.write(client.messageRecv.data(), static_cast<std::streamsize>(client.messageRecv.size())); 
        _ofs.flush();
		if (!_ofs)
		{
			Logger::getInstance().log(ERROR, "ResponseBuilder::setRegularPost : Stream Failed to flush", client);
			throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
		}
    }
	else
	{
		Logger::getInstance().log(ERROR, "ResponseBuilder::setRegularPost : Stream Failed to open", client);
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
    }

	client.messageRecv.clear();
}

/**
 * @brief Sets the body of the response based on the client request and state.
 * 
 * This function determines the appropriate method to set the body of the response.
 * If the request is a CGI request, it delegates the task to the CGI handler.
 * If the request is a multipart form-data, it handles it accordingly.
 * Otherwise, it processes a regular POST request.
 * 
 * @param client Reference to the Client object containing the request details.
 * @param eof Boolean flag indicating if the end of the file has been reached.
 */
void	ResponseBuilder::setBody( Client & client, bool eof ){

	if (this->_isCGI)	
		return this->_cgi.setBody(client, eof);
	else if (_isMultipart)
		setMultiPartPost(client);
	else
		setRegularPost(client);
}