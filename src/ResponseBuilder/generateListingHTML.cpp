#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

bool ResponseBuilder::isTargetModified( void ){

	// Look within the map
}

bool ResponseBuilder::foundDefaultPath( void ){

	string saveURI = _realURI;

	vector<string>::iterator it;

	// Looks accross all default files
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
	else // still need to check if the path is a directory called "index.html" 
	{
		if ( (_fileInfo.st_mode & S_IFMT) == S_IFDIR )
			return false;
		else if ( (_fileInfo.st_mode & S_IFMT) == S_IFREG )
		{
			_realURI = saveURI;
			return true;
		}
	}
	return false; // if default path "index.html" is neither a regular file or a directory
}

void ResponseBuilder::generateListingHTML( void ){

	// ! STEP 4 : stocker le timestamp du dossier cicle, pour savoir si l'on regenere le index.html ou si on le garde
	// ! STEP 1 : Check if there is an index.html at the targeted directory
	if (isTargetModified() and foundDefaultPath())
		return;

	// ! STEP 2 : Generate an index.html for the current page	
	// Build the HTLM generator

	// ! STEP 3 : Sauvegarder un index.html dans le dossier cible (verifier si les droits autho sont OK dans le dossier)

}
