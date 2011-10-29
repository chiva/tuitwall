<?php

/* Cargamos la configuración y librerías */
session_start();
require_once('twitteroauth/twitteroauth.php');
require_once('config.php');

/* Creamos un objeto TwitteroAuth */
$connection = new TwitterOAuth(CONSUMER_KEY, CONSUMER_SECRET, $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);

/* Pedimos los tokens de acceso a Twitter */
$access_token = $connection->getAccessToken($_REQUEST['oauth_verifier']);
$content = $connection->get('account/verify_credentials');

/* Guardamos los tokens además del nombre y dirección de la imágen del perfil */
$access_token['img_url'] = $content->profile_image_url;
$access_token['name'] = $content->name;
$access_token['type'] = $_SESSION['type'];
file_put_contents("auth.txt", implode(",",$access_token), LOCK_EX);

/* Borramos los tokens de petición que no son ya necesarios */
unset($_SESSION['oauth_token']);
unset($_SESSION['oauth_token_secret']);
unset($_SESSION['type']);

/* Si la respuesta HTTP es 200 continuamos, si no redirijimos a la página de conexión para reintentarlo */
if ($connection->http_code == 200) {
  /* El usuario ha sido verificado y los tokens son válidos */
  header('Location: ./index.php');
} else {
  header('Location: ./clearsessions.php');
}
?>