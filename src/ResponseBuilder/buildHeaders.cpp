#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::buildHeaders(){

	Logger::getInstance().log(DEBUG, "ResponseBuilder->buildHeader", *_client);


	errorCode codes;

	stringstream	streamStatusLine,
					streamTimeStamp,
					streamContentLenght,
					streamMasterHeader;

	
	// -------------- Madatory Headers --------------  

	// ✅ GET FRIENDLY ✅ POST FRIENDLY ✅ DELETE FRIENDLY
	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< _errorType
						<< SPACE 
						<< codes.getCode(_errorType)
						<< HTTP_HEADER_SEPARATOR;
	Headers.statusLine = streamStatusLine.str();

	// ✅ GET FRIENDLY ✅ POST FRIENDLY ✅ DELETE FRIENDLY
	streamTimeStamp		<< "Date:"
						<< SPACE 
						<< timeStamp::getTime()
						<< HTTP_HEADER_SEPARATOR;
	Headers.timeStamp = streamTimeStamp.str();
	
	// ✅ GET FRIENDLY ✅ POST FRIENDLY ✅ DELETE FRIENDLY
	uint64_t result = (_errorType == CODE_204_NO_CONTENT or _errorType == CODE_307_TEMPORARY_REDIRECT ) ? 0 : Headers.bodyLenght;
	streamContentLenght	<< "Content-Length:"
						<< SPACE
						<< result
						<< HTTP_HEADER_SEPARATOR;
	Headers.contentLenght = streamContentLenght.str();

	
	// --------------  Optionals Headers --------------  
	// ✅ GET FRIENDLY ✅ POST FRIENDLY ⛔ DELETE FRIENDLY
	// if (Headers.bodyLenght > 0 and _method == GET) //! ERROR
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

	// ✅ REDIRECTION ONLY
	if (isErrorRedirect())
	{
		stringstream streamLocation;
		streamLocation	<< "Location:"
						<< SPACE
						<< _myconfig.redirection
						// << "http://www.github.com/maitreverge"
						<< HTTP_HEADER_SEPARATOR;
		Headers.location = streamLocation.str();
	}

	// ======================== POST HEADERS ========================
	// TODO : Implement a redirection logic for a POST request, for avoiding getting stuck
	// if (_method == POST and _errorType == CODE_201_CREATED)
	// {
	// 	stringstream streamLocation;
	// 	streamLocation	<< "Location:"
	// 					<< SPACE
	// 					<< // ! NEEDLE WORK
	// 					<< HTTP_HEADER_SEPARATOR;
	// 	Headers.location = streamLocation.str();
	// }


	// ======================== POST HEADERS ========================


	// ======================== BONUS METHODS ========================
	// TODO : Coockie and session generator
	{

	}
	// ======================== BONUS METHODS ========================

	
	// ======================== BUILDING FINAL HEADERS ========================

	streamMasterHeader	<< Headers.statusLine
						<< Headers.timeStamp
						<< Headers.contentType 
						// Optionals
						<< Headers.location
						// << (Headers.bodyLenght ? Headers.contentLenght : Headers.transfertEncoding)
						<< Headers.contentLenght
						<< HTTP_HEADER_SEPARATOR; // HEADER / BODY SEPARATOR
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}