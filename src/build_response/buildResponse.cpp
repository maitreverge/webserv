#include "buildResponse.hpp"


buildResponse::buildResponse( void ){

	// TODO : List init some values ?
}


buildResponse::buildResponse( const buildResponse& copy ) :
	_httpResponse(copy._httpResponse),
	_statusLine(copy._statusLine),
	_timeStamp(copy._timeStamp),
	_contentType(copy._contentType),
	_transfertEncoding(copy._transfertEncoding),
	_contentLenght(copy._contentLenght),
	_body(copy._body){}


buildResponse& buildResponse::operator=( const buildResponse& right_operator ){
	if (this != &right_operator){

		// Reassign every value with the getter value 
		this->_httpResponse = right_operator.getHttpResponse();
		
		this->_statusLine = right_operator.getStatusLine();
		this->_timeStamp = right_operator.getTimeStamp();
		this->_contentType = right_operator.getContentType();
		this->_transfertEncoding = right_operator.getTransfertEncoding();
		this->_contentLenght = right_operator.getContentLenght();
		this->_body = right_operator.getBody();
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
const string &buildResponse::getStatusLine( void )const{ return _statusLine; }
const string &buildResponse::getTimeStamp( void )const{ return _timeStamp; }
const string &buildResponse::getContentType( void )const{ return _contentType; }
const string &buildResponse::getTransfertEncoding( void )const{ return _transfertEncoding; }
const string &buildResponse::getContentLenght( void )const{ return _contentLenght; }
const string &buildResponse::getBody( void )const{ return _body; }


// *	------------------- SETTERS ----------------------------------

// ! could be string or char* as an input
void buildResponse::setHttpResponse( vector<char> input){

	// TODO : concatenate to the vector on differents ways depending if it's a string, a vector or a `char*`
}

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

	return output_stream;
}

void buildResponse::buildBody( vector<char>& bodyInput){

	// Extract the vector towards a string.
	string tempBody = std::string( bodyInput.begin(), bodyInput.end());
	
	_body += HTTP_REPONSE_SEPARATOR;
	_body += tempBody;

	// Calculate size of body for headers. (!= from lenght() )
	_bodyLenght = tempBody.size();
}

void buildResponse::buildHeaders( e_errorCodes &errorCode, string &fileName ){

	// Status line
	{
		_statusLine << HTTP_PROTOCOL
					<< SPACE
					<< errorCode
					<< SPACE 
					<< codes.getCodes(errorCode) // TODO : plug the class
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
		_contentType	<< "Content-Type:"
						<< SPACE 
						<< "???" // TODO : generate a correct _contentType depending on what I'm about to recieve 
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
						<< (_bodyLenght ? _contentLenght.str() : _transfertEncoding.str())
						<< HTTP_REPONSE_SEPARATOR; 
	}	
}

stringstream buildResponse::buildContentType( string typeFile )const{

	stringstream result;
	string type, extension;

	string::size_type extensionIndex = typeFile.find_last_of(".");
	extension = typeFile.substr(extensionIndex + 1);


	type = extractType(extension);
	result << type << "/" << extension;

	return result;
}

string buildResponse::extractType( string& extension ) const {
    
	static const std::map<string, string> mimeTypes = {
        
		// Textual Content Types
        std::make_pair("html", "text/html"),
        std::make_pair("htm", "text/html"),
        std::make_pair("txt", "text/plain"),
        std::make_pair("css", "text/css"),
        std::make_pair("xml", "text/xml"),
        
		// Application Content Types
        std::make_pair("js", "application/javascript"),
        std::make_pair("json", "application/json"),
        std::make_pair("pdf", "application/pdf"),
        std::make_pair("zip", "application/zip"),
        
		// Image Content Types
        std::make_pair("jpeg", "image/jpeg"),
        std::make_pair("jpg", "image/jpeg"),
        std::make_pair("png", "image/png"),
        std::make_pair("gif", "image/gif"),
        std::make_pair("webp", "image/webp"),
        std::make_pair("bmp", "image/bmp"),
        
		// Audio and Video Content Types
        std::make_pair("mp3", "audio/mp3"),
        std::make_pair("mpeg", "audio/mpeg"),
        std::make_pair("ogg", "audio/ogg"),
        std::make_pair("wav", "audio/wav"),
        std::make_pair("mp4", "video/mp4"),
        std::make_pair("webm", "video/webm"),
        std::make_pair("ogv", "video/ogg")
    };

    map<string, string>::const_iterator it = mimeTypes.find(extension);
    if (it != mimeTypes.end())
        return it->second;
    else
        return "application/octet-stream"; // Default MIME type
}

