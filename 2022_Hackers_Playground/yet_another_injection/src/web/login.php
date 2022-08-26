<?php
	session_start();

	$username = check_param("username");
	$pwd = check_param("pwd");

	if (checkUser($username, $pwd)) {
		$_SESSION["username"] = $username;
		header("Location: index.php");
	}

	function check_param($var) {
		if (!isset($_POST[$var]) || $_POST[$var] === "") {
			return "";
		}
		return trim($_POST[$var]);
	}

	function checkUser($username, $pwd) {
		if (($username === "") || ($pwd === "")) {
			return false;
		}

		$accounts = @file_get_contents("accounts.txt");
		if ($accounts === false) {
			$users = array();
		} else {
			$users = explode("\n", $accounts);
		}

		array_push($users, "guest:".hash("sha256", "guest"));

		$granted = false;
		foreach ($users as $each) {
			$info = explode(":",$each);
			if ( $username === trim($info[0]) && hash("sha256", $pwd) === trim($info[1]) ) {
				$granted = true;
				break;
			}
		}

		return $granted;
	}
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<title>Yet Another Injection</title>
		<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/css/bootstrap.min.css" integrity="sha384-zCbKRCUGaJDkqS1kPbPd7TveP5iyJE0EjAuZQTgFLD2ylzuqKfdKlfG/eSrtxUkn" crossorigin="anonymous">
		<link rel="stylesheet" href="css/style.css">
	</head>
	<body>
		<div class="container">
			<div class="w-50 mx-auto shadow-lg p-3 my-5 bg-white rounded">
				<h1 class="mb-5">Research Paper Repository</h1>
				<form action="login.php" method="POST">
						<label for="username"><b>Username</b></label>
						<input type="text" placeholder="Enter Username" name="username" required> <br>
						<label for="pwd"><b>Password</b></label>
						<input type="password" placeholder="Enter Password" name="pwd" required> <br>
						<button type="submit">Login</button>
				</form>
				<a href="login.php?showsrc=login.php">hint</a>
			</div>
			<?php
				if (isset($_GET['showsrc']) && in_array($_GET['showsrc'], Array('index.php', 'login.php', 'library.php', 'paperdetail.php'), true)) {
					highlight_file($_GET['showsrc']);
				}
			?>
		</div>
	</body>
</html>
