<?php

define('SERVER', 'tuitwall.kungfulabs.com');							// definir la raíz de la página
define('CONSUMER_KEY', '');						// llaves de aplicación de twitter
define('CONSUMER_SECRET', '');
define('OAUTH_CALLBACK', 'http://'.SERVER.'/callback.php');				// fichero que recibe los callback de twitter
define('API_KEY', '4Z17HHTeWELgxqIsB3WMWfu9V1SESwh6YKoG77Nr');			// clave para limitar la petición de tweets (dejar vacío para desactivar el control)
define('SHOW_TWEET', true);												// mostrar tweet en página principal
define('SHOW_USER', true);												// incluir de quien es el tweet cuando Arduino lo pida
define('STREAM', false);												// mostrar stream desde UStream para demos del proyecto
define('USTREAM_ID', 9584783);											// ID del stream de UStreama a mostrar
?>