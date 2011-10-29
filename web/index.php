<?php

/*
	This file is part of tuitwall.
	
    tuitwall is an Arduino program that shows tweets, provided by
    a backend that interfaces Twitter, in a LED matrix.
    Copyright (C) 2011 Santiago Reig

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduBus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with tuitwall. If not, see <http://www.gnu.org/licenses/>.
	
	Tweet embedding idea and basecode gotten from:
	http://themergency.com/twitter-blackbird-pie-wordpress-plugin-demo/
*/

/* Cargamos la configuración y librerías */
require_once('twitteroauth/twitteroauth.php');
require_once('config.php');

/* Recogemos los datos del fichero donde están almacenados */
$file_c = file_get_contents("auth.txt");
$colms = explode(",",trim($file_c));
/* Si no tenemos los tokens (fichero vacío), redirijimos a la página de conexión */
if (count($colms)<6) {
	header('Location: ./clearsessions.php');
}
$access_token = array();
$access_token['oauth_token'] = $colms[0];
$access_token['oauth_token_secret'] = $colms[1];
$access_token['user_id'] = $colms[2];
$access_token['screen_name'] = $colms[3];
$access_token['img_url'] = $colms[4];
$access_token['name'] = $colms[5];
$access_token['type'] = $colms[6];

/* Creamos un objeto TwitterOauth con los tokens de consumer y usuario */
$connection = new TwitterOAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_token['oauth_token'], $access_token['oauth_token_secret']);

/* Establecemos la zona horaria y la traducción de fechas */
date_default_timezone_set('Europe/Madrid');
$week_days = array ("", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo");
$months = array ("", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre");

/* Pedimos los tweets a Twitter */
if ($access_token['type'] == 'timeline') { $content = $connection->get('statuses/home_timeline', array('count' => 1)); }
else if ($access_token['type'] == 'menciones') { $content = $connection->get('statuses/mentions', array('count' => 1)); }
else { $content = $connection->get('statuses/user_timeline', array('count' => 1, 'include_rts' => 'true')); }

if ($connection->http_code == 200){

	/* Comprobamos que hayan tweets para mostrar */
	if (count($content) == 0) {
		$error = 'No hay tweets';
		include('./inc/html.inc');
		exit;
	}

	/* Recogemos los datos que necesitemos de la respuesta */
	$tweet = array();
	if (property_exists($content[0],'retweeted_status')) {
		$response = $content[0]->retweeted_status;
		$tweet['retweeted'] = true;
	}
	else {
		$response = $content[0];
		$tweet['retweeted'] = false;
	}
	
	$tweet['favorited'] = $content[0]->favorited;
	$tweet['nick'] = $response->user->screen_name;
	$tweet['name'] = $response->user->name;
	$tweet['avatar'] = $response->user->profile_image_url;
	$tweet['background'] = $response->user->profile_background_image_url;
	$tweet['bkg_color'] = $response->user->profile_background_color;
	$tweet['use_bkg'] = $response->user->profile_use_background_image;
	$tweet['tile_bkg'] = $response->user->profile_background_tile;
	$tweet['id'] = $response->id_str;
	$tweet['text'] = $response->text;
	$tweet['source'] = $response->source;
	$tweet['time'] = substr($week_days[date("N",strtotime($response->created_at))],0,3)." ".date("j",strtotime($response->created_at))." ".substr($months[date("n",strtotime($response->created_at))],0,3)." ".date("G:i",strtotime($response->created_at));
	$tweet['time_long'] = date("j",strtotime($response->created_at))." de ".$months[date("n",strtotime($response->created_at))]." de ".date("Y",strtotime($response->created_at))." a las ".date("G:i",strtotime($response->created_at));
	$tweet['text_color'] = $response->user->profile_text_color;
	$tweet['link_color'] = $response->user->profile_link_color;

}
else {
	switch ($connection->http_code){
		case 420:
			$error = 'Alcanzado el límite de peticiones por hora';
			break;
		default:
			$error =  'Error al conectar con Twitter: '.$connection->http_code;
			break;
	}
}

/* Cargamos la página html */
include('./inc/html.inc');
?>