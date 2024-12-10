<?php
ini_set('default_mimetype', '');

// Récupérer le PATH_INFO depuis les variables d'environnement CGI
$path_info = getenv('PATH_INFO');

// Afficher le PATH_INFO pour débogage

// Vérifier que PATH_INFO contient une chaîne de requête après le "?".
if (strpos($path_info, '?') !== false) {
    // Extraire la partie après le "?".
    $query_string = substr($path_info, strpos($path_info, '?') + 1);

    // Analyser la chaîne de requête.
    parse_str($query_string, $params);

    // Extraire le login à partir des paramètres
    $login = isset($params['login']) ? htmlspecialchars($params['login']) : 'Non fourni';
} else {
    $login = 'Non fourni';
}

// Afficher le login pour débogage

$url = "https://webservcgi-default-rtdb.montcul.firebasedatabase.app/users/{$login}.json";

$response = file_get_contents($url);

// Vérifier si la requête a réussi
if ($response === FALSE) {
	die('Error occurred while fetching data');
}
// file_put_contents('response_log.txt', $response);

// Décoder la réponse JSON
$user_data = json_decode($response, true);

if (isset($user_data['login']) && $user_data['login'] === $login) {
    // Si le login correspond, extraire les autres informations
    $name = $user_data['name'];
    $anecdote = isset($user_data['anecdote']) ? $user_data['anecdote'] : 'No anecdote information';
	$favorite_color = isset($user_data['favorite_color']) ? $user_data['favorite_color'] : 'No favorite_color information';
	$age = isset($user_data['age']) ? $user_data['age'] : 'No age information';
	$surname = isset($user_data['surname']) ? $user_data['surname'] : 'No surname information';
	

    // Construire le corps de la réponse HTML avec les informations récupérées
    $body = "<html>
    <head>
        <title>Data received</title>
		<link rel=\"stylesheet\" href=\"/styles.css\">
    </head>
    <body style=\"font-family: Arial, sans-serif; padding: 20px;\">
		<div class=\"container\">
			<h1>User Information</h1>
			<p><strong>Name:</strong> $name</p>
			<p><strong>Login:</strong> $login</p>
			<p><strong>Anecdote:</strong> $anecdote</p>
			<p><strong>Favourite color:</strong> $favorite_color</p>
			<p><strong>Age:</strong> $age</p>
			<p><strong>Surname:</strong> $surname</p>
			<hr>
			<p><em>This is a response generated by the PHP script.</em></p>
		</div>
    </body>
    </html>";
} else {
    // Si le login ne correspond pas, afficher un message d'erreur
    $body = "<html>
    <head>
        <title>Error</title>
    </head>
    <body style=\"font-family: Arial, sans-serif; padding: 20px;\">
        <h1>User not found</h1>
        <p>The login <strong>$login</strong> does not match any user.</p>
        <hr>
        <p><em>This is a response generated by the PHP script.</em></p>
		<p><a href=\"/form.html\">Go back to the form</a></p>
    </body>
    </html>";
}
// Construire les en-têtes HTTP
$header = "HTTP/1.1 200 OK\r\n";
$header .= "Content-Type: text/html\r\n";
$header .= "Content-Length: " . strlen($body) . "\r\n";
$header .= "\r\n";

// Envoyer la réponse HTTP complète
echo $header . $body;

exit(0);
?>