<?php
   session_destroy();
   header("Refresh:0; url=index.php?page=main");
   die();
?>
