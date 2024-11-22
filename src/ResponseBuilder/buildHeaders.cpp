#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::buildHeaders(){

	Logger::getInstance().log(DEBUG, "ResponseBuilder->buildHeader", *_client);


	errorCode codes;

	stringstream	streamStatusLine,
					// streamTimeStamp,
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
		
		// Content Type
		string contentType = extractType(_fileExtension);
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
						<< HTTP_HEADER_SEPARATOR;
		Headers.location = streamLocation.str();
	}


	// ======================== BONUS METHODS ========================

	// TODO : Coockie and session generator
	{
	}

	// ======================== BONUS METHODS ========================

	
	// ======================== BUILDING FINAL HEADERS ========================

	streamMasterHeader	<< Headers.statusLine
						// << Headers.timeStamp
						<< Headers.contentLenght
						// Optionals
						<< Headers.contentType 
						<< Headers.location
						// Separator
						<< HTTP_HEADER_SEPARATOR;
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}