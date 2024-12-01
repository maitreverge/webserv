## CURRENT WORK
- Config 001 🟢
- Config 002 🟠
	=> Need from Kernel : Seb needs to throw a CODE_400 on empty POST body (config 002, test 13 and test 14)
	=> Need a CGI for treating test 23 and 24
- Config 003 🟢
- Config 004 🟢
- Config 005 🔴

## ASK SEB / DAN
- `setBody`, line `client.messageRecv.clear();` => See with Seb if this line is correct 🔴

## HOTFIX :
- Finish the multipart-form data POST logic 🔴


## Mandatory Work but can wait
- Delete duplicates nodes with the same URI in `extractRouteConfig` 🔴

- Clear nodes without the URI in `extractRouteConfig` 🔴

- Clear useless variables 🔴

- Reactivate TimeStamps for final project 🔴



# ULTRA BONUS AND EDGE CASE
- Delete webserv => 401 + kill the client 🔴

- Generate `listing.html` on the go instead of writting files to the disk. 🟠

- transversal attacks "../" (can go further with analyzing if "../" does exit root or not) 🔴


## DONE STUFF
- If there is `Content-Type: multipart/form-data;`, then activate the unfinished branch 🟢
- Exploit the Content-Type header for post requests 🟢
IF NOT : Need to generate a random 10 lenghts string + an extension to save the file 🟢
- Stabilize simple GET / DELETE REQUESTS 🟢

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
