<?php
include "config.php";
$q = $mysqli->query("select timestampdiff(second, val, now()) > 120 as should from reset_history order by no desc limit 1");
$res = $q->fetch_object();
if ($res && $res->should == 1) {
	$mysqli->query("truncate member");
	$mysqli->query("insert into reset_history (val) values (now())");
}
$q = $mysqli->query("insert into member values (NULL, 0, '{$_GET['id']}', '{$_GET['pw']}')");
echo $q;
