#include "../../includes/buildResponse.hpp"


buildResponse::buildResponse( void ){

	// Building mimeTypes 
	map<string, string> temp;

	// Textual Content Types
	temp.insert(std::make_pair("html", "text"));
	temp.insert(std::make_pair("htm", "text"));
	temp.insert(std::make_pair("txt", "text"));
	temp.insert(std::make_pair("css", "text"));
	temp.insert(std::make_pair("xml", "text"));
	// Application Content Types
	temp.insert(std::make_pair("js", "application"));
	temp.insert(std::make_pair("json", "application"));
	temp.insert(std::make_pair("pdf", "application"));
	temp.insert(std::make_pair("zip", "application"));
	// Image Content Types
	temp.insert(std::make_pair("jpeg", "image"));
	temp.insert(std::make_pair("jpg", "image"));
	temp.insert(std::make_pair("png", "image"));
	temp.insert(std::make_pair("gif", "image"));
	temp.insert(std::make_pair("webp", "image"));
	temp.insert(std::make_pair("bmp", "image"));
	// Audio Content Types
	temp.insert(std::make_pair("mp3", "audio"));
	temp.insert(std::make_pair("mpeg", "audio"));
	temp.insert(std::make_pair("ogg", "audio"));
	temp.insert(std::make_pair("wav", "audio"));
	// Video Content Types
	temp.insert(std::make_pair("mp4", "video"));
	temp.insert(std::make_pair("webm", "video"));
	temp.insert(std::make_pair("ogv", "video"));

	_mimeTypes = temp;
}


buildResponse::buildResponse( const buildResponse& copy ) :
	_mimeTypes(copy._mimeTypes),
	_httpResponse(copy._httpResponse),
	_statusLine(copy._statusLine.str()),
	_timeStamp(copy._timeStamp.str()),
	_contentType(copy._contentType.str()),
	_transfertEncoding(copy._transfertEncoding.str()),
	_contentLenght(copy._contentLenght.str()),
	_body(copy._body)
	{}


buildResponse& buildResponse::operator=(const buildResponse& right_operator){
    if (this != &right_operator)
	{
        // Reassign every value with the getter value 
        this->_httpResponse = right_operator._httpResponse;
        
        this->_statusLine.str(right_operator._statusLine.str());
        this->_timeStamp.str(right_operator._timeStamp.str());
        this->_contentType.str(right_operator._contentType.str());
        this->_transfertEncoding.str(right_operator._transfertEncoding.str());
        this->_contentLenght.str(right_operator._contentLenght.str());
        this->_body = right_operator._body;
        this->_mimeTypes = right_operator._mimeTypes;
    }
    return *this;
}

// ! DESTRUCTOR
buildResponse::~buildResponse( void ){

	// TODO : clean the httpResponse on each run ??
	_httpResponse.clear();

	_statusLine.clear();
	_timeStamp.clear();
	_contentType.clear();
	_transfertEncoding.clear();
	_contentLenght.clear();
	_body.clear();
}

// *	------------------- GETTERS ----------------------------------
const vector<char>& buildResponse::getHttpResponse( void )const{ return _httpResponse; }
const stringstream &buildResponse::getStatusLine( void )const{ return _statusLine; }
const stringstream &buildResponse::getTimeStamp( void )const{ return _timeStamp; }
const stringstream &buildResponse::getContentType( void )const{ return _contentType; }
const stringstream &buildResponse::getTransfertEncoding( void )const{ return _transfertEncoding; }
const stringstream &buildResponse::getContentLenght( void )const{ return _contentLenght; }
const string &buildResponse::getBody( void )const{ return _body; }


// *	------------------- SETTERS ----------------------------------

void	buildResponse::setStatusLine( string input ){ _statusLine << input; }
void	buildResponse::setTimeStamp( string input ){ _timeStamp << input; }
void	buildResponse::setContentType( string input ){ _contentType << input; }
void	buildResponse::setTransfertEncoding( string input ){ _transfertEncoding << input; }
void	buildResponse::setContentLenght( string input ){ _contentLenght << input; }
void	buildResponse::setBody( string input ){ _body = input; }



