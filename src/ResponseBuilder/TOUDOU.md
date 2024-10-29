# EXECUTION ROADMAP :

## BUILDING HEADER
- Extract Method (GET, POST or DELETE) ✅
- Implement URI Redirection ✅
- Get URI ✅
- Resolve URI  
	==> Rooted path from config file ⛔
	==> transversal attacks "../" (can go further with analyzing if "../" does exit root or not) ✅

- Validate URI
	==> Is the URI accessible  ✅
	==> Is the path a directory  ✅
	==> Is the path a CGI  ✅
	==> Route Mapping  ⛔
	==> Does the route accepts the current method  ⛔
	==> Definir une redirection HTTP ✅
	==> DEFINE AUTHORIZATION DEPENDING ON THE METHOD  ✅
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

	==> HTML Generator for Listing Directories ✅
		#   TimeStamp checker for avoiding making twice the same work (also) ⛔

- Executing the CGI ⛔
	==> Possibly extract timestamp from Last-Modified CGI code source for avoiding generating twice the same CGI.html ( depends if the CGI.py does outputs the same thing or not, will depend mosty on PATH_INFO and body, if one or another are exploited)

- Checker a chaque fois les authos des fichiers d'erreurs. Si un 404 est corrompu/illisible ou introuvable, en generer un a la volee (ou simplement renvoyer le code au navigateur)


## BUILDING BODY
- Done by Seb, needs explanations on the stream ⛔

## REQUETES POST

- Quand je vais recevoir un BODY de POSt, il faudra verifier le content type pour svaoir vers quelle extension je vais devoir l'enregistrer sur le PATH => verifier les authorizations, et basculer sur une erreur si j'ai pas les droits d'ecriture sur le chemin (faire attention si on me demande un POST sur un fichier et pas un dossier )



## HOTFIX :

Retour de page IMAGES foireux ✅ => image_chat.jpeg was an empty file

Refactorer le timestamp path access to a higher scope, accessible to all clients (needs a generic function for CGI as well)