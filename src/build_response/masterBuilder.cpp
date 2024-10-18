#include "buildResponse.hpp"

vector<char> masterBuilder( vector<char> &bodyInput, e_errorCodes errorCode, string& fileName ){

	// Builder class
	buildResponse builder;
	
	// ! STEP 1 : Make BODY
	builder.buildBody(bodyInput);
	
	// ! STEP 2 : Make HEADERS
	builder.buildHeaders( errorCode, fileName );

	// ! STEP 3 : Make EXTRAS-HEADERS (session ID, cookies, and non-mandatory headers)
	// builder.buildExtrasHeaders();

	builder.assembleResponse();

	return builder.getHttpResponse();
}