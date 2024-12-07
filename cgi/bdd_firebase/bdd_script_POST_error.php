<?php

function customErrorHandler($errno, $errstr, $errfile, $errline) {
    if ($errno == E_WARNING) {
        // Ici tu peux traiter les warnings spécifiquement
        echo "Avertissement : $errstr\n";
        // Retourner true pour éviter l'affichage du warning natif
		echo "Cacatch";
		exit(1);
        return true;
    }
    // Pour d'autres types d'erreurs, tu peux gérer autrement
    return false;  // Permet à PHP de continuer à traiter l'erreur par défaut
}

set_error_handler("customErrorHandler");

ini_set('default_mimetype', '');

// Lire tout le corps de la requête depuis l'entrée standard
$rawInput = file_get_contents("php://stdin");

// Écrire l'entrée brute dans la sortie d'erreur pour débogage (facultatif)
fwrite(fopen("php://stderr", "w"), "Données brutes : $rawInput\n");

// Parser les données reçues si elles sont au format application/x-www-form-urlencoded
parse_str($rawInput, $postData);

// Récupérer les valeurs des champs
$firstName = isset($postData['name']) ? htmlspecialchars($postData['name']) : 'Non fourni';
$login = isset($postData['login']) ? htmlspecialchars($postData['login']) : 'Non fourni';
$anecdote = isset($postData['anecdote']) ? htmlspecialchars($postData['anecdote']) : 'Non fourni';
$favorite_color = isset($postData['favorite_color']) ? htmlspecialchars($postData['favorite_color']) : 'Non fourni';
$age = isset($postData['age']) ? htmlspecialchars($postData['age']) : 'Non fourni';
$surname = isset($postData['surname']) ? htmlspecialchars($postData['surname']) : 'Non fourni';


$url = "https://webservcgi-default-rtdb.europe-west1.firebasedatabase.app/users/{$login}.json";


$data = [
	'name' => $firstName,
	'last_name' => $lastName,
	'login' => $login,
	'anecdote' => $anecdote,
	'favorite_color' => $favorite_color,
	'age' => $age,
	'surname' => $surname
];

// Convertir les données en JSON
$json_data = json_encode($data);


$options = [
    'http' => [
        'method'  => 'PUT',
        'header'  => "Content-Type: application/json\r\n" .
                     "Content-Length: " . strlen($json_data) . "\r\n",
        'content' => $json_data
    ]
];

$context = stream_context_create($options);

// Effectuer la requête PUT
$response = file_get_contents($url, false, $context);

// Vérifier si la requête a réussi
if ($response === FALSE) {
    die('Error occurred while sending PUT request');
}

// Décoder la réponse JSON
$response_data = json_decode($response, true);

// Construire le corps de la réponse HTML
$body = "<html>
<head>
	<title>Data updated</title>
	<link rel=\"stylesheet\" href=\"/styles.css\">
</head>
<body style=\"font-family: Arial, sans-serif; padding: 20px;\">
	<div class=\"container\">

		<h1>User Information Updated</h1>
		<p><strong>Login:</strong> $login</p>
		<p><strong>Name:</strong> " . $response_data['name'] . "</p>
		<p><strong>Anecdote:</strong> " . (isset($response_data['anecdote']) ? $response_data['anecdote'] : 'No pet information') . "</p>
		<p><strong>favourite color:</strong> " . (isset($response_data['favorite_color']) ? $response_data['favorite_color'] : 'No pet information') . "</p>
		<p><strong>age:</strong> " . (isset($response_data['age']) ? $response_data['age'] : 'No pet information') . "</p>
		<p><strong>surname:</strong> " . (isset($response_data['surname']) ? $response_data['surname'] : 'No pet information') . "</p>
		<hr>
		<p><em>This is a response generated by the PHP script.</em></p>
		<p><a href=\"/form.html\">Go back to the form</a></p>
	</div>
</body>
</html>";

// Construire les en-têtes HTTP
$header = "HTTP/1.1 200 OK\r\n";
$header .= "Content-Type: text/html\r\n";
$header .= "Content-Length: " . strlen($body). "\r\n";
$header .= "\r\n";

// Envoyer la réponse HTTP complète
echo $header . $body;
exit(0);
?>

