#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void sanitizeURI(string &oldURI) {

	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != -1){

		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}

int main(void)
{
	string URI = "../../../hello.py/../../../test/../../hcoucou../";

	sanitizeURI(URI);

	cout << URI << endl;


}