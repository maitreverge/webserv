#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

bool ResponseBuilder::isDirectoryUnchanged( void ){

	// Current TimeStamps
	timespec cur_M_Time;
	timespec cur_C_Time;

	// Stored Timestamps
	timespec last_M_Time;
	timespec last_C_Time;

	// Extract data of the Directory
	stat(_realURI.c_str(), &_fileInfo);
	cur_M_Time = _fileInfo.st_ctim;
	cur_C_Time = _fileInfo.st_mtim;
	
	// Check for new entries by raising a std::out_of_bounds exception
	try
	{
		last_M_Time = _lastDir_M_Time.at(_realURI);
		last_C_Time = _lastDir_C_Time.at(_realURI);
	}
	catch(const std::exception& e)
	{
		_lastDir_M_Time.insert(std::make_pair(_realURI, cur_M_Time));
		_lastDir_C_Time.insert(std::make_pair(_realURI, cur_C_Time));
		return true;
	}

	// If any of the timestamp does not match, it means that target has been modified
	if ((cur_C_Time.tv_nsec != last_C_Time.tv_nsec) or (cur_C_Time.tv_sec != last_C_Time.tv_sec) or
		(cur_M_Time.tv_nsec != last_M_Time.tv_nsec) or (cur_M_Time.tv_sec != last_M_Time.tv_sec))
	{
		_lastDir_M_Time[_realURI] = cur_M_Time;
		_lastDir_C_Time[_realURI] = cur_C_Time;
		return false;
	}
	
	return true;
}

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
	else if ( (_fileInfo.st_mode & S_IFMT) == S_IFREG )
	{
		_realURI = saveURI;
		return true;
	}
	return false;
}

void ResponseBuilder::generateListingHTML( void ){

	// ! STEP 1 : Checks if the Directory has been touched since, and if there is a default path
	if ( isDirectoryUnchanged() and foundDefaultPath())
		return;

	// ! STEP 2 : Generate an index.html for the current page	
	// Build the HTLM generator

	// ! STEP 3 : Sauvegarder un index.html dans le dossier cible (verifier si les droits autho sont OK dans le dossier)

}
