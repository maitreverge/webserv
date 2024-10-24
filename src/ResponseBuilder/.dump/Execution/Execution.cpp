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

void Execution::sanatizeURI( string &oldURI ){

	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != -1)
	{
		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}


void Execution::resolveURI( Client& inputClient, Config& config )
{
	// /images/../config/../../logo.png

	_realURI = _client->header.getURI();

	// ! STEP 1 : Trim all "../" from the URI for transversal path attacks
	sanatizeURI(_realURI);

	// TODO STEP 2 : Resolve URI with rooted path from config file
}

void	Execution::initialChecks( Client& inputClient, Config& config ){

	// ! STEP 1 = RESSOURCE EXISTS AND READEABLE ?
	// string curPath;

	// CASE 1 : The URI == "/"
	if (_realURI.empty())
		_errorType = CODE_404_NOT_FOUND;
	else if (_realURI == "/") // What if the resolved URI is a directory and not just "/"
	{
		vector<string>::iterator it;
		for (it = config.indexFiles.begin(); it < config.indexFiles.end(); ++it)
		{
			_realURI += *it;
			
			// Check permissions with stat
			if (stat(_realURI.c_str(), &_fileInfo) == 0)
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
			bool readRights = (_fileInfo.st_mode & S_IRUSR) || (_fileInfo.st_mode & S_IRGRP);
			if (not readRights)
				_errorType = CODE_401_UNAUTHORIZED;
		}
	}
	else // check classic path
	{
		if (stat(_realURI.c_str(), &_fileInfo) == -1) // if given path fails
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
			if (_fileInfo.st_mode & S_IFMT == S_IFDIR) // checks is the given path is a directory
				_isDirectory = true;
		}
	}

	// Implementer un booleen qui signifie que une erreur donnee 

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}

	// TODO = Is URI a CGI ??
	{

	}


	_isCheckingDone = true;
}

void Execution::launchCGI( void ){

	// TODO
}

void Execution::writeStream() {

	if (not _client->isBodyStreamOpen) // reading body for the first time 
	{
		// Openning stream for the new incomming body
		_client->bodyStream.open(_realURI, std::ios::binary); // openning stream in binary mode
		if (not _client->bodyStream.is_open())
		{
			// Handle error with a code 500 error
		}
		else
			_client->isBodyStreamOpen = true;
		_client->bodyStream.seekg(_client->filePosition); // Put the reading head at the start of the strea
	}
	else // no
	{
	
	}
}

void Execution::callGet( Client& inputClient, Config& config ){

	if (not _isCheckingDone)
	{
		_client = &inputClient; // init client
		resolveURI(inputClient, config);
		initialChecks( inputClient, config );
	
		if (_errorType >= 400)
		{
			// Remplace URI with the 404.html path from config files
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
		else if (_isCGI)
		{
			launchCGI(); // CGI must write within a file
		}
	}


	writeStream();
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
