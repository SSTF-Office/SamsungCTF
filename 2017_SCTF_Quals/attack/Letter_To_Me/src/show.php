  <div class="container">
   <h1> Your Letters </h1>
<?php
   if($_SESSION['user'] === "admin")
   {
     die("Nope!");
  }

  require "models.php";
  $user = $_SESSION['user'];
  $user = mysql_real_escape_string($user);
  $DB->execute("select data from notes where username=\"${user}\"");
  $notes = $DB->fetch_all();

  foreach($notes as $note)
  {
     $note = unserialize($note);

     echo "<br>";
     $letter = $note->letter;
     $info = $note->resolve_file();
     echo "<div class=\"container\">";
     echo "<blockquote>";
     echo "<p>${letter}</p>";
     if(isset($info))
     {
        $path = $info[1];
        $name = $info[0];
        echo "<footer>Attached file: <cite title=\"Source Title\"><a href=\"".$path."\" download=\"".$name."\">file</a></cite></footer>";
    }
     else
       echo "<footer>Attached file: None</footer>";
       echo "</blockquote>";
    echo "</div>";
   }
?>
</div>
