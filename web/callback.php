<?php
/**
 * @file
 * Take the user when they return from Twitter. Get access tokens.
 * Verify credentials and redirect to based on response from Twitter.
 */

session_start();
require_once('twitteroauth/twitteroauth.php');
require_once('config.php');

/* Create TwitteroAuth object with app key/secret and token key/secret from default phase */
$connection = new TwitterOAuth(CONSUMER_KEY, CONSUMER_SECRET, $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);

/* Request access tokens from twitter */
$access_token = $connection->getAccessToken($_REQUEST['oauth_verifier']);
$content = $connection->get('account/verify_credentials');

/* Save tokens, also name and profile image url */
$access_token['img_url'] = $content->profile_image_url;
$access_token['name'] = $content->name;
$access_token['type'] = $_SESSION['type'];
file_put_contents("auth.txt", implode(",",$access_token), LOCK_EX);

/* Remove no longer needed request tokens */
unset($_SESSION['oauth_token']);
unset($_SESSION['oauth_token_secret']);
unset($_SESSION['type']);

/* If HTTP response is 200 continue otherwise send to connect page to retry */
if ($connection->http_code == 200) {
  /* The user has been verified and the access tokens can be saved for future use */
  header('Location: ./index.php');
} else {
  /* Save HTTP status for error dialog on connnect page.*/
  header('Location: ./clearsessions.php');
}
?>