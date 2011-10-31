<div id="loggedIn" class="block" style="margin-bottom: 30px">
	<div style="float: left; padding-right: 8px; margin: 0; text-align: right;">
		<?php echo ucfirst($access_token['type']);?>
		<div style="margin:0; padding-top:3px;">
			<?php if (!DISALLOW_LOGOUT){
				echo "<a href='./clearsessions.php'>Cerrar Sesión</a>";
			}?>
		</div>
	</div>
	<div style="float: left; padding: 0; margin: 0; height: 48px; border-left: 1px solid #E6E6E6;">
		<img style="padding-left:8px" src=<?php echo $access_token['img_url'];?> />
	</div>
	<div style="float: left; padding: 0; margin: 0; text-align: left;">
		<strong><a href=http://twitter.com/intent/user?screen_name=<?php echo $access_token['screen_name'];?>>@<?php echo $access_token['screen_name'];?></a></strong>
		<div style="margin:0; padding-top:2px"><?php echo $access_token['name'];?></div>
	</div>
</div>