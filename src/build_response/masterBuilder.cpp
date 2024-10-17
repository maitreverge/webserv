#include "buildResponse.hpp"

vector<char> masterBuilder( vector<char> &bodyInput ){

	// Builder class
	buildReponse builder;
	// string headers, body, cookies;

	// ! STEP 1 : Make BODY
	builder.buildBody(bodyInput);
	
	// ! STEP 2 : Make HEADERS
	builder.buildHeaders();

	// ! STEP 3 : Make EXTRAS-HEADERS (session ID, cookies, and non-mandatory headers)
	builder.buildExtras();

	builder.assembleResponse();


	return builder.getHttpResponse();
}