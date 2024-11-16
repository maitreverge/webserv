#include "ResponseBuilder.hpp"
#include "Logger.hpp"
#include <stdlib.h>

void	ResponseBuilder::errorNotFoundGenerator( void ){

	// codes struct for getting the appropriate error Message.
	errorCode codes;
	
	stringstream result;
	stringstream body;
	stringstream image;

	string imageURL = "https://freeyork.org/wp-content/uploads/2015/06/DKOpAT4VAAEmmes.jpg";
	
	string errorName = codes.getCode(_errorType);

	u_int16_t errorNumber = static_cast<u_int16_t>(_errorType);

	// Body text
	body	<< "The "
			<< errorNumber
			<< ".html file has not been found."
			<< "This is a backup self-generated webpage.";
	
	// Image source because life is fun
	image   << "<img src=\""
			<< imageURL
			<< "\" alt=\"Description of image\" style=\"width:50%;\">";

	// Assemble all parts
	result	<< "<!DOCTYPE html>"
			<< "<html>"
			<< "<head>"
			<< "<title>"
			<< errorNumber
			<< "\t"
			<< errorName
			<< "</title>"
			<< "</head>"
			<< "<body>"
			<< "<h1>"
			<< body.str()
			<< "</h1>"
			<< "<h1>"
			<< image.str()
			<< "</h1>"
			<< "</body>"
			<< "</html>";

	ofstream backupStream(_backupNameFile.c_str());

	backupStream << result.str();
	
	// ! Modify the _realURI
	this->_realURI = _backupNameFile;
	this->_errorNotFound = true;
	
	backupStream.close();
}

/*

<!DOCTYPE html>
<html>
<head>
    <title>Basic HTML Page</title>
</head>
<body>
    <h1>Hello, World!</h1>
</body>
</html>


*/
