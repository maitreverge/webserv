## POST REQUEST EXAMPLE

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Multiple File Upload</title>
</head>
<body>
	<!-- ! Two files can be uploaded within the same request, fuck my life -->
    <h1>Upload Multiple Files</h1>
    <form id="uploadForm" action="/uploads/" method="POST" enctype="multipart/form-data">
        <input type="file" name="file1" id="fileInput1" required>
        <input type="file" name="file2" id="fileInput2" required>
        <button type="submit">Submit</button>
    </form>
</body>
</html>
```
=================================================================================

## EXAMPLE FROM GPT

POST /uploads/ HTTP/1.1\r\n
Host: example.com\r\n
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n
Content-Length: 138\r\n
\r\n
------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n
Content-Disposition: form-data; name="file1"; filename="example1.jpg"\r\n
Content-Type: image/jpeg\r\n
\r\n
<BINARY_DATA_OF_THE_FIRST_IMAGE>\r\n
------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n
Content-Disposition: form-data; name="file2"; filename="example2.jpg"\r\n
Content-Type: image/jpeg\r\n
\r\n
<BINARY_DATA_OF_THE_SECOND_IMAGE>\r\n
------WebKitFormBoundary7MA4YWxkTrZu0gW--\r\n


## WHAT'S IN A REAL FILE :

POST /uploads/ HTTP/1.1
Host: localhost:1510
Connection: keep-alive
Content-Length: 1585159
Cache-Control: max-age=0
sec-ch-ua: "Chromium";v="130", "Google Chrome";v="130", "Not?A_Brand";v="99"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"
Origin: http://localhost:1510
DNT: 1
Upgrade-Insecure-Requests: 1
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary1XN99skGpOHP8Og8
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: http://localhost:1510/testResponseBuilder/upload/uploadEntryPoint.html
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: en-US,en;q=0.9 \r\n
\r\n
------WebKitFormBoundary1XN99skGpOHP8Og8
Content-Disposition: form-data; name="file1"; filename="fr.subject.pdf" \r\n
Content-Type: application/pdf \r\n
\r\n\
<BINARY DATA>
------WebKitFormBoundary1XN99skGpOHP8Og8--
