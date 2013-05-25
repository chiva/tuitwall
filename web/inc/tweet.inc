<script type="text/javascript" src="blackbirdpie.js"></script>
<script type="text/javascript" src="https://raw.github.com/twitter/twitter-text-js/master/twitter-text.js"></script>
<script type="text/javascript" src="http://code.jquery.com/jquery-1.6.4.min.js"></script>
<link rel="stylesheet" type="text/css" href="./css/blackbirdpie.css" />

<script type="text/javascript">
	$(document).ready(function() {
		document.getElementById('tweet').innerHTML = twttr.txt.autoLink(document.getElementById('tweet').innerHTML);
	});
</script>

<div id="bbpBox_1" class="bbpBox" style="background-color: #<?php echo $tweet['bkg_color'];?>; <?php if($tweet['use_bkg']==true){echo 'background-image:url('.$tweet['background'].')';}?>; <?php if($tweet['tile_bkg']==false){echo 'background-repeat: no-repeat;';}?>">
	 <?php if ($tweet['retweeted'] && $tweet['favorited']) { echo '<div style="float:left; width: 24px; height: 25px; background-image:url(./images/tweet-dogear.png); background-repeat:no-repeat; border-top-left-radius:5px; background-position: 0px -25px;"></div>';}
	 else if ($tweet['retweeted']) { echo '<div style="float:left; width: 24px; height: 25px; background-image:url(./images/tweet-dogear.png); background-repeat:no-repeat; border-top-left-radius:5px; background-position: 0px -50px;"></div>';}
	 else if ($tweet['favorited']) { echo '<div style="float:left; width: 24px; height: 25px; background-image:url(./images/tweet-dogear.png); background-repeat:no-repeat; border-top-left-radius:5px; background-position: 0px 0px;"></div>';} ?>
	<div class="bbpBoxIn" style="color: #<?php echo $tweet['text_color'];?>;">
		<style>.bbpBoxIn span a { color: #<?php echo $tweet['link_color'];?> }</style>
		<span class="bbpTweet" id="tweet"><?php echo $tweet['text'];?></span>
		<div class="bbp-actions">
			<a title="tuiteado el <?php echo $tweet['time_long'];?>h" style="padding-left:20px;" href="http://twitter.com/<?php echo $tweet['nick'];?>/status/<?php echo $tweet['id'];?>" target="_blank"><?php echo $tweet['time'];?></a>
			vía <?php echo $tweet['source'];?>
			<a href="https://twitter.com/intent/tweet?in_reply_to=<?php echo $tweet['id'];?>" onclick="javascript:_gaq.push([&#39;_trackEvent&#39;,&#39;outbound-article&#39;,&#39;twitter.com&#39;]);" class="bbp-action bbp-reply-action" title="Responder">
				<span><em></em>Responder</span></a>
			<a href="https://twitter.com/intent/retweet?tweet_id=<?php echo $tweet['id'];?>" onclick="javascript:_gaq.push([&#39;_trackEvent&#39;,&#39;outbound-article&#39;,&#39;twitter.com&#39;]);" class="bbp-action bbp-retweet-action" title="Retweet">
				<span><em></em>Retweet</span></a>
			<a href="https://twitter.com/intent/favorite?tweet_id=<?php echo $tweet['id'];?>" onclick="javascript:_gaq.push([&#39;_trackEvent&#39;,&#39;outbound-article&#39;,&#39;twitter.com&#39;]);" class="bbp-action bbp-favorite-action" title="Favorito">
				<span><em></em>Favorito</span></a>
		</div>
		<div style="float:left; padding:0; margin:0">
			<a href="http://twitter.com/intent/user?screen_name=<?php echo $tweet['nick'];?>" >
			<img class="colorbox2" src="<?php echo $tweet['avatar'];?>"></a>
		</div>
		<div style="float:left; padding:0; margin:0"><a style="font-weight:bold; color: #<?php echo $tweet['link_color'];?>" href="http://twitter.com/intent/user?screen_name=<?php echo $tweet['nick'];?>" style="text-decoration: none" >@<?php echo $tweet['nick'];?></a>
			<div style="margin:0; padding-top:2px"><?php echo $tweet['name'];?></div>
		</div>
		<div style="clear:both"></div>
	</div>
</div>