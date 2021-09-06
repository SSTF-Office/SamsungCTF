<?php
include "../salt.php";

function get_info() {
	$info = openssl_decrypt(base64_decode($_COOKIE["state"]), "aes-256-cbc", KEY, OPENSSL_RAW_DATA, hex2bin(IV));
	if ($info === false) {
		setcookie("state", "");
		exit("Wrong state");
	}
	$info = json_decode($info);
	if ($info->expire < mktime()) {
		setcookie("state", "");
		exit("Expired");
	}

	// Update state
	$info->salt = bin2hex(random_bytes(16));
	$info->expire = mktime() + 600;
	return $info;
}

function set_info($info) {
	setcookie("state", base64_encode(openssl_encrypt(json_encode($info), "aes-256-cbc", KEY, OPENSSL_RAW_DATA, hex2bin(IV))));
}
