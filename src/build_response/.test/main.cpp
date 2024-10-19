#include "../../../includes/buildResponse.hpp"
// #include "errorCode.hpp"

// using namespace std;

#include "../../../includes/utils_templates.hpp"

vector<char> masterBuilder( vector<char> &bodyInput, e_errorCodes errorCode, string& fileName );

int main(int ac, char**av){

	if (ac != 2)
	{
		cout << "ERROR, program needs 2 arguments" << endl;
		exit(1);
	}

	string arg = av[1];

	ifstream htmlFile(arg.c_str());

	string stringBuffer;

	stringstream streamBuffer;

	while(getline(htmlFile, stringBuffer)){

		streamBuffer << stringBuffer;
	}

	cout << "raw file =" << endl;
	cout << streamBuffer.str() << endl;

	string buffbuff = streamBuffer.str();

	vector<char> htmlVector(buffbuff.begin(), buffbuff.end());

	vector<char> answer = masterBuilder(htmlVector, CODE_408_REQUEST_TIMEOUT, arg);


	cout << "Answer =" << endl;

	string coucou(answer.begin(), answer.end());
	cout << coucou << endl;

	// g++ main.cpp ../buildResponse.cpp ../masterBuilder.cpp ../errorCode.cpp -I ../../../includes/ -g 

}