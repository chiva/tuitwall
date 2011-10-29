<?php
 
/* Destruimos la sesión y vaciamos el fichero de datos */
session_start();
session_destroy();
fclose(fopen("auth.txt","w"));
 
/* Redirijimos a la página para conectarse con Twitter */
header('Location: ./connect.php');
?>