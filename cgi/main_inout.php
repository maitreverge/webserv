<?php
// Lire l'entrée standard en morceaux de 20 caractères
while ($input = fread(STDIN, 20)) {
    // Écrire l'entrée standard vers la sortie d'erreur
    fwrite(STDERR, $input);
}

// Corps de la réponse HTML
$body = "<html>
<head><title>My Styled Page</title></head>
<body style=\"background-color: #f0f0f0; text-align: center; padding: 50px;\">
<h1 style=\"color: #ff5733; font-family: Arial, sans-serif;\">Hello, World!</h1>
<p style=\"color: #555; font-size: 18px;\">This is a simple page with inline CSS.</p>
</body>
</html>";

// En-têtes HTTP
$header = "HTTP/1.1 200 OK\r\n";
$header .= "Content-Type: text/html\r\n";
$header .= "Content-Length: " . strlen($body) . "\r\n";
$header .= "\r\n";

// Envoyer les en-têtes et le corps de la réponse
echo $header . $body;

exit(0);
?>
