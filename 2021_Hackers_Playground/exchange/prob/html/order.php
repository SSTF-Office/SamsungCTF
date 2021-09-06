<?php
include "config.php";

$info = get_info();

$ordertype = $_POST['ordertype'];
$amount = $_POST['amount'];

$amount = intval($amount * 10000) / 10000;
if ($amount < 0.1) {
	exit("Check minimum amount to order");
}

if ($ordertype == "bid") {
	$price = 1001;
	$amountkrw = round($price * $amount);
	if ($info->krw < $amountkrw) {
		exit("Need more KRW");
	}
	$fee = intval(0.0020 * $amount * 10000) / 10000;
	$info->krw -= $amountkrw;
	$info->bt += $amount - $fee;
} else if ($ordertype == "ask") {
	$price = 1000;
	$amountkrw = round($price * $amount);
	if ($info->bt < $amount) {
		exit("Need more BT");
	}
	$feekrw = round(0.0020 * $amountkrw);
	$info->bt -= $amount;
	$info->krw += $amountkrw - $feekrw;
} else {
	exit("Wrong order type");
}

set_info($info);
echo "<a href=./trade.php>[Go back]</a>";
?>
