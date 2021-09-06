<?php
include "config.php";
if ($_POST['id']) {
	setcookie("id", $_POST['id']);
	setcookie("pw", $_POST['pw']);
	header("location: migrate.php");
	exit;
}
?>
<h1 align=center>The Former Fun and Cool Service</h1>
<h4 align=center>(SIMPLE IS BEST style)</h4>
<h2 align=center><a href=./>[Main]</a></h2>
<form method=post>
<p align=center>
<strong>Login Page</strong><br><br>
ID : <input type=text name=id><br>
PW : <input type=password name=pw><br>
<input type=submit value="Sign in"><br>
</p>
</form>
