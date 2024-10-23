#include "Execution.hpp"

// -------------------- COPLIAN FORM -------------------- 
Execution::Execution( void ) :
	_client(NULL)
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

void	Execution::initialChecks( Client& inputClient, Config& config ){

	// ! STEP 1 = Does the Asked Ressources Exists ?
	string curPath;
	bool readRights;
	// CASE 1 : The URI == "/"
	if (_client->header.getURI().empty())
	{
		// CODE_404_NOT_FOUND
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
			// CODE_404_NOT_FOUND
		}
		else // ressource found
		{
			readRights = (_fileInfo.st_mode & S_IRUSR) || (_fileInfo.st_mode & S_IRGRP) || (_fileInfo.st_mode & S_IROTH);
			if (not readRights)
			{
				// CODE_401_UNAUTHORIZED
			}
			else // reading rights, write in stream
				_isCheckingDone = true;
		}
	}
	else if ( /*some other paths*/ )
	{

	}

}


void Execution::callGet( Client& inputClient, Config& config ){

	// Init client
	_client = &inputClient;

	string uri = _client->header.getURI();

	initialChecks( inputClient, config );

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}
	

	

	// ! STEP 2 = Does the Asked Ressource can be read (or executable if script)

	// ! STEP 3 = Is URI a CGI ??

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
