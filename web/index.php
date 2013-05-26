<?php

/*
	This file is part of tuitwall.
	
    tuitwall is an Arduino program that shows tweets, provided by
    a backend that interfaces Twitter, in a LED matrix.
    Copyright (C) 2011 Santiago Reig

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduBus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with tuitwall. If not, see <http://www.gnu.org/licenses/>.
	
	Tweet embedding idea and basecode gotten from:
	http://themergency.com/twitter-blackbird-pie-wordpress-plugin-demo/
*/

/* Load required lib files. */
require_once('twitteroauth/twitteroauth.php');
require_once('config.php');

/* Load tokens from storage file */
$file_c = file_get_contents("auth.txt");
$colms = explode(",",trim($file_c));
/* If access tokens are not available redirect to connect page. */
if (count($colms)<6) {
	header('Location: ./clearsessions.php');
}

/* Load tokens from storage file */
$access_token = array();
$access_token['oauth_token'] = $colms[0];
$access_token['oauth_token_secret'] = $colms[1];
$access_token['user_id'] = $colms[2];
$access_token['screen_name'] = $colms[3];
$access_token['img_url'] = $colms[4];
$access_token['name'] = $colms[5];
$access_token['type'] = $colms[6];

/* Create a TwitterOauth object with consumer/user tokens. */
$connection = new TwitterOAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_token['oauth_token'], $access_token['oauth_token_secret']);

/* Stablish timezone */
date_default_timezone_set('Europe/Madrid');

/* Ask Twitter for tweets */
if ($access_token['type'] == 'timeline') { $content = $connection->get('statuses/home_timeline', array('count' => 1)); }
else if ($access_token['type'] == 'mentions') { $content = $connection->get('statuses/mentions_timeline', array('count' => 1)); }
else { $content = $connection->get('statuses/user_timeline', array('count' => 1, 'include_rts' => 'true')); }

if ($connection->http_code == 200){

	/* Check that there are tweets available */
	if (count($content) == 0) {
		$error = 'There are no tweets';
		include('inc/html.inc');
		exit;
	}

	/* Get the data required */
	$tweet = array();
    /* Was this tweet a retweet of the user? */
	if (property_exists($content[0],'retweeted_status')) {
		$response = $content[0]->retweeted_status;
		$tweet['retweeted'] = true;
	}
	else {
		$response = $content[0];
		$tweet['retweeted'] = false;
	}
	
	$tweet['favorited'] = $content[0]->favorited;
	$tweet['nick'] = $response->user->screen_name;
	$tweet['name'] = $response->user->name;
	$tweet['avatar'] = $response->user->profile_image_url;
	$tweet['background'] = $response->user->profile_background_image_url;
	$tweet['bkg_color'] = $response->user->profile_background_color;
	$tweet['use_bkg'] = $response->user->profile_use_background_image;
	$tweet['tile_bkg'] = $response->user->profile_background_tile;
	$tweet['id'] = $response->id_str;
	$tweet['text'] = $response->text;
	$tweet['source'] = $response->source;
	$tweet['time'] = date("D",strtotime($response->created_at))." ".date("j",strtotime($response->created_at))." ".date("M",strtotime($response->created_at))." ".date("G:i",strtotime($response->created_at));
	$tweet['time_long'] = date("j",strtotime($response->created_at))." ".date("F",strtotime($response->created_at))." ".date("Y",strtotime($response->created_at))." at ".date("G:i",strtotime($response->created_at));
	$tweet['text_color'] = $response->user->profile_text_color;
	$tweet['link_color'] = $response->user->profile_link_color;

}
else {
	switch ($connection->http_code){
		case 420:
			$error = 'Hourly request limit reached';
			break;
		default:
			$error =  'Error connecting to Twitter: '.$connection->http_code;
			break;
	}
}

/* Include HTML to display on the page */
include('inc/html.inc');
?>