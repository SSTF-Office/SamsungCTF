<?php
//error_reporting(E_ALL);
include "config.php";

function encrypt_ecb($key, $plaintext) {
	$ciphertext_new = "";
	for($i = 0; $i <= floor(strlen($plaintext) / 15); $i++) {
		$ciphertext_new = $ciphertext_new.openssl_encrypt(substr($plaintext, $i * 15, 15), "aes-128-cbc", $key, OPENSSL_RAW_DATA, ZERO16);
	}
	return $ciphertext_new;
}

function decrypt_ecb($key, $ciphertext) {
	$plaintext = "";
	for($i = 0; $i < ceil(strlen($ciphertext) / 16); $i++) {
		$plaintext_block = openssl_decrypt(substr($ciphertext, $i * 16, 16), "aes-128-cbc", $key, OPENSSL_RAW_DATA, ZERO16);
		if ($plaintext_block === false) {
			return false;
		}
		$plaintext = $plaintext.$plaintext_block;
	}
	return $plaintext;
}

$parsed = unpack('Ifrom/Ito/a*msg', base64_decode($_GET['msg']));
$from = $parsed['from'];
$to = $parsed['to'];
$msg = $parsed['msg'];

if ($to == 0) {
	// Copy the original message into 0's inbox
	$msg_base64ed = base64_encode($msg);
	$mysqli->query("insert into packets values (NULL, {$from}, {$to}, '{$msg_base64ed}')");

	// Decrypt by $from(outer)'s key
	$plaintext = decrypt_ecb(get_key($from), $msg);
	if ($plaintext === false) {
		exit(json_encode(["result"=>"no"]));
	}

	$parsed = unpack('Ifrom/Ito/a*msg', $plaintext);
	$from = $parsed['from'];
	$to = $parsed['to'];

	if ($from != 1 && stripos($parsed['msg'], "flag") !== false) {
		exit(json_encode(["result"=>"filter"]));
	}

	if ($to == get_flag_mate($from) && json_decode($parsed["msg"])->msg == FLAG_MSG) {
		if (consume($to)) {
			exit(json_encode(["result"=>"flag", "flag"=>FLAG]));
		}
	}

	// Encrypt by $to(inner)'s key
	$msg = encrypt_ecb(get_key($to), $plaintext);
} else if ($to > 0x80000000) { // $to is flag mate
	if ($to != get_flag_mate($from)) {
		exit(json_encode(["result"=>"no"]));
	}

	// Decrypt by $to(outer)'s key
	$plaintext = decrypt_ecb(get_key($to), $msg);

	$parsed = unpack('Ifrom/Ito/a*msg', $plaintext);
	$from = $parsed['from'];
	$to = $parsed['to'];

	if (json_decode($parsed["msg"])->msg == FLAG_MSG) {
		if (consume($to)) {
			exit(json_encode(["result"=>"flag", "flag"=>FLAG]));
		}
	}
}
$msg = base64_encode($msg);
$mysqli->query("insert into packets values (NULL, {$from}, {$to}, '{$msg}')");
echo json_encode(["result"=>"ok"]);

