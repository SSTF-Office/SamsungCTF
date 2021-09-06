<?php
include "config.php";

$recv = intval($_GET['id']);
$start = intval($_GET['list']) * 30;
if ($_GET['order'] == "asc") {
	$qres = $mysqli->query("select idx, msg from packets where recv='{$recv}' order by idx asc limit {$start}, 30");
} else {
	$qres = $mysqli->query("select idx, msg from packets where recv='{$recv}' order by idx desc limit {$start}, 30");
}
$result = Array();
while($arr = $qres->fetch_assoc()) {
	array_push($result, $arr);
}
echo json_encode($result);
?>

