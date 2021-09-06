<?php
$mysqli = new mysqli("dbb", "siteb", "12345678", "siteb");

function get_userinfo() {
	global $mysqli;
	if ($_COOKIE['id']) {
		$cid = addslashes($_COOKIE['id']);
		$cpw = $_COOKIE['pw'];

		$q = $mysqli->query("select * from member where user_id='{$cid}'");

		if ($q->num_rows == 0) {
			exit("No mathced ID. <a href=./signin.php>Sign in</a>");
		}

		$res = $q->fetch_object();

		if ($res->password != md5($cpw)) {
			exit("No mathced ID and PW. <a href=./signin.php>Sign in</a>");
		}

		return $res;
	} else {
		echo "<script>location.href='signin.php';</script>";
		return null;
	}
}
