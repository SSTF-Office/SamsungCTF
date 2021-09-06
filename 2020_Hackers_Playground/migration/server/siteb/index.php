<?php
include "config.php";

$res = get_userinfo();
?>
<h1 align=center>The Fun and Cool Service</h1>
<h4 align=center>(SIMPLE IS BEST style)</h4>
<h2 align=center><a href=./>[Main]</a> <a href=./signin.php>[Sign in]</a></h2>
<p align=center>
<?php
echo "Hello {$res->user_id}!<br>";
?>
Our service is under construction so your data can be gone without any notice.<br>
Only <a href=./admin.php>admin</a> page is ready right now.<br>
</p>
