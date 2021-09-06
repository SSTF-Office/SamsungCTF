<?php
include "../salt.php";
error_reporting(0);
define("FLAG", "SCTF{th3r3_1s_A_r34s0n_n0t_t0_u5e_ECB}");
define("FLAG_MSG", "give me the flag my mate");
define("ZERO16", "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");

$mysqli = new mysqli("db", "xero", "12345678", "xero");

function get_key($id) {
	if ($id == 1) {
		$hex_digest = "9b1d5efdb3c2d7bd2c7901e01fdb3916";
	} else if ($id == 3463874446) {
		$hex_digest = "85be20b9b90b050efb1699c8817931e1";
	} else {
		$hex_digest = md5(SALT.$id);
	}
	return pack('H*', $hex_digest);
}

function get_flag_mate($id) {
	return unpack('N', pack('H*', md5("a6877135158ace0df30edcd0fb69aa99".$id)))[1] | 0x80000000;
}

function check_consume($id) {
	global $mysqli;
	return $mysqli->query("select count(*) from consumes where idx='{$id}'")->fetch_array()[0] > 0;
}

function consume($id) {
	global $mysqli;
	if (check_consume($id)) return false;
	$mysqli->query("insert into consumes values ('{$id}')");
	return true;
}
