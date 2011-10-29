<?php

/* Cargamos la configuración */
require_once('config.php');

/* Salimos con un mensaje de error si CONSUMER_KEY o CONSUMER_SECRET no están definidos */
if (CONSUMER_KEY === '' || CONSUMER_SECRET === '') {
  $error = 'Tienes que definir las claves de aplicación. Consigue unas en <a href="https://twitter.com/apps">https://twitter.com/apps</a>';
}

/* Activamos para mostrar el html apropiado de dentro del inc */
$login = true;
/* Cargamos la página HTML */
include('./inc/html.inc');
?>