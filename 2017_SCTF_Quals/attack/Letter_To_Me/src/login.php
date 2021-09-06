<?php
   require "models.php";
   if(isset($id) && isset($pw))
   {
      $id = mysql_real_escape_string($id);
      $pw = mysql_real_escape_string($pw);

      $DB->execute("select * from users where username=\"${id}\" and password = \"${pw}\"");
      $result = $DB->fetch_one();
      if($result)
      {
         $_SESSION['user'] = $id;
         header("Refresh:0; url=index.php?page=main");
         die();
      }
      else
      {
         $err = "alert('No No No~~');";
      }
   }

?>

<div class="login-page">
  <div class="form">
     <form class="register-form" method="post">
        <h1> Inviter only </h1>
        <p> Tell admin, if you want to be invited </p>
      <p class="message">Already registered? <a href="#">Sign In</a></p>
   </form>

   <form class="login-form" method="post" action="?page=login">
      <input type="text" placeholder="username" name="id"/>
      <input type="password" placeholder="password" name="pw"/>
      <button>login</button>
      <p class="message">Not registered? <a href="#">Create an account</a></p>
    </form>
  </div>
</div>
<script>
   <?php if(isset($err)) echo $err ?>
   $('.message a').click(function(){
            $('form').animate({height: "toggle", opacity: "toggle"}, "slow");
      });
</script>
