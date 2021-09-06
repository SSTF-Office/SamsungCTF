<?php 
   if(!isset($_SESSION['user']))
   {
      header("Refresh:0; url=index.php?page=main");
      die();
   }
?>
<h1> Hi <?php echo $_SESSION['user'] ?> </h1>
