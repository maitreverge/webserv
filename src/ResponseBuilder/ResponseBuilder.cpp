#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder( void ){}


ResponseBuilder::ResponseBuilder( const ResponseBuilder& copy ){ static_cast<void>(copy); }


ResponseBuilder& ResponseBuilder::operator=( const ResponseBuilder& right_operator ){

	static_cast<void>(right_operator);
	return *this;
}


ResponseBuilder::~ResponseBuilder( void ){}

vector<char>	ResponseBuilder::getHeader( Client &, Config& ){

	
}
void			ResponseBuilder::getBody( Client &, Config& ){

	
}