<?php
error_reporting(E_ALL);
include "config.php";
$id = intval($_GET['id']);
if ($id > 2147483647) {
	exit(json_encode(["result"=>"no"]));
}
if (!consume($id)) {
	exit(json_encode(["result"=>"no"]));
}
$mateId = get_flag_mate($id);
if (check_consume($mateId)) {
	exit(json_encode(["result"=>"no"]));
}
echo json_encode(["key"=>base64_encode(get_key($id)),"mate"=>$mateId]);
?>
