<?php
  class db
  {
     public $conn, $res;
     function __construct()
     {
        global $servername, $username, $password, $db_name;
        $this->conn = mysql_connect($servername, $username, $password) or die("connect error");
        mysql_select_db($db_name);
     }

     function execute($query)
     {
        $this->res = mysql_query($query) or die("SQL ERROR");
     }

     function fetch_all()
     {
        $res = array();
        while($row = mysql_fetch_row($this->res))
        {
           array_push($res, $row[0]);
        }
        return $res;
     }

     function fetch_arr()
     {
        return mysql_fetch_array($this->res);
     }

     function fetch_one()
     {
        $res = mysql_fetch_row($this->res);
        return count($res) > 0 ? $res[0] : NULL;
     }

     function get_auto_incre()
     {
        return mysql_insert_id();
     }

  };


  $DB = new db();


  class note
  {
     public $user, $letter, $attached_file;

     function addfile($realname, $filename)
     {
        global $DB;

        $realname = mysql_real_escape_string($realname);
        $filename = mysql_real_escape_string($filename);

        $DB->execute("insert into files values (NULL, \"${realname}\", \"${filename}\")");
        $this->attached_file = $DB->get_auto_incre();

     }

     function add()
     {
        global $DB;
        $str = serialize($this);
        $str = $this->filter($str);

        $user = mysql_real_escape_string($this->user);
        $str = mysql_real_escape_string($str);
        $DB->execute("insert into notes values (\"${user}\", \"${str}\")");
     }

     function filter($str)
     {
        global $profanity_word_replace;
        $filter_word = array("s**t", "f**k", "as*", "bi**h", "H**l");
        foreach($filter_word as $word)
        {
           $replace = str_repeat($profanity_word_replace, strlen($word));
           $word = preg_quote($word);
           $str = eregi_replace($word, $replace, $str);
        }
        return $str;
     }

     function resolve_file()
     {
        global $DB;
        $id = $this->attached_file;
        if($id)
        {
          $DB->execute("select realname, path from files where id=${id}");
          return $DB->fetch_arr();
       }
       return NULL;
     }
  };

?>
