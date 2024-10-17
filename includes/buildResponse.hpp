#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"

class buildReponse
{

private:

	vector<char> _httpResponse;

protected:

	// Some protected members

public:

	buildReponse( void );
	buildReponse( const buildReponse& copy );
	buildReponse& operator=( const buildReponse& right_operator );
	~buildReponse();

	// Getters
	const vector<char> &getHttpResponse( void )const;
	// Setters
	void setHttpResponse( vector<char> input /* could be string or char*   */ );

};

ostream& operator<<( ostream& output_stream, const buildReponse& input );


// ---------- Functions declarations ---------------


buildReponse::buildReponse( void ){

	// TODO : List init some values ?
}


buildReponse::buildReponse( const buildReponse& copy ) :
	_httpResponse(copy._httpResponse){}


buildReponse& buildReponse::operator=( const buildReponse& right_operator ){

	if (this != &right_operator){
		// Reassign every value with the getter value 
		this->_httpResponse = right_operator.getHttpResponse()
	}
	return *this;
}


buildReponse::~buildReponse( void ){

	// TODO : clean the httpResponse on each run ??
}

const vector<char>& buildReponse::getHttpResponse( void )const{ return _httpResponse; }


// ! could be string or char*
void buildReponse::setHttpResponse( vector<char> input){

	/*
	TODO : concatenate to the vector on differents ways depending if it's a string,
	a vector or a `char*`
	
	*/
}



ostream& operator<<( ostream& output_stream, const buildReponse& right_input ){

	output_stream << right_input.getHttpResponse();

	return output_stream;
}
