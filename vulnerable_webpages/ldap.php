<?php
	$username = 'admin';
	$pass = 'spaceaids';
	$basedn = 'dc=test,dc=com';
	
	
	$conn = ldap_connect("127.0.0.1", 389)  or die("Could not connect to LDAP server.");
	
	if ($conn) {
		if (ldap_bind($conn, $_GET['username'], $_GET['password'])) {
			// log them in!
			echo "true";
		} else {
			// error message
			echo "false";
		}
	}
	
?>
