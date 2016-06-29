<?php
	/*
	ini_set('display_errors','On');
	ini_set('error_reporting',E_ALL);*/
	
	$dbhost = 'localhost:3036';
	$dbuser = 'santaclaus';
	$dbpass = 'spaceaids';
	
	$connection = mysql_connect($dbhost, $dbuser, $dbpass);  
	if (!$connection) {
		die("Could not connect. " . mysql_error());
	}
	mysql_select_db("customerdatabase", $connection);
	
	$sql = "select * from customerdata";
	$result = mysql_query($sql, $connection) or die ("Error in Selecting " . mysql_error($connection));
	$temparray = array();
	while ($row = mysql_fetch_assoc($result)) {
		$temparray[] = $row;
	}
	
	echo json_encode($temparray);
?>
