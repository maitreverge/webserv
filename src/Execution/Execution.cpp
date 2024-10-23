#include "Execution.hpp"

// -------------------- COPLIAN FORM -------------------- 
Execution::Execution( void ) :
	_client(NULL),
	_isCheckingDone(false),
	_isCGI(false),
	_isDirectory(false){}


Execution::Execution( const Execution& copy ){ static_cast<void>(copy); }


Execution& Execution::operator=( const Execution& right_operator ){

	static_cast<void>(right_operator);
	return *this;
}


Execution::~Execution( void ){}


// -------------------- METHODS -------------------- 


void Execution::sanatizeURI( string oldURI ){


}

void Execution::resolveURI( Client& inputClient, Config& config )
{
	// /images/../config/../../logo.png

	// ! STEP 1 : Trim all "../" from the path
	string oldURI = _client->header.getURI();

	oldURI = sanatizeURI(oldURI);
}

void	Execution::initialChecks( Client& inputClient, Config& config ){

	// ! STEP 1 = RESSOURCE EXISTS AND READEABLE ?
	string curPath;
	bool readRights;
	// CASE 1 : The URI == "/"
	if (_client->header.getURI().empty())
	{
		_errorType = CODE_404_NOT_FOUND;
	}
	else if (_client->header.getURI() == "/")
	{
		vector<string>::iterator it;
		for (it = config.indexFiles.begin(); it < config.indexFiles.end(); ++it)
		{
			curPath = "/" + *it;
			
			// Check permissions with stat
			if (stat(curPath.c_str(), &_fileInfo) == 0)
				break ;
		}

		if (it == config.indexFiles.end())
		{
			if (errno == EACCES) // The process does not have execute permissions on one or more directories in the path.
				_errorType = CODE_401_UNAUTHORIZED;
			else if (errno == ENOENT) // The file or directory does not exist.
				_errorType = CODE_404_NOT_FOUND;
		}
		else // ressource found
		{
			readRights = (_fileInfo.st_mode & S_IRUSR) || (_fileInfo.st_mode & S_IRGRP);
			if (not readRights)
				_errorType = CODE_401_UNAUTHORIZED;
		}
	}
	else // check classic path
	{
		curPath = _client->header.getURI();
		if (stat(curPath.c_str(), &_fileInfo) == -1) // if given path fails
		{
			if (errno == EACCES)
				_errorType = CODE_401_UNAUTHORIZED;
			else if (errno == ENOENT or errno == EFAULT) // EFAULT The provided path is invalid or points to a restricted memory space.
				_errorType = CODE_404_NOT_FOUND;
		}
		else // Supposed valid path
		{
			/*
				! STAT FLAGS : https://www.man7.org/linux/man-pages/man7/inode.7.html
			*/
			if (_fileInfo.st_mode & S_IFMT == S_IFDIR)
				_isDirectory = true;
			

		}
	}


	// Implementer un booleen qui signifie que une erreur donnee 

	// ! STEP 1 = RESSOURCE EXISTS AND READEABLE ?
	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}

	// TODO = Is URI a CGI ??


	_isCheckingDone = true;
}


void Execution::callGet( Client& inputClient, Config& config ){

	// Init client
	_client = &inputClient;

	try
	{
		_realURI = resolveURI(inputClient, config);
	}
	catch(const std::exception& e)
	{
		_isCheckingDone = true;
		_errorType = CODE
	}
	

	if (not _isCheckingDone)
		initialChecks( inputClient, config );
	
	if (_errorType >= 400)
	{
		// generate error code HTML with `ERROR PROCESS`
	}
	else if (_isDirectory)
	{
		if (config.listingDirectories)
		{
			// TODO :  generate an HTML page for listing directories
		}
		else
		{
			// man idk ?
		}
	}

	

	


	if (_isCGI)
		launchCGI();
	else
	{
		// Write the body in a buffer
	} 
	


	
	// ! STEP 2 = write in a client.stream the body
	// Dereference client
	_client = NULL;
}

void Execution::callPost( Client& inputClient ){

	// Init client
	_client = &inputClient;
	// ! STEP 1 : Append POST body while
	if (_client->body)

	
	
	// ! STEP 3 = Is URI a CGI
	if (_isCGI)
		launchCGI();
	else
	{
		// Write the body in a buffer
	} 

	// Dereference client
	_client = NULL;
}

void Execution::callDelete( Client& inputClient ){

	// Init client
	_client = &inputClient;

	// print(inputClient);
	// Dereference client
	_client = NULL;

}
