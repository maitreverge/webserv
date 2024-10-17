#include <iostream>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

typedef enum
{
	// Informational responses
	CODE_100_CONTINUE = 100,
	CODE_101_SWITCHING_PROTOCOLS = 101,
	CODE_102_PROCESSING = 102,

	// Successful responses
	CODE_200_OK = 200,
	CODE_201_CREATED = 201,
	CODE_202_ACCEPTED = 202,
	CODE_203_NON_AUTHORITATIVE_INFORMATION = 203,
	CODE_204_NO_CONTENT = 204,
	CODE_205_RESET_CONTENT = 205,
	CODE_206_PARTIAL_CONTENT = 206,
	CODE_207_MULTI_STATUS = 207,
	CODE_208_ALREADY_REPORTED = 208,
	CODE_226_IM_USED = 226,

	// Redirection messages
	CODE_300_MULTIPLE_CHOICES = 300,
	CODE_301_MOVED_PERMANENTLY = 301,
	CODE_302_FOUND = 302,
	CODE_303_SEE_OTHER = 303,
	CODE_304_NOT_MODIFIED = 304,
	CODE_305_USE_PROXY = 305,
	CODE_306_SWITCH_PROXY = 306,
	CODE_307_TEMPORARY_REDIRECT = 307,
	CODE_308_PERMANENT_REDIRECT = 308,

	// Client error responses
	CODE_400_BAD_REQUEST = 400,
	CODE_401_UNAUTHORIZED = 401,
	CODE_402_PAYMENT_REQUIRED = 402,
	CODE_403_FORBIDDEN = 403,
	CODE_404_NOT_FOUND = 404,
	CODE_405_METHOD_NOT_ALLOWED = 405,
	CODE_406_NOT_ACCEPTABLE = 406,
	CODE_407_PROXY_AUTHENTICATION_REQUIRED = 407,
	CODE_408_REQUEST_TIMEOUT = 408,
	CODE_409_CONFLICT = 409,
	CODE_410_GONE = 410,
	CODE_411_LENGTH_REQUIRED = 411,
	CODE_412_PRECONDITION_FAILED = 412,
	CODE_413_PAYLOAD_TOO_LARGE = 413,
	CODE_414_URI_TOO_LONG = 414,
	CODE_415_UNSUPPORTED_MEDIA_TYPE = 415,
	CODE_416_RANGE_NOT_SATISFIABLE = 416,
	CODE_417_EXPECTATION_FAILED = 417,
	CODE_418_IM_A_TEAPOT = 418, // HEY YO WHAT THE FUCK ?
	CODE_421_MISDIRECTED_REQUEST = 421,
	CODE_422_UNPROCESSABLE_ENTITY = 422,
	CODE_423_LOCKED = 423,
	CODE_424_FAILED_DEPENDENCY = 424,
	CODE_425_TOO_EARLY = 425,
	CODE_426_UPGRADE_REQUIRED = 426,
	CODE_428_PRECONDITION_REQUIRED = 428,
	CODE_429_TOO_MANY_REQUESTS = 429,
	CODE_431_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
	CODE_451_UNAVAILABLE_FOR_LEGAL_REASONS = 451,

	// Server error responses
	CODE_500_INTERNAL_SERVER_ERROR = 500,
	CODE_501_NOT_IMPLEMENTED = 501,
	CODE_502_BAD_GATEWAY = 502,
	CODE_503_SERVICE_UNAVAILABLE = 503,
	CODE_504_GATEWAY_TIMEOUT = 504,
	CODE_505_HTTP_VERSION_NOT_SUPPORTED = 505,
	CODE_506_VARIANT_ALSO_NEGOTIATES = 506,
	CODE_507_INSUFFICIENT_STORAGE = 507,
	CODE_508_LOOP_DETECTED = 508,
	CODE_510_NOT_EXTENDED = 510,
	CODE_511_NETWORK_AUTHENTICATION_REQUIRED = 511

} e_errorCodes;

class errorCode
{

public:

	errorCode( void );
	~errorCode();

	const string& getCode( e_errorCodes );

	const string getCorrespondingString(e_errorCodes)const;

private:

	map<e_errorCodes, const string> _mapErrorCode;
};

errorCode::errorCode( void ){

	for (size_t code = CODE_100_CONTINUE; code <= CODE_511_NETWORK_AUTHENTICATION_REQUIRED; ++code)
	{
		_mapErrorCode.insert(std::make_pair(static_cast<e_errorCodes>(code), getCorrespondingString(static_cast<e_errorCodes>(code))));
	}
}

const string& errorCode::getCode( e_errorCodes codeInput ){

	return _mapErrorCode.at(codeInput);
}


const string errorCode::getCorrespondingString(e_errorCodes inputCode) const {
    
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



errorCode::~errorCode( void ){}

int main(void){

	errorCode code;

	std::cout << code.getCode(CODE_202_ACCEPTED) << std::endl;
}