<?php

include "secret.php";	//server_secret, iv, flag

$cookie_name = "SESSION";

if (!isset($_COOKIE[$cookie_name])) {
	header('Location: /signin.php');
	exit;
}

$cipher="aes-128-ctr";
list($iv, $encrypted_session_data) = explode("|", base64_decode($_COOKIE[$cookie_name]), 2);
$session_data = openssl_decrypt($encrypted_session_data, $cipher, $server_secret, OPENSSL_RAW_DATA, $iv);
list($username, $auth_code) = explode("|", $session_data);
if ($auth_code !== $server_secret) {
	die("No hack!");
}
?>

<html>
<head>

<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6" crossorigin="anonymous">

<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.1/dist/umd/popper.min.js" integrity="sha384-SR1sx49pcuLnqZUnnPwx6FCym0wLsk5JZuNx2bPPENzswTNFaQU1RDvt3wT4gWFG" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.min.js" integrity="sha384-j0CNLUeiqtyaRmlzUHCPZ+Gy5fQu0dQ6eZ/xAww941Ai1SxSY+0EQqNXNE6DZiVc" crossorigin="anonymous"></script>

</head>
<body>
	<div class="px-4 py-5 my-5">
		<div class="col-lg-6 mx-auto text-center">
			<p class="lead mb-4">
<?php
print("Welcome, ".$username." :)<br>");

if ($username === "admin") {
	print("Flag: ".$flag);
}else{
	print("Only admin can see the flag. Sorry.");
}
?>
		</p>
		<div>
			<button type="button" class="btn btn-success btn-sm" onclick="location.href='index.php?source';">View Source</button>

			<button type="button" class="btn btn-warning btn-sm" onclick="location.href='signin.php?logout';">Logout</button>
		</div>
	</div>
<?php

if (isset($_GET['source'])) {
	print("<hr>\n");
	highlight_file(__FILE__);
}

?>
</div>
</body>
</html>
