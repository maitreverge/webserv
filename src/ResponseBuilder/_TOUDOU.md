## CURRENT WORK

## ASK SEB / DAN

## HOTFIX :


## Mandatory Work but can wait
- Delete duplicates nodes with the same URI in `extractRouteConfig` 🔴

- Clear nodes without the URI in `extractRouteConfig` 🔴

- Clear useless variables 🟠

- Reactivate TimeStamps for final project 🔴

- Reactivate Finalenames for post uploading for final project 🔴

- Missing `HOST` headers or double => 400 BAD REQUEST 🔴

- reliker en dur le vrai fichier de config une fois fait



# ULTRA BONUS AND EDGE CASE
- Delete webserv => 401 + kill the client 🔴

- Generate `listing.html` on the go instead of writting files to the disk. 🟠

- transversal attacks "../" (can go further with analyzing if "../" does exit root or not) 🔴


## DONE STUFF
- If there is `Content-Type: multipart/form-data;`, then activate the unfinished branch 🟢
- Exploit the Content-Type header for post requests 🟢
IF NOT : Need to generate a random 10 lenghts string + an extension to save the file 🟢
- Stabilize simple GET / DELETE REQUESTS 🟢
- Finish the multipart-form data POST logic 🟢
- Finish cookies with Dan 🟢
- curl command from the correction --resolve 🟢

## BLOODY REDIRECTIONS :

############################# This is a valid config #############################

								
								
								DO NOT FUCKING TOUCH THIS



[route1]
uri = /
redirection = _websites/cyborg_gaming/
allowedMethods = GET

# ... which matches a route
[route2]
uri = /_websites/cyborg_gaming
allowedMethods = GET
index = index.html

############################# This is a valid config #############################

🟢
🟠
🔴
