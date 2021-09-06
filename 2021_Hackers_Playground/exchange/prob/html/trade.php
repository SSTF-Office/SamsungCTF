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
			<table>
				<tr>
					<td style="background-color:#0000ff44" colspan=2>&nbsp;<br>&nbsp;</td>
					<td rowspan=6>
<p>
<form action=order.php method=post>
						<strong>Order Type</strong> <input type=radio name=ordertype value=bid onclick="document.getElementsByName('price')[0].value=1001" checked>buy <input type=radio name=ordertype value=ask onclick="document.getElementsByName('price')[0].value=1000">sell</p>
<p>
						<strong>Price Type</strong> <input type=radio name=pricetype value=limit disabled>limit <input type=radio name=pricetype value=market checked>market</p>
<p align=left><strong>Balance</strong></p>
<p align=right>
<?=$info->krw?> <strong>KRW</strong><br>
<?=$info->bt?> <strong>BT</strong><br>
</p>
<p>
<strong>Amount</strong> <input type=number name=amount step=0.0001 value=0.1 min=0.1>BT<br>
<strong>Price</strong> <input type=number name=price value=1001 min=1000 max=1001 disabled>KRW/BT<br>
<font size=2>
* Minimum amount to order is 0.1 BT.<br>
* Fee rate is 0.20%.
</font>
</p>
<p align=center><input type=submit value="Make Order"></p>
</form>
					</td>
				</tr>
				<tr><td style="background-color:#0000ff44" colspan=2>&nbsp;<br>&nbsp;</td></tr>
				<tr style="background-color:#0000ff44" onclick=document.getElementsByName('price')[0].value=1001><td style="width:50px"><p align=left style="color:red"><span style="font-size:16px">1,001</span><br><span style="font-size:9px">+0.10%</span></p></td><td style="width:50px"><p align=right style="font-size:12px">&infin;<br>&nbsp;</p></td></tr>
				<tr style="background-color:#ff000044" onclick=document.getElementsByName('price')[0].value=1000><td style="width:50px"><p align=left style="color:black"><span style="font-size:16px">1,000</span><br><span style="font-size:9px">0.00%</span></p></td><td style="width:50px"><p align=right style="font-size:12px">&infin;<br>&nbsp;</p></td></tr>
				<tr><td style="background-color:#ff000044" colspan=2>&nbsp;<br>&nbsp;</td></tr>
				<tr><td style="background-color:#ff000044" colspan=2>&nbsp;<br>&nbsp;</td></tr>
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
