#include "../../includes/master.hpp"
#include "RequestParser.hpp"

int main(void){

	RequestParser parser;
	const std::vector<char> data;
	parser.parse(data);
	parser.extractHeaders();
	parser.displayAttributes();
	parser.displayHeaders();
}