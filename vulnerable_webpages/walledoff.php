<?php
	session_start();
	
	$cookie_file = fopen("cookiekey.txt", "r") or die("unable to open file");
	$hashed_cookie  = fread($cookie_file, filesize("cookiekey.txt"));
	echo $hashed_cookie;
	if (!isset($_SESSION['username'])) {
		exit;
		if (!isset($_COOKIE['rememberme']) && !password_verify($_COOKIE['rememberme'])) {
			header("Location: /login.php");
		}
	}
?>
<html>
<head>
	<title>Secret stash</title>
	<link href="site.css" rel="stylesheet">
</head>

<body>
	<div id="main">
		<h2>You found me!</h2>
		<img src="https://www.fndvisions.org/img/cutecat.jpg">
	</div>
	
	<script src="/script.js"></script>
</body>
</html>
