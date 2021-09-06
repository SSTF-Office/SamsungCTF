<?php
include "config.php";

$info = get_info();
set_info($info);
?>
<html>
	<head>
		<link rel="stylesheet" href="./exchange.css">
	</head>
	<body>
		<h1 align=center>Exchange for BT</h1>
		<h2 align=center><a href=./>[Main]</a> <a href=./trade.php>[Trade]</a> <a href=./items.php>[Claim]</a> <a href=./register.php>[Register]</a></h2>
		<p align=center>
			<?php
			echo "<strong>Your balance</strong><br>{$info->krw} KRW<br>{$info->bt} BT<br><br>";
			echo "<strong>Items</strong><br>";
			?>
			<table>
			<tr><th>Item Name</th><th>Item Price</th><th>Claim</th></tr>
			<tr><td>flag</td><td>2.1 BT</td><td><a href=./claim.php?idx=1>Claim</a></td></tr>
			<tr><td>Chance 50% off after 1 month</td><td>0.1 BT</td><td><a href=./claim.php?idx=2>Claim</a></td></tr>
			</table>
		</p>
	</body>
</html>

<html>
<head>
</head>
<body>
</body>
</html>
