#include "timeStamp.hpp"

timeStamp::timeStamp( void ){}


timeStamp::timeStamp( const timeStamp& copy ) { static_cast<void>(copy); }


timeStamp& timeStamp::operator=( const timeStamp& right_operator ){

	static_cast<void>(right_operator);
	return *this;
}

/**
 * @brief Returns a compliant HTTP/1.1 TimeStamp.
 * 
 * Example : `Wed, 11 Oct 2024 10:24:12 GMT`
 * 
 * @return string 
 */
string timeStamp::getTime( void ){

	time_t _timestamp;

	stringstream result;

	string weekDay, month, day, hours, year, timeZone;

	timeZone = "GMT";

	// Get the current time.
	time(&_timestamp);

	stringstream buffTime;

	// Put the current time within a stringstream
	buffTime << ctime(&_timestamp);

	// Dispatch the ctime format accross the corresponding strings
	// ? ctime format = Sat Oct 19 10:31:54 2024
	buffTime >> weekDay >> month >> day >> hours >> year;

	// Rearrange time to match HTTP standarts.
	// ? ex : Wed, 11 Oct 2024 10:24:12 GMT
	result	<< weekDay
			<< ", "
			<< day
			<< SPACE
			<< month
			<< SPACE 
			<< year
			<< SPACE
			<< hours
			<< SPACE
			<< timeZone;

	// Convert the result in a string then return it.
	return result.str();
}


timeStamp::~timeStamp( void ){}