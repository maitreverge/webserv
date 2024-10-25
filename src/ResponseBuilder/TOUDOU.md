EXECUTION ROADMAP :

- Extract Method (GET, POST or DELETE) ✅
- Get URI ✅
- Resolve URI
	==> Rooted path from config file ⛔
	==> transversal attacks "../" (can go further with analyzing if "../" does exit root or not) ✅

- Validate URI
	==> Is the URI accessible  ✅
	==> Is the path a directory  ✅
	==> Is the path a CGI  ⛔
	==> Does the route accepts the current method  ⛔
	==> Definir une redirection HTTP ⛔⛔⛔⛔⛔⛔
	==> DEFINE AUTHORIZATION DEPENDING ON THE METHOD  ⛔
	(GET Request:
	Read Permission: The server needs to read the file or directory contents.
	POSIX Permission: R_OK (Read permission)
	POST Request:
	Write Permission: The server needs to write data to the file or directory.
	POSIX Permission: W_OK (Write permission)
	Execute Permission: If the target is a directory, the server needs execute permission to access the directory.
	POSIX Permission: X_OK (Execute permission)
	DELETE Request:
	Write Permission: The server needs to delete the file or directory.
	POSIX Permission: W_OK (Write permission)
	Execute Permission: If the target is a directory, the server needs execute permission to access the directory.
	POSIX Permission: X_OK (Execute permission))






✅

⛔