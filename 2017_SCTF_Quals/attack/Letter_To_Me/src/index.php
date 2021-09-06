<!DOCTYPE html>
<html>
   <head>
      <title>Letter To Me</title>
      <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
      <link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/toastr.js/latest/toastr.min.css">
      <link rel="stylesheet" href="./app.css">
      <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1" />

      <script src="https://code.jquery.com/jquery-3.1.1.min.js"></script>
      <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
      <script src="//cdnjs.cloudflare.com/ajax/libs/toastr.js/latest/toastr.min.js"></script>
   </head>
   <body>
      <?php
         require "conn.php";
         @extract($_GET);
         @extract($_POST);

         $page = isset($page) ? $page : "main";
         if(preg_match("uploads", $page))
         {
            die("No hack");
         }
         session_start();
      ?>
      <div class="container">
         <nav>
         <ul class="nav navbar-nav">
            <li role="presentation" class="active"><a href="?page=main">Home</a></li>
            <?php if(isset($_SESSION['user'])) : ?>
            <li role="presentation" class="active"><a href="?page=send">send</a></li>
            <li role="presentation" class="active"><a href="?page=show">show</a></li>
            <?php endif; ?>
         </ul>
         <ul class="nav navbar-nav navbar-right">
            <?php if($_SESSION['user'] === "admin") : ?>
            <li role="presentation"><a href="?page=admin">admin page</a></li>
            <?php endif; ?>
            <?php if(isset($_SESSION['user'])) : ?>
            <li role="presentation"><a href="?page=mypage"><?php echo $_SESSION['user'] ?></a></li>
            <li role="presentation"><a href="?page=logout">logout</a></li>
            <?php else : ?>
            <li role="presentation"><a href="?page=login">login</a></li>
            <li role="presentation"><a href="?page=register">register</a></li>
            <?php endif; ?>

         </ul>
         </nav>
      </div>
      <?php
         (include($page.".php")) or die("<h1>404 Not Found</h1>");
      ?>
   </body>
</html>
