#include "buildResponse.hpp"

// TODO : Decide with the execution part on how to encapsulate data input
vector<char> masterBuilder( vector<char> &bodyInput, e_errorCodes errorCode, string& fileName ){

	/*
		Selon le code d'erreur recu par masterBuilder, cette fonction doit se charger
		de faire un ifstream pour recuperer les pages HTML d'erreur correspondantes
	*/

	/*
		Pseudo code :
		if (error_code 400 ou 500 + pas de body)
			=> chercher les pages HTML d'erreur correspondantes
		if pas de code d'erreur
			=> CODE_200
		(potnetiellement voir si un code d'erreur nest pas fourni, mettre un code par default)
	*/
	// Builder class
	buildResponse builder;
	
	// ! STEP 1 : Make BODY
	builder.buildBody(bodyInput);
	
	// ! STEP 2 : Make HEADERS
	builder.buildHeaders( errorCode, fileName );

	// ! STEP 3 : Make EXTRAS-HEADERS (session ID, cookies, and non-mandatory headers)
	// builder.buildExtrasHeaders();

	builder.assembleResponse();

	// printColor(BOLD_GREEN, builder.getBodyLenght());
	// printColor(BOLD_CYAN, builder.getContentLenght());

	// print(builder);

	return builder.getHttpResponse();
}