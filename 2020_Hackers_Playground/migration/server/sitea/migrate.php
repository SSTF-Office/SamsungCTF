<?php
include "config.php";

if ($_COOKIE['id']) {
	$cid = $_COOKIE['id'];
	$cpw = $_COOKIE['pw'];

	$q = $mysqli->query("select * from member where user_id='{$cid}'");

	if ($q->num_rows == 0) {
		exit("No matched ID");
	}

	$res = $q->fetch_object();

	if ($res->password != md5($cpw)) {
		exit("No mathced ID and PW");
	}
}

if ($_POST['id']) {
	if (strlen($_POST['id'] > 50)) {
		exit("Too long id");
	}
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, "{$SECRET_URL}?id=".urlencode($_POST['id'])."&pw=".md5($_POST['pw']));
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
	$result = curl_exec($ch);
	curl_close($ch); 
	if (strlen($result) == 1) {
		exit("Success");
	} else {
		exit("Failed");
	}
}

?>
<h1 align=center>The Former Fun and Cool Service</h1>
<h4 align=center>(SIMPLE IS BEST style)</h4>
<h2 align=center><a href=./>[Main]</a></h2>
<form method=POST>
<p align=center>
<strong>Migration Page</strong><br><br>
User ID, encrypted password and etc will be migrated.<br>
Input your new account information.<br><br>
ID : <input type=text name=id value="<?=$cid?>"><br>
PW : <input type=password name=pw><br>
<input type=submit value="Migrate"><br>
</p>
</form>
