<?php

/**
 * @file
 * Check if consumer token is set and if so send user to get a request token.
 */

/**
 * Exit with an error message if the CONSUMER_KEY or CONSUMER_SECRET is not defined.
 */
require_once('config.php');
if (CONSUMER_KEY === '' || CONSUMER_SECRET === '') {
  $error = 'Tienes que definir las claves de aplicación. Consigue unas en <a href="https://twitter.com/apps">https://twitter.com/apps</a>';
}

/* Activate the login portion of the html page */
$login = true;

/* Include HTML to display on the page. */
include('./inc/html.inc');
?>