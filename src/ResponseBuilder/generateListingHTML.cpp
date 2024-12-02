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

bool ResponseBuilder::isFileIgnored( string &str ){

	if (	str == "." or
			str == ".." or
			str == ".git" or
			str == ".gitignore" or
			str == "webserv" or
			str == "debug_webserv" or
			str == ".gitmodules")
	{
		return true;
	}
	return false;
}

void ResponseBuilder::makeHeaderListing(std::stringstream &result) {
	
	stringstream documentTitle;

	documentTitle << "Listing Directory of URL "
				  << _realURI;

	// Build Head
	result << "<!DOCTYPE html>\n"
		   << "<html lang=\"en\">\n"
		   << "<head>\n"
		   << "<meta charset=\"UTF-8\">\n"
		   << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		   << "<title>"
		   << documentTitle.str()
		   << "</title>\n<style>"
		   // ! ADD ICONS STYLING
		   << "ul li a[href$=\"/\"]::before {\n"
		   << "\tcontent: \"\\1F4C1\";\n"
		   << "\tmargin-right: 8px;\n"
		   << "}\n"
		   << "ul li a:not([href$=\"/\"])::before {\n"
		   << "\tcontent: \"\\1F4C4\";\n"
		   << "\tmargin-right: 8px;\n"
		   << "}\n"
		   << "ul li a {\n"
		   << "\ttext-decoration: none;\n"
		   << "\tcolor: inherit;\n"
		   << "}\n"
		   << "ul li a span {\n"
		   << "\ttext-decoration: underline;\n"
		   << "\tcolor: blue;\n"
		   << "}\n";
	// ! ADD ICONS STYLING

	result << "</style>\n</head>\n<body>\n<ul>";
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

	if (*_realURI.rbegin() != '/')
		_realURI += '/';
	
	string path;

	path = getcwd(cwd, sizeof(cwd));

	if (*path.rbegin() != '/')
		path += "/";
	
	if (_realURI != "/")
		path += _realURI;

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

	makeHeaderListing(result);

	// Build Body for each entry
	vector< string > paths;

	// Appending every path to a vector
    while ((listing = readdir(dir)) != NULL)
	{
		string curFile = listing->d_name;
		if (!isFileIgnored(curFile))
		{
			if (listing->d_type == DT_DIR)
				curFile += "/"; // Append trailing slash for directories
			if ( !_myconfig.root.empty() and curFile.find(_myconfig.root) == std::string::npos)
			{
				curFile.insert(0, _myconfig.root);
			}
			
			paths.push_back(curFile);
		}
	}

	// Alphabetically sorting paths
	std::sort(paths.begin(), paths.end());

	// Build every tag for every path
	for(std::vector< string >::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		// <a href="test.html">Go to Test Page</a>
		result << "<li>";
		result << "<a href=\"";
		result << *it;
		result << "\">";
		result << *it;
		// result << listing->d_name;
		result << "</a>";
		result << "</li>\n";
	}
	
	// End on the body and the file
	result	<< "</ul>"
			<< "</body>\n"
			<< "</html>\n";
	

	string listingName = "listing.html";

	string defautFile = path;

	if (*defautFile.rbegin() != '/')
		defautFile += "/";

	defautFile += listingName;

	ofstream listingFile(defautFile.c_str());

	listingFile << result.str();

	_realURI += listingName;

	// Comment this to make listing.html
	_deleteURI = true;

	if (*_realURI.begin() == '/')
		_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")

	closedir(dir);

	listingFile.close();
}

void ResponseBuilder::generateListingHTML( void ){

	// ! STEP 0 : check if we can read in the directory
	if ( _myconfig.listingDirectory == false and _myconfig.index.empty())
	{
		Logger::getInstance().log(ERROR, "setError404 on `generateListingHTML`");
		setError(CODE_404_NOT_FOUND);
	}
	
	listingHTMLBuilder();
	
	checkNature();
	
	checkAutho();
}
