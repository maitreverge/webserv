#pragma once

class Error
{
	private:

	public:
		Error();
		~Error();
		static Error& getInstance();
		void	handleError(int errorCode, struct Client *client) const;

};