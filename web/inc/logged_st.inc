<div style="display: inline-block; height: 303px margin-bottom: 40px">
	<div id="loggedIn" class="block" style="float: left; margin-top: 80px; margin-bottom: 80px;">
		<div style="float: left; padding: 0; margin: 0; height: 48px; margin-bottom: 10px;">
			<img class="colorbox2" src=<?php echo $access_token['img_url'];?> />
		</div>
		<div style="float: left; padding: 0; margin: 0; text-align: left;">
			<strong><a href=http://twitter.com/intent/user?screen_name=<?php echo $access_token['screen_name'];?>>@<?php echo $access_token['screen_name'];?></a></strong>
			<div style="margin:0; padding-top:2px"><?php echo $access_token['name'];?></div>
		</div>
		<div style="clear:both;"></div>
		<div style="margin: 0; text-align: center; border-top: 1px solid #E6E6E6; padding-top: 5px;">
			<?php echo ucfirst($access_token['type']);?>
			<div style="margin:0;">
				<?php if (!DISALLOW_LOGOUT){
					echo "<a href='./clearsessions.php'>Logout</a>";
				}?>
			</div>
		</div>
	</div>
	<div style="float: left; display: inline-block; margin-left: 30px">
		<object type="application/x-shockwave-flash" height="378" width="620" id="live_embed_player_flash" data="http://es-es.twitch.tv/widgets/live_embed_player.swf?channel=<?php echo TWITCH_ID;?>" bgcolor="#000000"><param name="allowFullScreen" value="true" /><param name="allowScriptAccess" value="always" /><param name="allowNetworking" value="all" /><param name="movie" value="http://es-es.twitch.tv/widgets/live_embed_player.swf" /><param name="flashvars" value="hostname=es-es.twitch.tv&channel=<?php echo TWITCH_ID;?>&auto_play=true&start_volume=0" /></object>
	</div>
</div>