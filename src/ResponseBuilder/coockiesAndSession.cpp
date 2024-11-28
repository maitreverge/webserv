#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

void	ResponseBuilder::buildSetCookieHeader()
{
	stringstream streamCookie;
	streamCookie	<< "Set-Cookie:"
					<< SPACE
					<< "sessionID=test_cookie"
					<< HTTP_HEADER_SEPARATOR;
	Headers.cookie = streamCookie.str();
}