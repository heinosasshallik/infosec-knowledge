<html>
<body>
	<?php if (empty($_POST)): ?>
		<form action=<?php echo $_SERVER["PHP_SELF"]; ?> method="POST">
			First name:<br>
			<input type="text" name="firstname"><br>
			Last name:<br>
			<input type="text" name="lastname"><br>
			<input type="submit">
		</form>
	<?php else: ?>
		Welcome, <?php echo $_POST["firstname"]." ".$_POST["lastname"]."!"; ?><br>
	<?php endif; ?>
</body>
</html>