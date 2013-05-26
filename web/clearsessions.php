<?php
/**
 * @file
 * Clears PHP sessions and redirects to the connect page.
 */

 /* Load and clear sessions and storage file*/
session_start();
session_destroy();
fclose(fopen("auth.txt","w"));
 
/* Redirect to page with the connect to Twitter option. */
header('Location: ./connect.php');
?>