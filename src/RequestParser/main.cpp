#include "../../includes/master.hpp"
#include "RequestParser.hpp"

int main(int ac, char **av, char **envp){

	RequestParser parser;
	const std::vector<char> data;


	parser.parse(data);
	parser.displayAttributes();

}