<?php
include "config.php";

$info = get_info();

if ($_GET['idx'] == 1) { // case: flag / 2.1 BT
	if ($info->bt < 2.1) {
		exit("Need more BT");
	}
	$info->bt -= 2.1;
	set_info($info);
	echo "SCTF{1t_W4s_n0T_MY_f4U1T}";
} else if ($_GET['idx'] == 2) { // case: chance 50% off / 0.1 BT
	if ($info->bt < 0.1) {
		exit("Need more BT");
	}
	$info->bt -= 0.1;
	set_info($info);
	echo "You can buy BT with 50% off next month";
	echo "<br><a href=./items.php>[Go back]</a>";
} else {
	set_info($info);
	exit("Invalid item");
}
