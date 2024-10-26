# EXECUTION ROADMAP :

## BUILDING HEADER
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
	==> Definir une redirection HTTP ⛔ (possibly send a CODE_300 and let the client sent back another request)
	==> DEFINE AUTHORIZATION DEPENDING ON THE METHOD  ⛔
		# GET Request:
			* Read Permission: The server needs to read the file or directory contents.
			* POSIX Permission: R_OK (Read permission)
		# POST Request:
			* Write Permission: The server needs to write data to the file or directory.
			* POSIX Permission: W_OK (Write permission)
			* Execute Permission: If the target is a directory, the server needs execute permission to access the directory.
			* POSIX Permission: X_OK (Execute permission)
		# DELETE Request:
			* Write Permission: The server needs to delete the file or directory.
			* POSIX Permission: W_OK (Write permission)
			* Execute Permission: If the target is a directory, the server needs execute permission to access the directory.
			* POSIX Permission: X_OK (Execute permission)


- Executing the CGI ⛔
	==> Possibly extract timestamp from Last-Modified CGI code source for avoiding generating twice the same CGI.html ( depends if the CGI.py does outputs the same thing or not, will depend mosty on PATH_INFO and body, if one or another are exploited)


## BUILDING BODY



✅

⛔

## HOTFIX :

Retour de page IMAGES foireux