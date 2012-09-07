<?php

define('SERVER', $_SERVER['SERVER_NAME']);  							// definir la raíz de la página
define('CONSUMER_KEY', '');						// llaves de aplicación de twitter
define('CONSUMER_SECRET', '');
define('ANYWHERE_CONSUMER_KEY', '');			// llaves de @Anywhere de Twitter (sirve para auto-enlazar los usuarios de twitter, hastags y enlaces en los tweets)
define('OAUTH_CALLBACK', 'http://'.SERVER.'/callback.php');				// fichero que recibe los callback de twitter
define('API_KEY', '4Z17HHTeWELgxqIsB3WMWfu9V1SESwh6YKoG77Nr');			// clave para limitar la petición de tweets (dejar vacío para desactivar el control)
define('SHOW_TWEET', true);												// mostrar tweet en página principal
define('SHOW_USER', true);												// incluir de quien es el tweet cuando Arduino lo pida
define('STREAM', false);												// mostrar stream desde twitch.tv para demos del proyecto
define('TWITCH_ID', 'xxxx');											// ID del stream de twitch.tv a mostrar
define('DISALLOW_LOGOUT', false);										// deshabilita el poder cerrar sesión, útil para demos o si queremos fijar la configuración
?>