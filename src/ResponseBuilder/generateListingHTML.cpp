#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

bool ResponseBuilder::foundDefaultPath( void ){

	string saveURI = _realURI;

	vector<string>::iterator it;

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
}

void ResponseBuilder::generateListingHTML( void ){

	// ! STEP 0 : check if we can read in the directory
	if ( not _isROK )
	{
		setError(CODE_403_FORBIDDEN);
		return;
	}
	// ! STEP 1 : Checks if the Directory has been touched since, and if there is a default path
	else if (foundDefaultPath())
		return;

	// ! STEP 2 : Generate an index.html for the current page	
	listingHTMLBuilder();


	// ! STEP 3 : Sauvegarder un index.html dans le dossier cible (verifier si les droits autho sont OK dans le dossier)
	if (not _isWOK)
	{
		// If we can't write a default file in the directory, we need to index it in another place
		// or eventually make it a stream straight to the body
	}

	// ! Refresh timestamps
	isDirectoryUnchanged(); 

	// TODO : Update URI depending on the default file location
}
