#include "ResponseBuilder.hpp"
#include "Logger.hpp"
#include <stdlib.h>

void	ResponseBuilder::errorNotFoundGenerator( void ){

	// codes struct for getting the appropriate error Message.
	errorCode codes;
	
	stringstream result;
	stringstream body;
	stringstream image;
	
	string errorName = codes.getCode(_errorType);

	u_int8_t errorNumber = static_cast<u_int8_t>(_errorType);

	body	<< "The "
			<< errorNumber
			<< ".html has not been found."
			<< "This is a backup auto-generated solution.";

	result	<< "<!DOCTYPE html>"
			<< "<html>"
			<< "<head>"
			<< "<title>"
			<< errorNumber
			<< errorName
			<< "</title>"
			<< "</head>"
			<< "<body>"
			<< "<h1>"
			<< body.str()
			<< "</h1>"
			<< "<h1>"
			<< " hehhehehehehhehehehehehehhehehe"
			<< "</h1>"
			<< "</body>"
			<< "</html>";


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
