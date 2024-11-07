#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

bool ResponseBuilder::foundDefaultPath( void ){

	if (_myconfig.index.empty())
		return false;
	
	string saveURI = _realURI;

	saveURI += _myconfig.index;

	if (stat(_realURI.c_str(), &_fileInfo) == 0) // found default path
	{
		// check if the index.html is a file AND can be read
		if ( (_fileInfo.st_mode & S_IFMT) == S_IFREG and ( _fileInfo.st_mode & S_IRUSR) )
		{
			_realURI = saveURI;
			extractFileNature(_realURI);
			return true;
		}
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
	char cwd[1024];

	string path;

	path = getcwd(cwd, sizeof(cwd)) + _originalURI;

	// ! STEP 1 : Opens the directory
	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
	{
		Logger::getInstance().log(ERROR, "Failing Openning Directory");
		setError(CODE_500_INTERNAL_SERVER_ERROR);
		return;
	}

	struct dirent *listing;

	stringstream result;

	stringstream documentTitle;
	
	documentTitle	<< "Listing Directory of URL "
					<< _realURI;

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

	result	<< "<body>\n<ul>";

	// Build Body for each entry
    while ((listing = readdir(dir)) != NULL)
	{
		string curFile = listing->d_name;
		string curPath = _originalURI + listing->d_name;
		if (curFile != "." and curFile != "..") // do not list either "." or ".."
		{
			// <a href="test.html">Go to Test Page</a>
			result << "<li>";
			result << "<a href=\"";
			result << curPath;
			result << "\">";
			result << listing->d_name;
			result << "</a>";
			result << "</li>\n";
		}
	}
	
	// End on the body and the file
	result	<< "</ul>"
			<< "</body>\n"
			<< "</html>\n";
	

	// TODO : Get from config the default file name for listing directories

	// if (*_realURI.begin() == '/')
	// 	_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")

	string listingName = "listing.html";

	string defautFile = path + listingName;

	ofstream listingFile(defautFile.c_str());

	
	

	listingFile << result.str();

	_realURI = _originalURI + listingName;


	closedir(dir);

	listingFile.close();
	// test
	// localhost:1510/testResponseBuilder/listingDirectory
}

void ResponseBuilder::generateListingHTML( void ){

	// ! STEP 0 : check if we can read in the directory
	if (not _myconfig.listingDirectory and _myconfig.index.empty())
		setError(CODE_401_UNAUTHORIZED);
	
	// if (foundDefaultPath())
	// {
	// }
	listingHTMLBuilder();
	checkNature();
	checkAutho();

	// if (_myconfig.index.empty())
	
	// if (not _myconfig.listingDirectory or not _isWOK or _isROK)
	// {
	// }
	// 	return;


}
