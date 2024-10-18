#include "errorCode.hpp"

errorCode::errorCode( void ){

	/*
		Invoking the constructor actually fills up the maps with
		CODE = Corresponding String

		ex : CODE_404_NOT_FOUND = "Not Found"
	*/
	for (size_t code = CODE_100_CONTINUE; code <= CODE_511_NETWORK_AUTHENTICATION_REQUIRED; ++code)
	{
		//  Need static cast, because `code` is actually a int within this loop.
		_mapErrorCode.insert(std::make_pair(static_cast<e_errorCodes>(code), getCorrespondingString(static_cast<e_errorCodes>(code))));
	}
}


/**
 * @brief Retrieves the error code string corresponding to the given error code enumeration.
 * 
 * This function looks up the provided error code enumeration in the internal map and returns
 * the associated error code string.
 * 
 * @param codeInput The error code enumeration value to look up.
 * @return const string& The error code string corresponding to the provided enumeration.
 * 
 * @throws std::out_of_range If the provided error code enumeration is not found in the map.
 */
const string &errorCode::getCode( const e_errorCodes &codeInput )const{

	return _mapErrorCode.at(codeInput);
}


const string errorCode::getCorrespondingString( e_errorCodes inputCode) const {
    
	switch (inputCode)
	{
        case CODE_100_CONTINUE: return "Continue";
        case CODE_101_SWITCHING_PROTOCOLS: return "Switching Protocols";
        case CODE_200_OK: return "OK";
        case CODE_201_CREATED: return "Created";
        case CODE_202_ACCEPTED: return "Accepted";
        case CODE_203_NON_AUTHORITATIVE_INFORMATION: return "Non-Authoritative Information";
        case CODE_204_NO_CONTENT: return "No Content";
        case CODE_205_RESET_CONTENT: return "Reset Content";
        case CODE_206_PARTIAL_CONTENT: return "Partial Content";
        case CODE_300_MULTIPLE_CHOICES: return "Multiple Choices";
        case CODE_301_MOVED_PERMANENTLY: return "Moved Permanently";
        case CODE_302_FOUND: return "Found";
        case CODE_303_SEE_OTHER: return "See Other";
        case CODE_304_NOT_MODIFIED: return "Not Modified";
        case CODE_305_USE_PROXY: return "Use Proxy";
        case CODE_307_TEMPORARY_REDIRECT: return "Temporary Redirect";
        case CODE_400_BAD_REQUEST: return "Bad Request";
        case CODE_401_UNAUTHORIZED: return "Unauthorized";
        case CODE_402_PAYMENT_REQUIRED: return "Payment Required";
        case CODE_403_FORBIDDEN: return "Forbidden";
        case CODE_404_NOT_FOUND: return "Not Found";
        case CODE_405_METHOD_NOT_ALLOWED: return "Method Not Allowed";
        case CODE_406_NOT_ACCEPTABLE: return "Not Acceptable";
        case CODE_407_PROXY_AUTHENTICATION_REQUIRED: return "Proxy Authentication Required";
        case CODE_408_REQUEST_TIMEOUT: return "Request Timeout";
        case CODE_409_CONFLICT: return "Conflict";
        case CODE_410_GONE: return "Gone";
        case CODE_411_LENGTH_REQUIRED: return "Length Required";
        case CODE_412_PRECONDITION_FAILED: return "Precondition Failed";
        case CODE_413_PAYLOAD_TOO_LARGE: return "Payload Too Large";
        case CODE_414_URI_TOO_LONG: return "URI Too Long";
        case CODE_415_UNSUPPORTED_MEDIA_TYPE: return "Unsupported Media Type";
        case CODE_416_RANGE_NOT_SATISFIABLE: return "Range Not Satisfiable";
        case CODE_417_EXPECTATION_FAILED: return "Expectation Failed";
        case CODE_426_UPGRADE_REQUIRED: return "Upgrade Required";
        case CODE_500_INTERNAL_SERVER_ERROR: return "Internal Server Error";
        case CODE_501_NOT_IMPLEMENTED: return "Not Implemented";
        case CODE_502_BAD_GATEWAY: return "Bad Gateway";
        case CODE_503_SERVICE_UNAVAILABLE: return "Service Unavailable";
        case CODE_504_GATEWAY_TIMEOUT: return "Gateway Timeout";
        case CODE_505_HTTP_VERSION_NOT_SUPPORTED: return "HTTP Version Not Supported";
        case CODE_511_NETWORK_AUTHENTICATION_REQUIRED: return "Network Authentication Required";
        default: return "UNKNOWN TYPE";
    }
}

// Destructor
errorCode::~errorCode( void ){}