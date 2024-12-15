#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

/**
 * @brief Checks if the default index path exists and is readable.
 *
 * This function verifies if the default index path specified in the configuration
 * exists and is a regular file that can be read. If the index path is found and
 * readable, it updates the internal URI to the index path and extracts the file nature.
 *
 * @return true if the default index path is found and readable, false otherwise.
 */
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

/**
 * @brief Checks if a file should be ignored.
 *
 * This function determines whether a given file name should be ignored
 * based on a predefined list of file names.
 *
 * @param str The name of the file to check.
 * @return true if the file should be ignored, false otherwise.
 */
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

/**
 * @brief Generates the HTML header for a directory listing.
 * 
 * This function constructs the HTML header section for a directory listing page.
 * It sets the document title based on the current URL and includes styling for
 * directory and file icons.
 * 
 * @param result A stringstream to which the generated HTML header will be appended.
 */
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

	result << "</style>\n</head>\n<body>\n<ul>";
}

/**
 * @brief Generates an HTML listing of the contents of a directory.
 *
 * This function builds an HTML file that lists the contents of the directory specified by `_realURI`.
 * It appends a trailing slash to `_realURI` if it does not already end with one, and constructs the full path
 * to the directory. The directory is then opened, and its contents are read and stored in a vector.
 * The contents are sorted alphabetically and written to an HTML file named "listing.html".
 * The function also handles errors related to opening the directory and logs relevant information.
 *
 * @note The generated HTML file is saved in the directory specified by `_realURI`.
 * @note If `_realURI` begins with a '/', it is removed from `_realURI` after generating the listing.
 */
void	ResponseBuilder::listingHTMLBuilder( void ){

	char cwd[1024];

	if (*_realURI.rbegin() != '/')
		_realURI += '/';
	
	string path;

	path = getcwd(cwd, sizeof(cwd));

	if (*path.rbegin() != '/')
		path += "/";
	
	if (_realURI != "/")
		path += _realURI;

	// Opens the directory
	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
	{
		Logger::getInstance().log(ERROR, "500 Detected from `listingHTMLBuilder` : Server failed to open durrent directory with `opendir`");
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
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
			{
				curFile += "/"; // Append trailing slash for directories
			}
			if ( !_myconfig.root.empty() and curFile.find(_myconfig.root) == std::string::npos)
			{
				curFile.insert(0, _myconfig.root);
			}
			paths.push_back(curFile);
		}
	}

	closedir(dir);

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

	ofstream listingFileStream(defautFile.c_str());

	if (listingFileStream.is_open())
	{
		// Write to the stream
		listingFileStream << result.str();
		
		// Check for write errors
		if (listingFileStream.fail())
		{
			Logger::getInstance().log(ERROR, "ResponseBuilder::generateDeleteHTML : listingFileStream failed to write data");
			throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
		}
		else
			Logger::getInstance().log(DEBUG, "ResponseBuilder::generateDeleteHTML : listingFileStream correctly wrote data");
		
		_realURI += listingName;

		_deleteURI = true;
		Logger::getInstance().log(DEBUG, "ResponseBuilder::listingHTMLBuilder : The _realURI will be deleted");
		
		if (*_realURI.begin() == '/')
			_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")

		Logger::getInstance().log(DEBUG, "ResponseBuilder::generateDeleteHTML : The _realURI will be deleted");
		
	}
	else 
	{
		Logger::getInstance().log(ERROR, "ResponseBuilder::generateDeleteHTML : deleteFileStream failed to open");
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
	}

	listingFileStream.close();
}

/**
 * @brief Generates the HTML listing for a directory.
 * 
 * This function generates an HTML listing for a directory if directory listing is enabled
 * and no index file is found. If directory listing is disabled and no index file is found,
 * it logs a 404 error and sets the error code to 404 Not Found.
 * 
 * The function performs the following steps:
 * - Checks if directory listing is disabled and no index file is found, logs a 404 error, and sets the error code.
 * - Calls the listingHTMLBuilder() function to build the HTML listing.
 * - Calls the checkNature() function to perform additional checks.
 * - Calls the checkAutho() function to perform authorization checks.
 */
void ResponseBuilder::generateListingHTML( void ){

	if ( _myconfig.listingDirectory == false and _myconfig.index.empty())
	{
		Logger::getInstance().log(ERROR, "404 Detected from `generateListingHTML`");
		setError(CODE_404_NOT_FOUND);
	}
	
	listingHTMLBuilder();
	
	checkNature();
	
	checkAutho();
}
