#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::initBoundaryTokens( void ){

	// To replace with Dan future parsed token.
	string parsedBoundary = "----WebKitFormBoundary1XN99skGpOHP8Og8";

	_tokenDelim = parsedBoundary;
	_tokenDelim.insert(0, "--");

	_tokenEnd = _tokenDelim + "--";
}

bool	ResponseBuilder::isLineDelim( vector< char >& curLine, vector< char >& nextLine ){

	// (void)(curLine);
	string temp(curLine.begin(), curLine.end());

	std::string::size_type posSeparator;

	posSeparator = temp.find(HTTP_HEADER_SEPARATOR);
	if (posSeparator == std::string::npos)
		return false;
	
	// Does the curLine ends with a trailing \r\n ONLY
	if (posSeparator + 2 == temp.size())
		return true;
	else // in the opposite case, we need to trim the curLine and append the rest to nextLine
	{
		posSeparator += 2;
		nextLine.insert(nextLine.end(), (temp.begin() + static_cast<long>(posSeparator)), temp.end());
		curLine.erase(curLine.begin() + static_cast<long>(posSeparator), curLine.end());
	}

	return true;
}

e_lineNature	ResponseBuilder::processCurrentLine( vector< char >& curLine ){

	// This function serves the purpose of extracting the filename
	string temp(curLine.begin(), curLine.end());

	return TOKEN_END;

}

void	ResponseBuilder::boundarySetBodyPost( Client & client, bool eof ){

	usleep(50000);
    Logger::getInstance().log(DEBUG, "setBodyPost");

	if (this->_isCGI)	
		return this->_cgi.setBody(client, eof);

	vector< char > recVector;
	static vector< char > curLine;
	static vector< char > nextLine;

	// if (!_tokenEnd.empty()) // ! if this condition might overlap on others functions calls ?
	{
		initBoundaryTokens();
	}

	if ( !this->_ofs.is_open() )
	{
		if (!nextLine.empty())
		{
			curLine.insert(curLine.end(), nextLine.begin(), nextLine.end());
			nextLine.clear();
		}

		recVector = client.messageRecv;
		curLine.insert(curLine.end(), recVector.begin(), recVector.end());
		if (not isLineDelim(curLine, nextLine))
			return;
		// ! WORK NEEDLE 🪡🪡🪡🪡🪡🪡🪡🪡
		processCurrentLine(curLine);
		

	}

    if (_ofs.is_open())
	{

	}
	// _postFileName // ! Variable to init during reception

	// ! STEP 1 : Extract the current line until I met \r\n

	// ? CASE 1 : Is the current line a _tokenDelim ?
	// ? CASE 2 : Is the current line a _tokenEnd ?
	// ? CASE 2 : Is the current neither a _tokenDelim or a _tokenEnd
}

// Blueprint function
void	ResponseBuilder::_setBodyPost( Client & client, bool eof ){

	usleep(50000);
    Logger::getInstance().log(DEBUG, "setBodyPost");

	if (this->_isCGI)	
		return this->_cgi.setBody(client, eof);
	
    // static std::ofstream ofs("uploads/image_upload.jpeg", std::ios::binary);
	if (!this->_ofs.is_open())
	{	
		Logger::getInstance().log(INFO, _realURI.c_str());	
		string pathToWrite = "uploads/apple.jpeg";
		this->_ofs.open(pathToWrite.c_str(), std::ios::binary);
			//! GERER L ERREUR
	}

	// _ofs.clear();//!
    if (_ofs.is_open()) {
		this->_ofs.seekp(0, std::ios::end);
		// this->_ofs.end();
        _ofs.write(client.messageRecv.data(), static_cast<std::streamsize>(client.messageRecv.size())); 
        _ofs.flush();
		if (!_ofs)
		{
			std::cout << "Erreur decriture dans le fichier." << std::endl;
			//! GERER L ERREUR 
		}
    } else {
        std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
	client.messageRecv.clear();
}