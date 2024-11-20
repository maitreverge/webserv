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

# ============= PORT ISSUES =============

# ============= PORT ISSUES =============

# ============= CHECK WITH A BROWSER =============

# ============= BONUS PARTS =============



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

