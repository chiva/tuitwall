<form action="redirect.php" method="post">
	<div id="login" class="block" style="padding-top: 0px;">
		<div style="clear: both; padding-top: 15px; padding-bottom: 15px;"><strong>¿Que tweets quieres mostrar?</strong></div>
		<div style="float: left; padding-right: 15px; margin: 0; text-align: left; border-right: 1px solid #E6E6E6;">
			<input type="radio" name="tweet" value="timeline" checked="checked" />Timeline<br />
			<input type="radio" name="tweet" value="menciones" />Menciones<br />
			<input type="radio" name="tweet" value="propios" />Propios<br />
		</div>
		<div style="float: right; padding-left: 15px; margin: 0;">
			<input type="image" src="https://si0.twimg.com/images/dev/buttons/sign-in-with-twitter-d.png" style="padding-top: 22px">
		</div>
	</div>
</form>