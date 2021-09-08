<?php
   function generateRS($length = 64) {
      $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
      $charactersLength = strlen($characters);
      $randomString = '';

      $urandom = fopen("/dev/urandom", "rb");
      for ($i = 0; $i < $length; $i++) {
         $randomString .= $characters[ord(fread($urandom, 1)) % ($charactersLength - 1)];
      }
      return $randomString;
   }


   if($_SESSION['user'] === "admin")
   {
      die("Nope!");
   }

   require "models.php";
   if(isset($letter))
   {
      $note = new note();
      $note->user = $_SESSION['user'];
      $note->letter = $letter;

      if($_FILES['file']['size'] > 0)
      {
         if($_FILES['file']['size'] > 30)
         {
            echo "too big";
         }
         else if($_FILES['file']['error'] > 0 || !is_uploaded_file($_FILES['file']['tmp_name']))
         {
            echo "error1";
         }
         else
         {
            $uploadfile = "./uploads/".generateRS();
            if (move_uploaded_file($_FILES['file']['tmp_name'], $uploadfile))
            {
               $fname = $_FILES['file']['name'];
               $note->addfile($fname, $uploadfile);
            }
            else
            {
               echo "error2";
            }
         }
      }
      $note->add();
   }
?>

<div class="login-page">
   <div class="form">
      <h4> To. me</h4>
      <form class="" method="post" enctype="multipart/form-data">
         <textarea class="form-control" rows="3" name="letter"></textarea>
         <div class="form-group">
            <label for="File">attach</label>
            <input type="file" id="File" name="file">
         </div>
         <div style="padding-top:14px;">
            <button>send</button>
         </div>
      </form>
   </div>
</div>

