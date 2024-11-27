## CURRENT WORK
- Config 001 🟢
- Config 002 🟠
- Config 003 🔴
- Config 004 🔴
- Config 005 🔴

## HOTFIX :
- Stabilize simple GET / DELETE REQUESTS 🟠
- Finish the multipart-form data POST logic 🔴
- Need from Kernel : Seb needs to throw a CODE_400 on empty POST body (config 002, test 13 and test 14)


## Mandatory Work but can wait
- Delete duplicates nodes with the same URI in `extractRouteConfig` 🔴

- Clear nodes without the URI in `extractRouteConfig` 🔴

- Clear useless variables 🔴



# ULTRA BONUS AND EDGE CASE
- Delete webserv => 401 + kill the client 🔴

- Generate `listing.html` on the go instead of writting files to the disk. 🟠

- transversal attacks "../" (can go further with analyzing if "../" does exit root or not) 🔴


## DONE STUFF
- If there is `Content-Type: multipart/form-data;`, then activate the unfinished branch 🟢
- Exploit the Content-Type header for post requests 🟢
IF NOT : Need to generate a random 10 lenghts string + an extension to save the file 🟢

🟢
🟠
🔴
