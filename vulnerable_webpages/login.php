<?php
/* 
 * Guidelines for setting up logins - READ THROUGH THEM
 * http://stackoverflow.com/questions/549/the-definitive-guide-to-form-based-website-authentication/477578#477578
 * Our current one is not secure against session fixation, for example.
 * 
 * Also we are vulnerable to timing attacks (though fix session fixation first)
 * https://paragonie.com/blog/2015/04/secure-authentication-php-with-long-term-persistence
 * blog.ircmaxell.com/2014/11/its-all-about-time.html
 * 
 * If also putting MySQL queries here to support more than 1 user, make sure to protect against SQLi
 */
?>

<!DOCTYPE html>
<?php
	ini_set('display_errors', 1);
	ini_set('display_startup_errors', 1);
	error_reporting(E_ALL);
	
	//How PHP sessions work (all the variables are stored server-side, the id is shared with the client):
	//http://stackoverflow.com/questions/1535697/how-do-php-sessions-work-not-how-are-they-used
	session_start();
	
	if (isset($_POST['logout'])) {
		//Clears all the data in $_SESSION. However, the session_id() will still stay the same and the $_SESSION array will still be there (though without any data)
		//http://www.pixelstech.net/article/1377961123-What-does-session_destroy%28%29-do-in-PHP for a bit more info.
		//You can also do unset($_SESSION['username'])
		session_destroy();
	}
	else if (!empty($_POST['username']) && !empty($_POST['password'])) {
		//password_hash also provided a salt, which is embedded into the string.
		if ($_POST['username'] === 'user' && password_verify($_POST['password'], "$2y$10$0TZsz.dhbTf8JsUSHHAzJ.eeMoyrJLnCRY7sykSRgsafo0QwHZcoO")) {
			$_SESSION['valid'] = true;
			$_SESSION['timeout'] = time();
			$_SESSION['username'] = 'user';
			
			if (isset($_POST['rememberme'])) {
				$cookie_key = bin2hex(openssl_random_pseudo_bytes(20));
				setcookie("rememberme", $cookie_key);
				//Should be a mysql database but I'm too lazy
				$cookie_file = fopen("cookiekey.txt", "w") or die("unable to open file");
				fwrite($cookie_file, password_hash($cookie_key, PASSWORD_DEFAULT));
				fclose($cookie_file);
			}
			header('Location: /walledoff.php');
			exit;
		} else {
			$wrongPass = true;
		}
	}
?>

<html>
	
<head>
	<title>w4nka Co.</title>
	<link href="site.css" rel="stylesheet">
</head>

<body>
	<nav id="nav1"></nav>

	<div id="main">
		<h1>Login to restricted area</h1>
		<?php
			if (isset($_SESSION['valid']) && $_SESSION['valid'] == true) {
				echo "logged in.";
			} if (isset($_POST['logout'])) {
				echo "Logged out.";
			} if (isset($wrongPass)) {
				echo "Wrong login.";
			}
		?>
		<form name="form1" method="post" action="">
			<input name="username" type="text" id="username" placeholder="username"><br>
			<input name="password" type="password" id="password" placeholder="password"><br>
			<input type="submit" name="login" value="Login">
			<input type="checkbox" name="rememberme" value="rememberme"> Remember me
		</form>
		<form name="form2" method="post" action="">
			<input name="logout" type="hidden" id="logout">
			<input type="submit" name="logout" value="Logout">
		</form>
		<footer id="footer1"></footer>
	</div>
	
	<script src="https://vulnerable/script.js"></script>
</body>
</html>
