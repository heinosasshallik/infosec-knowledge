<?php
	$myfile = fopen("index.php", "r") or die("Unable to open file!");
	echo fread($myfile,filesize("index.php"));
	fclose($myfile);
?>
