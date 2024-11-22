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
