#include "../../includes/buildResponse.hpp" // relative path for local


buildResponse::buildResponse( void ){

	// Building mimeTypes 
	// map<string, string> temp;

	// Textual Content Types
	_mimeTypes.insert(std::make_pair("html", "text/html"));
	_mimeTypes.insert(std::make_pair("htm", "text/htm"));
	_mimeTypes.insert(std::make_pair("txt", "text/txt"));
	_mimeTypes.insert(std::make_pair("css", "text/css"));
	_mimeTypes.insert(std::make_pair("xml", "text/xml"));
	// Application Content Types
	_mimeTypes.insert(std::make_pair("js", "application/javascript")); // FIXME doubt on this one
	_mimeTypes.insert(std::make_pair("json", "application/json"));
	_mimeTypes.insert(std::make_pair("pdf", "application/pdf"));
	_mimeTypes.insert(std::make_pair("zip", "application/zip"));
	// Image Content Types
	_mimeTypes.insert(std::make_pair("jpeg", "image/jpeg"));
	_mimeTypes.insert(std::make_pair("jpg", "image/jpg"));
	_mimeTypes.insert(std::make_pair("png", "image/png"));
	_mimeTypes.insert(std::make_pair("gif", "image/gif"));
	_mimeTypes.insert(std::make_pair("webp", "image/webp"));
	_mimeTypes.insert(std::make_pair("bmp", "image/bmp"));
	// Audio Content Types
	_mimeTypes.insert(std::make_pair("mp3", "audio/mp3"));
	_mimeTypes.insert(std::make_pair("mpeg", "audio/mpeg"));
	_mimeTypes.insert(std::make_pair("ogg", "audio/ogg"));
	_mimeTypes.insert(std::make_pair("wav", "audio/wav"));
	// Video Content Types
	_mimeTypes.insert(std::make_pair("mp4", "video/mp4"));
	_mimeTypes.insert(std::make_pair("webm", "video/webm"));
	_mimeTypes.insert(std::make_pair("ogv", "video/ogv"));

	// _mimeTypes(_mimeTypes);
}

buildResponse::buildResponse( const buildResponse& copy ){ static_cast<void>(copy); }


buildResponse& buildResponse::operator=(const buildResponse& right_operator){
	
	static_cast<void>(right_operator);
    return *this;
}

// ! DESTRUCTOR
buildResponse::~buildResponse( void ){}

// *	------------------- GETTERS ----------------------------------
const vector<char>& buildResponse::getHttpResponse( void )const{ return _httpResponse; }
const string &buildResponse::getStatusLine( void )const{ return _statusLine; }
const string &buildResponse::getTimeStamp( void )const{ return _timeStamp; }
const string &buildResponse::getContentType( void )const{ return _contentType; }
const string &buildResponse::getTransfertEncoding( void )const{ return _transfertEncoding; }
const string &buildResponse::getContentLenght( void )const{ return _contentLenght; }
const string &buildResponse::getBody( void )const{ return _body; }


// *	------------------- SETTERS ----------------------------------

void	buildResponse::setStatusLine( string input ){ _statusLine = input; }
void	buildResponse::setTimeStamp( string input ){ _timeStamp = input; }
void	buildResponse::setContentType( string input ){ _contentType = input; }
void	buildResponse::setTransfertEncoding( string input ){ _transfertEncoding = input; }
void	buildResponse::setContentLenght( string input ){ _contentLenght = input; }
void	buildResponse::setBody( string input ){ _body = input; }



ostream& operator<<( ostream& output_stream, const buildResponse& right_input ){
	
	output_stream << BOLD_HIGH_INTENSITY_GREEN << "Status Line = \n";
	output_stream << right_input.getStatusLine();

	output_stream << BOLD_BLUE << "\n\nTimeStamp = \n";
	output_stream << right_input.getTimeStamp();

	output_stream << BOLD_BLUE << "\n\nContent-Type = \n";
	output_stream << right_input.getContentType();

	output_stream << BOLD_BLUE << "\n\nTransfert-Encoding = \n";
	output_stream << right_input.getTransfertEncoding();

	output_stream << BOLD_BLUE << "\n\nContent-Lenght = \n";
	output_stream << right_input.getContentLenght();

	output_stream << BOLD_HIGH_INTENSITY_RED << "\n\nHTTP BODY = \n";
	output_stream << right_input.getBody();

	output_stream << RESET;

	return output_stream;
}

void buildResponse::buildBody( const vector<char>& bodyInput ){

	// Extract the vector towards a string.
	string tempBody = std::string(bodyInput.begin(), bodyInput.end());
	
	_body += HTTP_REPONSE_SEPARATOR;
	_body += tempBody;

	// Calculate size of body for headers. (!= from lenght() )
	_bodyLenght = tempBody.size();
}

void buildResponse::buildHeaders( const e_errorCodes &errCode, const string &fileName ){

	// Status line
	errorCode codes;
	stringstream	streamStatusLine,
					streamTimeStamp,
					streamContentType,
					streamContentLenght,
					streamMasterHeader,
					stream;

	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< errCode
						<< SPACE 
						<< codes.getCode(errCode)
						<< HTTP_REPONSE_SEPARATOR;
	
	_statusLine = streamStatusLine.str();

	// TimeStamp
	// TODO Implement a TimeStamp generator
	streamTimeStamp	<< "Date:"
					<< SPACE 
					<< "Wed, 11 Oct 2024 10:24:12 GMT"
					<< HTTP_REPONSE_SEPARATOR;

	_timeStamp = streamTimeStamp.str();

	/*
		! NOTE
		Using multipart in responses is less common and is typically reserved
		for specialized cases rather than everyday web browsing scenarios.
		For Webserv, implementing multipart responses would be more
		of an advanced feature rather than a core requirement.
	*/
	// * Content-Type (if body)
	if (this->_bodyLenght > 0)
	{
		string contentType = buildContentType(fileName);
		streamContentType	<< "Content-Type:"
							<< SPACE 
							<< contentType 
							<< HTTP_REPONSE_SEPARATOR;
		
		_contentType = streamContentType.str();
	}

	// ContentLenght
	streamContentLenght	<< "Content-Lenght:"
						<< SPACE
						<< _bodyLenght
						<< HTTP_REPONSE_SEPARATOR;
	
	_contentLenght = streamContentLenght.str();

	// Building Final Headers
	streamMasterHeader	<< _statusLine
						<< _timeStamp
						<< _contentType
						<< (_bodyLenght ? _contentLenght : _transfertEncoding);
	
	_masterHeader = streamMasterHeader.str();
	
	// FIXME
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
string buildResponse::buildContentType( const string &typeFile )const{

	string result, extension;

	string::size_type extensionIndex = typeFile.find_last_of(".");
	extension = typeFile.substr(extensionIndex + 1);

	result = extractType(extension);

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
string buildResponse::extractType( const string& extension ) const {
    
    map<string, string>::const_iterator it = _mimeTypes.find(extension);
    if (it != _mimeTypes.end())
        return it->second;
    else
        return "application/octet-stream"; // Default MIME type
}

void buildResponse::assembleResponse( void ){

	string assemble;

	assemble += _masterHeader;
	assemble += _body;

	_httpResponse.insert(_httpResponse.end(), assemble.begin(), assemble.end());
}