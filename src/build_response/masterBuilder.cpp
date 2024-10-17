#include "buildResponse.hpp"

vector<char> masterBuilder( vector<char> &bodyInput, u_int8_t errorCode ){

	// Builder class
	buildResponse builder;
	
	// string headers, body, cookies;

	/*
		! HEADERS and BODY DELIMITORS

	The end of the headers section and the start of the body is marked by a blank line (\r\n\r\n).
	
	Within the headers section, each header is separated by a newline (\r\n).
	*/

	// ! STEP 1 : Make BODY
	builder.buildBody(bodyInput);
	
	// ! STEP 2 : Make HEADERS
	builder.buildHeaders( errorCode );

	// ! STEP 3 : Make EXTRAS-HEADERS (session ID, cookies, and non-mandatory headers)
	builder.buildExtras();

	builder.assembleResponse();



	return builder.getHttpResponse();
}