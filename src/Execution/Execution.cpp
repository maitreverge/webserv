#include "Execution.hpp"

// -------------------- COPLIAN FORM -------------------- 
Execution::Execution( void ) :
	_client(NULL),
	_isCGI(false),
	_isDirectory(false){}


Execution::Execution( const Execution& copy ){ static_cast<void>(copy); }


Execution& Execution::operator=( const Execution& right_operator ){

	static_cast<void>(right_operator);
	return *this;
}


Execution::~Execution( void ){}


// -------------------- METHODS -------------------- 


void Execution::callGet( Client& inputClient ){

	// Init client
	_client = &inputClient;

	string uri = _client->header.getURI();

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}
	
	// ! STEP 1 = Does the Asked Ressources Exists ?

	

	// ! STEP 2 = Does the Asked Ressource can be read (or executable if script)

	// ! STEP 3 = Is URI a CGI

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

	// Dereference client
	_client = NULL;
}

void Execution::callPost( Client& inputClient ){

	// Init client
	_client = &inputClient;

	// print(inputClient);
	// Dereference client
	_client = NULL;

}
