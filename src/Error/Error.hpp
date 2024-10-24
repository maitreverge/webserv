#pragma once

class Error
{
	private:

	public:
		Error();
		~Error();
		static Error& getInstance();
		void	handleError() const;

};