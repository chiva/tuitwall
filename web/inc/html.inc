<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
	<head>
		<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
		<title>tuitwall</title>
		<link rel="stylesheet" type="text/css" href="css/style.css" />
	</head>
	<body>
		<a href="http://github.com/chiva/tuitwall"><img style="position: absolute; top: 0; right: 0; border: 0;" src="https://s3.amazonaws.com/github/ribbons/forkme_right_white_ffffff.png" alt="Fork me on GitHub"></a>
		<div>
			<h1><span>tuit</span>wall</h1>
			<?php if (DISALLOW_LOGOUT){
				switch ($access_token['type']){
					case "timeline":
						echo "<h3>What's happening now?</h3>";
						break;
					case "mentions":
						echo "<h3>Mention @{$access_token['screen_name']} and appear!</h3>";
						break;
					case "own":
						echo "<h3>What's telling us?</h3>";
						break;
				}
			}
			else echo "<h3>Login with your user and you will see the tweets in the LED matrix</h3>";
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