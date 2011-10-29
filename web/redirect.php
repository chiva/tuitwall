<?php

/* Cargamos la configuración y librerías */
session_start();
require_once('twitteroauth/twitteroauth.php');
require_once('config.php');

/* Construimos un objeto TwiterOAuth con las credenciales del cliente */
$connection = new TwitterOAuth(CONSUMER_KEY, CONSUMER_SECRET);
 
/* Obtener credenciales temporales */
$request_token = $connection->getRequestToken(OAUTH_CALLBACK);

/* Guardamos las credenciales temporales en la sesión */
$_SESSION['oauth_token'] = $token = $request_token['oauth_token'];
$_SESSION['oauth_token_secret'] = $request_token['oauth_token_secret'];
$_SESSION['type'] = $_POST['tweet'];
 
/* Si la conexión falló, no redirigir al link de autorización */
switch ($connection->http_code) {
	case 200:
		/* Construir la URL de autorización y redirigir al usuario a ella */
		$url = $connection->getAuthorizeURL($token);
		header('Location: ' . $url); 
		break;
	default:
		/* Mostrar notificación si algo fue mal */
		echo 'No se pudo conectar a Twitter. Actualiza la página o intentalo más tarde';
}
?>