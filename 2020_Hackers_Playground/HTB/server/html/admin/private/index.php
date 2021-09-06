<?php
session_start();

	if (isset($_POST['username']) && isset($_POST['password'])) {
    $username = $_POST['username'];
    $password = $_POST['password'];
    if (!isset($_POST['token'])) {
        $error = "No token";
    } else if ($_POST['token'] != $_SESSION['token']) {
        $error = 'Token is incorrect';
    } else if ($username == 'secret_admin_login' && $password == 'SuP3R_S3CR3T_P@SSW0RD') {
        echo '<meta http-equiv="refresh" content="0;url=rocket/index.php">';
        #echo "Success";
        die();
    } else if ($username != 'secret_admin_login') {
        $error = "The username or password is incorrect";
    } else if ($password != 'SuP3R_S3CR3T_P@SSW0RD') {
        $error = "The username or password is incorrect";
    }
}

$token = md5(rand(1000, 9999));
$_SESSION['token'] = $token;
?>

<!DOCTYPE html>
<html lang="en">
<head>
	<title>Account Login</title>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
<!--===============================================================================================-->	
	<link rel="icon" type="image/png" href="images/icons/icon.png"/>
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/bootstrap/css/bootstrap.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="fonts/font-awesome-4.7.0/css/font-awesome.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="fonts/Linearicons-Free-v1.0.0/icon-font.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/animate/animate.css">
<!--===============================================================================================-->	
	<link rel="stylesheet" type="text/css" href="vendor/css-hamburgers/hamburgers.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/animsition/css/animsition.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/select2/select2.min.css">
<!--===============================================================================================-->	
	<link rel="stylesheet" type="text/css" href="vendor/daterangepicker/daterangepicker.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="css/util.css">
	<link rel="stylesheet" type="text/css" href="css/main.css">
<!--===============================================================================================-->
</head>
<body>
	
	<div class="limiter">
		<div class="container-login100">
			<div class="wrap-login100">
				<form class="login100-form validate-form" method="post">
					<span class="login100-form-title p-b-34">
						Account Login
					</span>
					<input type="hidden" name="token" value="<?= $token?>" action="/">
					<div class="wrap-input100 rs1-wrap-input100 validate-input m-b-20" data-validate="Type user name">
						<input id="first-name" class="input100" type="text" name="username" placeholder="User name">
						<span class="focus-input100"></span>
					</div>
					<div class="wrap-input100 rs2-wrap-input100 validate-input m-b-20" data-validate="Type password">
						<input class="input100" type="password" name="password" placeholder="Password">
						<span class="focus-input100"></span>
					</div>
					<div class="container-login100-form-btn">
					<p><div style="text-align: center; color: red"><?php echo $error ?></div></div></p>
					<div class="container-login100-form-btn">
						<button class="login100-form-btn">
							Sign in
						</button>
					</div>

					<div>
						<p><br></p>
						<p><br></p>
						<p><br></p>
						<p><br></p>
						<p><br></p>
						<p><br></p>
						<p><br></p>
						<p><br></p>
					</div>
		
					<div class="w-full text-center">
						<a href="#" class="txt3">
							<img src="images/icons/icon.png" style="height: 70px;width: 70px">
						</a>
					</div>
				</form>

				<div class="login100-more" style="background-image: url('images/bg-01.jpg');"></div>
			</div>
		</div>
	</div>
	
	

	<div id="dropDownSelect1"></div>
	
<!--===============================================================================================-->
	<script src="vendor/jquery/jquery-3.2.1.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/animsition/js/animsition.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/bootstrap/js/popper.js"></script>
	<script src="vendor/bootstrap/js/bootstrap.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/select2/select2.min.js"></script>
	<script>
		$(".selection-2").select2({
			minimumResultsForSearch: 20,
			dropdownParent: $('#dropDownSelect1')
		});
	</script>
<!--===============================================================================================-->
	<script src="vendor/daterangepicker/moment.min.js"></script>
	<script src="vendor/daterangepicker/daterangepicker.js"></script>
<!--===============================================================================================-->
	<script src="vendor/countdowntime/countdowntime.js"></script>
<!--===============================================================================================-->
	<script src="js/main.js"></script>

</body>
</html>