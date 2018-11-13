<?php
error_reporting(E_ALL);

echo "<h2>Connexion TCP/IP</h2>\n";

/* Lit l'adresse IP du serveur de destination */
$address = '127.0.0.1';

/* Crée un socket TCP/IP. */
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    echo "socket_create() a échoué : raison :  " . socket_strerror(socket_last_error()) . "\n";
} else {
    echo "OK.\n";
}

echo "Essai de connexion à '$address' sur le port '$service_port'...";
$result = socket_connect($socket, $address, $service_port);
if ($socket === false) {
    echo "socket_connect() a échoué : raison : ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
} else {
    echo "OK.\n";
}

$in = "enigme 6";

echo "Envoi de la requête HTTP HEAD...";
socket_write($socket, $in, strlen($in));
echo "OK.\n";

?>