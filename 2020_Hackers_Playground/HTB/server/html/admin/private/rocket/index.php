<!DOCTYPE html>
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    
    <title>Server debug</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- Le styles -->
    <link href="./index/bootstrap.css" rel="stylesheet">

    <style type="text/css">
      body {
        padding-top: 60px;
        padding-bottom: 40px;
      }
    </style>
    <link href="./index/bootstrap-responsive.css" rel="stylesheet">

  </head>

  <body>

    <div class="navbar navbar-inverse navbar-fixed-bottom">
      <div class="navbar-inner">
        <div class="container">
          <a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </a>
          <a class="brand" href="">Connection debug</a>
          <div class="nav-collapse collapse">
            <ul class="nav">
              <li class="active"><a href="../index.php">Home</a></li>
            </ul>
          </div><!--/.nav-collapse -->
        </div>
      </div>
    </div>
        <div class="container">
    <pre>
    <p align="center">System:</p>
<?php
  system("cat start");
?>
</pre>
      <footer>
      </footer>

    </div> <!-- /container -->
  
    <div class="container">
    <pre>
    <p align="center">Check avalible id of rocket:</p><form action='index.php?ip=' method='GET' align="center">Please input id: <input name='id'/><input type='submit' value='submit'/></form>
<?php
error_reporting(E_ERROR | E_PARSE);
  if (!(preg_match('/^\d{1,3}$/', $_GET['id']))) {
     header("Location: index.php?id=1");
  }
  system("sh rocket.sh ".$_GET['id']);
?>
</pre>
      <footer>
      </footer>

    </div> <!-- /container -->

    <div class="container">
    <pre>
        Some moudle
</pre>
      <footer>
      </footer>

    </div> <!-- /container -->





</body></html>
