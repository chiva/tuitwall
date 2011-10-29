<?php

/* Cargamos la configuración y librerías */
require_once('twitteroauth/twitteroauth.php');
require_once('config.php');
if (defined('API_KEY') && API_KEY != ''){
	if (!isset($_GET['api']) || $_GET['api'] != API_KEY){
		echo 'API key incorrecta!';
		exit;
	}
}

/* Si no existe el fichero de autenticación, salimos */
if (!file_exists("auth.txt")){
	print 'Visita '.SERVER;
	exit;
}

/* Obtenemos los tokens de acceso */
$file_c = file_get_contents("auth.txt");
$colms = explode(",",trim($file_c));
/* Si no tenemos los tokens, indicamos las acciones a tomar */
if (count($colms)<6) {
	print 'Visita '.SERVER;
} else {
	$access_token = array();
	$access_token['oauth_token'] = $colms[0];
	$access_token['oauth_token_secret'] = $colms[1];
	$access_token['user_id'] = $colms[2];
	$access_token['screen_name'] = $colms[3];
	$access_token['img_url'] = $colms[4];
	$access_token['name'] = $colms[5];
	$access_token['type'] = $colms[6];

	/* Creamos un objeto TwitterOAuth con los tokens de usuario y consumer */
	$connection = new TwitterOAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_token['oauth_token'], $access_token['oauth_token_secret']);
	/* Recogemos el tweet, lo procesamos y mostramos */
	if ($access_token['type'] == 'timeline') { $pack = $connection->get('statuses/home_timeline', array('count' => 1)); }
	else if ($access_token['type'] == 'menciones') { $pack = $connection->get('statuses/mentions', array('count' => 1)); }
	else { $pack = $connection->get('statuses/user_timeline', array('count' => 1, 'include_rts' => 'true')); }
	switch ($connection->http_code) {
		case 200:
			/* Comprobamos que hayan tweets para mostrar */
			if (count($pack) == 0) {
				echo 'No hay tweets';
				break;
			}
			$name = $pack[0]->user->screen_name;
			$text = $pack[0]->text;
			$text = str_replace(array('¿','º','ª'),array(),$text);
			setlocale(LC_ALL, 'es_ES');
			if (SHOW_USER) $text = iconv('UTF-8', 'ASCII//TRANSLIT', '@'.$name.': '.$text);
			else $text = iconv('UTF-8', 'ASCII//TRANSLIT', $text);
			echo $text.''.chr(0);
			break;
		case 420:
			echo 'Alcanzado el límite de peticiones por hora';
			break;
		default:
			echo 'Error al conectar con Twitter';
			break;
	}
}
?>