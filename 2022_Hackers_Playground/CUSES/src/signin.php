<?php

include "dbutil.php";
include "secret.php";

$cookie_name = "SESSION";

$die_msg = false;
$username = $_POST['username'];
$password = $_POST['password'];
$uid = find_from_db($username, $password);
$ip = $_SERVER['REMOTE_ADDR'];

if (isset($_GET['register'])) {
	setcookie($cookie_name, "", time() - 3600, "/");
	$die_msg = "Ask admin. Only admin can add an account.";
} else if (isset($_GET['logout'])) {
	setcookie($cookie_name, "", time() - 3600, "/");
	$die_msg = "Successfully logged out.";
} else if ($username === "admin" && !in_array($ip, $admin_login_ip)) {
	$die_msg = "Remote login is not allowed for admin.";
} else if (isset($username) && isset($password)) {
	if ($uid !== false) {
		$cipher="aes-128-ctr";
		$iv = openssl_random_pseudo_bytes(openssl_cipher_iv_length($cipher));
		$session_data = $username."|".$server_secret;
		$encrypted_session_data = openssl_encrypt($session_data, $cipher, $server_secret, OPENSSL_RAW_DATA, $iv);
		setcookie($cookie_name, base64_encode($iv."|".$encrypted_session_data));
		header('Location: /index.php');
		exit;
	} else {
		$die_msg = "Login failed. Check your account info.";
	}
}

?>
<html>
<head>

<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6" crossorigin="anonymous">

<link href="calm_breeze.css" rel="stylesheet">

<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.1/dist/umd/popper.min.js" integrity="sha384-SR1sx49pcuLnqZUnnPwx6FCym0wLsk5JZuNx2bPPENzswTNFaQU1RDvt3wT4gWFG" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.min.js" integrity="sha384-j0CNLUeiqtyaRmlzUHCPZ+Gy5fQu0dQ6eZ/xAww941Ai1SxSY+0EQqNXNE6DZiVc" crossorigin="anonymous"></script>

</head>

<body>
<!-- https://codepen.io/Lewitje/pen/BNNJjo -->

<div class="wrapper">
	<div class="container">
		<h1>Welcome</h1>
		
		<form class="form" method="post" action="signin.php">
			<input type="text" placeholder="Username" name="username">
			<input type="password" placeholder="Password" name="password">
			<button type="submit" id="login-button">Login</button>
		</form>
		<form style="padding:0px">
			<button type="button" style="background-color: rgba(255, 255, 255, 0);color: white;" onclick="location.href='signin.php?register';">Register</button>
  		</form>
	</div>
	
	<ul class="bg-bubbles">
		<li></li>
		<li></li>
		<li></li>
		<li></li>
		<li></li>
		<li></li>
		<li></li>
		<li></li>
		<li></li>
		<li></li>
	</ul>
</div>

<?php
if ($die_msg) {
?>
<div class="position-fixed bottom-0 start-50 translate-middle p-3" style="z-index: 5">
<div class="toast align-items-center text-white bg-success border-0" role="alert" aria-live="assertive" aria-atomic="true">
  <div class="d-flex">
    <div class="toast-body">
      <?php echo $die_msg; ?>
    </div>
    <button type="button" class="btn-close btn-close-white me-2 m-auto" data-bs-dismiss="toast" aria-label="Close"></button>
  </div>
</div>
</div>
<?php
}
?>

<script>
window.onload = function() {
	var toastElList = [].slice.call(document.querySelectorAll('.toast'));
	var toastList = toastElList.map(function(toastEl) {
	// Creates an array of toasts (it only initializes them)
  	return new bootstrap.Toast(toastEl) // No need for options; use the default options
});
toastList.forEach(toast => toast.show()); // This show them
};
</script>

<!-- guest account: guest / guestpassword -->

</body>
</html>