ostream& operator<<( ostream& output_stream, const buildResponse& right_input ){
	
	output_stream << BOLD_HIGH_INTENSITY_GREEN << "Status Line = \n";
	output_stream << right_input.getStatusLine().str();

	output_stream << BOLD_BLUE << "\n\nTimeStamp = \n";
	output_stream << right_input.getTimeStamp().str();

	output_stream << BOLD_BLUE << "\n\nContent-Type = \n";
	output_stream << right_input.getContentType().str();

	output_stream << BOLD_BLUE << "\n\nTransfert-Encoding = \n";
	output_stream << right_input.getTransfertEncoding().str();

	output_stream << BOLD_BLUE << "\n\nContent-Lenght = \n";
	output_stream << right_input.getContentLenght().str();

	output_stream << BOLD_HIGH_INTENSITY_RED << "\n\nHTTP BODY = \n";
	output_stream << right_input.getBody();

	return output_stream;
}

void buildResponse::buildBody( const vector<char>& bodyInput ){

	// Extract the vector towards a string.
	string tempBody = std::string( bodyInput.begin(), bodyInput.end());
	
	_body += HTTP_REPONSE_SEPARATOR;
	_body += tempBody;

	// Calculate size of body for headers. (!= from lenght() )
	_bodyLenght = tempBody.size();
}

void buildResponse::buildHeaders( const e_errorCodes &errCode, const string &fileName ){

	// Status line
	errorCode codes;

	{
		_statusLine << HTTP_PROTOCOL
					<< SPACE
					<< errCode
					<< SPACE 
					<< codes.getCode(errCode) // TODO : plug the class
					<< HTTP_REPONSE_SEPARATOR;
	}

	// TimeStamp
	{
		// TODO Implement a TimeStamp generator
		_timeStamp	<< "Date:"
					<< SPACE 
					<< "Wed, 11 Oct 2024 10:24:12 GMT"
					<< HTTP_REPONSE_SEPARATOR;
	}

	/*
		! NOTE
		Using multipart in responses is less common and is typically reserved
		for specialized cases rather than everyday web browsing scenarios.
		For Webserv, implementing multipart responses would be more
		of an advanced feature rather than a core requirement.
	*/
	// * Content-Type (if body)
	if (_bodyLenght)
	{
		string contentType = buildContentType(fileName).str();
		_contentType	<< "Content-Type:"
						<< SPACE 
						<< contentType
						<< HTTP_REPONSE_SEPARATOR;
	}

	// ContentLenght
	{
		_contentLenght	<< "Content-Lenght:"
						<< SPACE
						<< _bodyLenght
						<< HTTP_REPONSE_SEPARATOR;
	}

	// Building Final Headers
	{
		_masterHeader	<< _statusLine.str()
						<< _timeStamp.str()
						<< _contentType.str()
						<< (_bodyLenght ? _contentLenght.str() : _transfertEncoding.str());
	}
	// ! Potential hangling HTTP_SEPARATOR if there is extra headers or not !!!!
}

/**
 * @brief Constructs the Content-Type header value based on the file type.
 *
 * This method takes a file name or path, extracts its extension, and constructs
 * a Content-Type header value in the format "type/extension". The type is determined
 * by the extractType method.
 *
 * @param typeFile The name or path of the file from which to extract the extension.
 * @return A stringstream containing the constructed Content-Type header value.
 */
stringstream buildResponse::buildContentType( string typeFile )const{

	stringstream result;
	string type, extension;

	string::size_type extensionIndex = typeFile.find_last_of(".");
	extension = typeFile.substr(extensionIndex + 1);

	type = extractType(extension);
	result << type << "/" << extension;

	return result;
}

/**
 * @brief Extracts the MIME type based on the file extension.
 * 
 * This function searches for the provided file extension in the MIME types map.
 * If the extension is found, it returns the corresponding MIME type.
 * If the extension is not found, it returns the default MIME type "application/octet-stream".
 * 
 * @param extension The file extension for which the MIME type is to be extracted.
 * @return A string representing the MIME type corresponding to the given file extension.
 */
string buildResponse::extractType( string& extension ) const {
    
    map<string, string>::const_iterator it = _mimeTypes.find(extension);
    if (it != _mimeTypes.end())
        return it->second;
    else
        return "application/octet-stream"; // Default MIME type
}

void buildResponse::assembleResponse( void ){

	string assemble;

	assemble += _masterHeader.str();
	assemble += _body;

	_httpResponse.insert(_httpResponse.end(), assemble.begin(), assemble.end());
}