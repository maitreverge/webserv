#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
// #include <ofstream>

using namespace std;

int main(void){

	string filename = "index.html";

	string::size_type extensionIndex = filename.find_last_of(".");
	string extension = filename.substr(extensionIndex + 1);

	cout << extension << endl;

}