/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorCode.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:10:07 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:12:00 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"

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
	CODE_242_CONNECTION = 242,

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

	const string &getCode( const e_errorCodes& )const;

private:

	map<e_errorCodes, const string> _mapErrorCode;
	
	const string getCorrespondingString(e_errorCodes)const;

};