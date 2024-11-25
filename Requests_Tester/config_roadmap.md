# ============= BASIC CHECKS =============

## GET Method
- Single GET ✅ => config_001.ini

## POST Method
- Single POST ✅ => config_002.ini

## DELETE Method
- Single DELETE ✅ => config_003.ini

## UNKNOWN Method
- Shitty Method ✅ => config_004.ini

## UPLOAD
- Single UPLOAD ✅ => config_005.ini

# ============= CONFIGURATION =============
- Setup Multiple Servers with Differents Ports ❌ =>
- Setup Multiple Servers with Differents Hostnames ❌ =>
- Setup Standart Error Pages ✅ => config_006.ini
- Setup MISSING(s) Error Pages ✅ => config_007.ini
- Limit the client body ✅ => config_008.ini
- Setup routes in a server to different directories ✅ => config_009.ini
- Setup a default file index ✅ => config_010.ini
- Setup a list of methods accepted ✅ => config_011.ini

# ============= PORT ISSUES =============

- Setup Multiples Ports and use differents websites ✅ => config_012.ini

# ============= CHECK WITH A BROWSER =============
- Setup the Same Port Multiple Time ✅ => config_013.ini
-  Listing Directory Case 1 ✅ => config_014.ini
-  Listing Directory Case 2 ✅ => config_015.ini
-  Redirect URL CASE 1 ✅ => config_017.ini
-  Redirect URL CASE 2 ✅ => config_018.ini
-  Redirect URL CASE 3 ✅ => config_019.ini
-  Redirect URL CASE 4 ✅ => config_020.ini
-  Redirect URL CASE 5 ✅ => config_021.ini

# ============= CHECK CGI =============

- The server is working fine with a CGI ✅ => config_022.ini
- CGI Not Allowed, or incorrect formated ✅ => config_023.ini
- CGIs for relative Path file access ✅ => config_024.ini
- Infinite Loop or Error CGIs ✅ => config_025.ini

# ============= BONUS PARTS =============
- Coockies and session ❌ => config_???.ini



## ROUTE CONFIGS :
allowedMethods = 
redirection =
root = 
listingDirectory    =
index =
cgiAllowed =
uploadAllowed =
uploadDirectory =

## EDGE CASES :
- Missing errorPages
- Small amount of clients allowed
- Small Body Size
- No route given to server

