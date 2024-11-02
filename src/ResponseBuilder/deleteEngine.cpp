#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::deleteEngine( void ){

	string targetToDelete = _realURI;

    pid_t pid = fork();

    if (pid == -1) {
        // Fork	 failed
		setError(CODE_500_INTERNAL_SERVER_ERROR);
        return;
    }

    if (pid == 0)
	{
        // Child process
        const char *rm = "/bin/rm";
        const char *args[] = {rm, targetToDelete.c_str(), NULL};
        execve(rm, (char *const *)args, NULL);
        // If execve returns, there was an error
		setError(CODE_500_INTERNAL_SERVER_ERROR);
    }
	else
	{
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1)
			setError(CODE_500_INTERNAL_SERVER_ERROR);
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			// log here
		}
		else
			setError(CODE_500_INTERNAL_SERVER_ERROR);
    }
}