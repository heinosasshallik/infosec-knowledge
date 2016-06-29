<html>
<head>
<title>Don't SQLi this, ya hear?</title>
</head>
<body>
	<?php
		$dbhost = 'localhost:3036';
		$dbuser = 'santaclaus';
		$dbpass = 'spaceaids';
		$connection = mysql_connect($dbhost, $dbuser, $dbpass); 
		if (!$connection) {
			die("Could not connect. " . mysql_error());
		}
		mysql_select_db("database1", $connection);
	
		$id = $_GET['id'];
		$sql = "select id, product_name from table1 where product_name='{$id}'" or die(mysql_error());;
		//$sql = "select id, product_name from table1 where id='".$id."'" or die(mysql_error());
		//$sql = "SELECT 1, 2";
		$retval = mysql_query($sql, $connection);
		if (!$retval) {
			echo mysql_error();
		}
		while ($row = mysql_fetch_array($retval, MYSQL_ASSOC)) {
			echo "ID: {$row['id']} <br>"."Product: {$row['product_name']}<br>";
		}
		echo "<br><br>".$sql;
		mysql_close($connection);
	
	?>
</body>
</html>

