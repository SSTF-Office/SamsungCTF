<?php
include "config.php";
$res = get_userinfo();
if (!$res->is_admin) {
	exit("You are not admin.");
}
?>
Admin Page<br>
See <a href=admin.php?mode=log>logs</a> or <a href=admin.php?mode=information>information</a><br>
<?php
if ($_GET['mode'] == "log") {
	$_POST['idx'] = addslashes($_POST['idx']);
	$q = $mysqli->query("select * from log where idx={$_POST['idx']}");
	if (!$q) {
		echo "Error occurred<br>";
	} else {
		echo "Log hidden<br>";
	}
	echo "<form action=admin.php?mode=log method=post>";
	echo "IDX : <input type=text name=idx><br>";
	echo "<input type=submit value=Search><br>";
	echo "</form>";
} else if ($_GET['mode'] == "information") {
	$q = $mysqli->query("select * from information");
	echo "PROTOCOL | HOST | ID | PW<br>";
	while($res = $q->fetch_object()) {
		$masked = substr($res->pw, 0, 5).str_repeat("*", strlen($res->pw) - 5);
		echo "{$res->protocol} | {$res->host} | {$res->id} | {$masked}<br>";
	}
}
?>
