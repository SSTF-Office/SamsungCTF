<?php
   if(!(isset($_SESSION['user']) && $_SESSION['user'] === "admin"))
   die();

   require "models.php";
   if(isset($id) && isset($pw))
   {
      if(strlen($id) < 8 || strlen($pw) < 8)
        $err = "should be len(id) > 8 && len(pw) > 8";
      else
      {
         $id = mysql_real_escape_string($id);
         $pw = mysql_real_escape_string($pw);

         $DB->execute("select * from users where username=\"${id}\"");
         $result = $DB->fetch_one();
         if($result)
         {
            $err = "user already exists";
         }
         else
         {
            $DB->execute("insert into users values(\"${id}\", \"${pw}\")");
            $err = "successfully invite!";
         }
      }
   }
   $err = isset($err) ? "alert(\"".$err."\");" : NULL;
?>
<div class="login-page">
  <div class="form">
     <form class="register-form" method="post" action="?page=admin">
        <h1> Inviter user </h1>
        <p> Hi admin, invite who?</p>
      <input type="text" placeholder="name" name="id"/>
      <input type="password" placeholder="password" name="pw"/>
      <button>create</button>
   </form>
  </div>
  </div>

  <script>
     <?php echo $err; ?>
   $("form").animate({height: "toggle", opacity: "toggle"});
</script>
