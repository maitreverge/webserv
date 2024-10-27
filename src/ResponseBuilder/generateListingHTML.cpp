#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

bool ResponseBuilder::foundDefaultPath( void ){

	string saveURI = _realURI;

	vector<string>::iterator it;

	{
		// TODO : If default path does not exists => 403 forbiden
	}
	// Looks accross all default files
	// ! Can possibly be another map in the config file !
	for (it = _config->indexFiles.begin(); it < _config->indexFiles.end(); ++it)
	{
		saveURI += *it;	
		
		if (stat(_realURI.c_str(), &_fileInfo) == 0)
			break ;
		else
			saveURI.erase(saveURI.find_last_of("/") + 1, (*it).size()); // Clean the appended string from the last "/"
	}

	if (it == _config->indexFiles.end())
		return false;
	else if ( (_fileInfo.st_mode & S_IFMT) == S_IFREG and ( _fileInfo.st_mode & S_IRUSR) ) // check if the index.html is a file AND can be read
	{
		_realURI = saveURI;
		return true;
	}
	return false;
}

void	ResponseBuilder::listingHTMLBuilder( void ){

	// TODO  EDGE CASE TO HANDLE :  what is we can't write in the directory

	/*
		! NOTE FOR EDGE CASE
		Either I make a timeStamp checker + a authorization checker to avoiding generating the same file over and over
		OR
		I just check the authorizations within the 
	*/
	// ! STEP 1 : Opens the directory
	DIR *dir = opendir(_realURI.c_str());
	if (dir == NULL)
	{
		Logger::getInstance().log(ERROR, "Failing Openning Directory");
		setError(CODE_500_INTERNAL_SERVER_ERROR);
		return;
	}

	struct dirent *listing;

	stringstream result;

	stringstream documentTitle;
	
	documentTitle	<< "<h1 style=\"color: #ff5733; font-family: Arial, sans-serif;\">"
					<< "Listing Directory of URL "
					<< _realURI
					<< " </h1>";

	// Build Head							
	result	<< "<!DOCTYPE html>\n"
			<< "<html lang=\"en\">\n"
			<< "<head>\n"
			<< "<meta charset=\"UTF-8\">\n"
			<< "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
			<< "<title>"
			<< documentTitle.str()
			<< "</title>\n"
			<< "</head>\n";

	result	<< "<body>\n";

	// Build Body for each entry
    while ((listing = readdir(dir)) != NULL)
	{
		string curFile = listing->d_name;
		string curPath = _realURI + "/" + listing->d_name;
		if (curFile != "." and curFile != "..") // do not list either "." or ".."
		{
			// <a href="test.html">Go to Test Page</a>
			result << "<h1>";
			result << "<a href=\"";
			result << curPath;
			result << "\">";
			result << listing->d_name;
			result << "</a>\n";
			result << "</h1>\n";
		}
	}
	
	// End on the body and the file
	result	<< "</body>\n"
			<< "</html>\n";
	

	// TODO : Get from config the default file name for listing directories

	string defautFile = _realURI + "/index.html";
	ofstream listingFile(defautFile.c_str());

	listingFile << result.str();

	_realURI = defautFile;

	closedir(dir);

	listingFile.close();
	// test
	// localhost:1510/testResponseBuilder/listingDirectory
}

void ResponseBuilder::generateListingHTML( void ){

	// ! STEP 0 : check if we can read in the directory
	if ( not _isROK )
	{
		setError(CODE_401_UNAUTHORIZED);
		return;
	}
	// ! STEP 1 : Checks if the Directory has been touched since, and if there is a default path
	else if (foundDefaultPath())
		return;

	// ! STEP 2 : Generate an index.html for the current page	
	if (_config->listingDirectories)
		listingHTMLBuilder();
	else
		setError(CODE_401_UNAUTHORIZED);

	// TODO : Update URI depending on the default file location
}
