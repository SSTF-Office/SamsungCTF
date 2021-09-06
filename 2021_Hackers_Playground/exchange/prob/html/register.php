<?php
include "config.php";

$salt = bin2hex(random_bytes(16));
$expire = mktime() + 600;
$krw = 1000;
$bt = 1.0;
$arr = ["salt"=>$salt, "expire"=>$expire, "krw"=>$krw, "bt"=>$bt];
$cipher = openssl_encrypt(json_encode($arr), "aes-256-cbc", KEY, OPENSSL_RAW_DATA, hex2bin(IV));

setcookie("state", base64_encode($cipher));
header("location: ./trade.php");
