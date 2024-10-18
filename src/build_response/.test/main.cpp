#include "../../../includes/buildResponse.hpp"
// #include "errorCode.hpp"

vector<char> masterBuilder( const vector<char> &bodyInput, const e_errorCodes &errorCode, const string& fileName ){

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

// using namespace std;

#include "../../../includes/utils_templates.hpp"
int main(int ac, char**av){

	if (ac != 2)
	{
		cout << "ERROR, program needs 2 arguments" << endl;
		exit(1);
	}

	ifstream htmlFile(av[1]);

	string stringBuffer;

	stringstream streamBuffer;

	while(getline(htmlFile, stringBuffer)){

		streamBuffer << stringBuffer;
	}

	cout << "raw file =" << endl;
	cout << streamBuffer.str() << endl;

	vector<char> htmlVector(streamBuffer.str().begin(), streamBuffer.str().end());

	vector<char> answer = masterBuilder(htmlVector, CODE_200_OK, "index.html");


	cout << "Answer =" << endl;

	string coucou(answer.begin(), answer.end());
	cout << coucou << endl;

}