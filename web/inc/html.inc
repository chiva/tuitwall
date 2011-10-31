<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
	<head>
		<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
		<title>tuitwall</title>
		<link rel="stylesheet" type="text/css" href="./css/style.css" />
	</head>
	<body>
		<div>
			<h1><span>tuit</span>wall</h1>
			<?php if (DISALLOW_LOGOUT){
				switch ($access_token['type']){
					case "timeline":
						echo "<h3>¿Cual es la actualidad?</h3>";
						break;
					case "menciones":
						echo "<h3>¡Menciona a @{$access_token['screen_name']} y aparece!</h3>";
						break;
					case "propios":
						echo "<h3>¿Que nos cuenta?</h3>";
						break;
				}
			}
			else echo "<h3>Inicia sesión con tu usuario y verás los tweets en la barra de LEDs</h3>";
			?>
		</div>
		<?php if (isset($error)) {
				if (isset($access_token['user_id'])) {include('logged.inc');}
				echo '<br />'.$error.'<br />';
			}
			else { 
				if (isset($tweet)) {
					if (STREAM) {include('logged_st.inc');}
					else {include('logged.inc');}
					if (SHOW_TWEET) {include('tweet.inc');}
					else {echo '<div style="clear:both"></div>';}
				}
				if (isset($login)) {include('login.inc');}
			} 
		include('footer.inc'); ?>
	</body>
</html>