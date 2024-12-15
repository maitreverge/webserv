#include "ResponseBuilder.hpp"
#include "Logger.hpp"


/**
 * @brief Builds the HTTP response headers.
 * 
 * This function constructs the mandatory and optional HTTP headers for the response.
 * It includes the status line, content length, content type, and location headers.
 * Additionally, it handles error redirection and sets cookies if applicable.
 * The final headers are stored in a master header vector.
 */
void	ResponseBuilder::buildHeaders(){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::buildHeaders", *_client);

	errorCode codes;
	
	stringstream	streamStatusLine,
					// streamTimeStamp, // TODO : Put Timestamps back once webserv finished
					streamContentLenght,
					streamMasterHeader;

	
	// -------------- Madatory Headers --------------  

	// ! Status Line
	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< _errorType
						<< SPACE 
						<< codes.getCode(_errorType)
						<< HTTP_HEADER_SEPARATOR;
	Headers.statusLine = streamStatusLine.str();

	// TODO : Put Timestamps back once webserv finished
	// ! Date:
	// streamTimeStamp		<< "Date:"
	// 					<< SPACE 
	// 					<< timeStamp::getTime()
	// 					<< HTTP_HEADER_SEPARATOR;
	// Headers.timeStamp = streamTimeStamp.str();
	
	// ! Content-Length
	if (not isErrorRedirect())
	{
		uint64_t result = (_errorType == CODE_204_NO_CONTENT ) ? 0 : Headers.bodyLenght;
		streamContentLenght	<< "Content-Length:"
							<< SPACE
							<< result
							<< HTTP_HEADER_SEPARATOR;
		Headers.contentLenght = streamContentLenght.str();
	}

	// --------------  Optionals Headers --------------  
	// ! Content-Type
	if (Headers.bodyLenght > 0)
	{
		stringstream streamContentType;
		string contentType;
		
		if (_method == DELETE and _errorType == CODE_200_OK)
			contentType = "text/html";
		else
			contentType = extractType(_fileExtension);
		
		streamContentType	<< "Content-Type:"
							<< SPACE 
							<< contentType 
							<< HTTP_HEADER_SEPARATOR;
		
		Headers.contentType = streamContentType.str();
	}

	// ! Location:
	if (isErrorRedirect())
	{
		stringstream streamLocation;
		streamLocation	<< "Location:"
						<< SPACE
						<< _realURI
						<< HTTP_HEADER_SEPARATOR
						<< "Content-Length: 0"
						<< HTTP_HEADER_SEPARATOR
						<< "Connection: close\r\n"
						<< HTTP_HEADER_SEPARATOR;
		Headers.location = streamLocation.str();
	}

	// ======================== BONUS METHODS ========================

	buildSetCookieHeader();

	// ======================== BUILDING FINAL HEADERS ========================

	streamMasterHeader	<< Headers.statusLine
						// << Headers.timeStamp // TODO : Put Timestamps back once webserv finished
						<< Headers.contentLenght
						// Optionals
						<< Headers.cookie
						<< Headers.contentType 
						<< Headers.location
						<< HTTP_HEADER_SEPARATOR;
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}