<?php

ini_set("display_errors", 0);

if (isset($_GET['source'])) {
    highlight_file(__FILE__);
    exit();
}

include_once('flag.php');
@session_start();
define('TIME_LIMIT', 20);
define('NEED_CORRECT', 10);


function set_session($key, $val) {
    $_SESSION[$key] = $val;
}
function get_session($key) {
    return isset($_SESSION[$key]) ? $_SESSION[$key] : '';
}

$MSG = '';

function create_new_captcha() {
    $symbols = '0123456789';
    $keystring = '';
    // $length = mt_rand(5,6);
    $length = 6;
    for ($i=0;$i<$length;$i++) {
        $keystring.=$symbols{mt_rand(0,strlen($symbols)-1)};
    }
    return $keystring;
}

function get_mp3_abspath() {
    $prefix = dirname(__FILE__)."/cache/";
    if (isset($_COOKIE['PHPSESSID'])) {
        return $prefix.md5($_COOKIE['PHPSESSID']).".mp3";
    } else {
        return "";
    }
}
function get_mp3_url() {
    $prefix = "./cache/";
    if (isset($_COOKIE['PHPSESSID'])) {
        return $prefix.md5($_COOKIE['PHPSESSID']).".mp3?t=".time().mt_rand(100,999);
    } else {
        return "";
    }
}

function create_mp3() {
    $filepath = get_mp3_abspath();
    $captcha_val = get_session('captcha_val');
    $captcha_mp3 = get_session('captcha_mp3');
    if ($filepath !== "" && $captcha_val !== $captcha_mp3) {
        @unlink($filepath);
        $mp3_segments = array();
        for($i=0;$i<strlen($captcha_val);$i++) {
            $file = dirname(__FILE__).'/mp3/'.$captcha_val[$i].'.mp3';
            $mp3_segments[] = $file;
        }
        $contents = '';
        foreach($mp3_segments as $segment) {
            $contents .= file_get_contents($segment);
        }
        file_put_contents($filepath, $contents);

        // delete old files
        if (rand(0, 99) == 0) {
            foreach(glob(dirname(__FILE__)."/cache/*.mp3") as $file) {
                // one hour lifetime
                if (filetime($file) + 60 * 60 < time()) {
                    @unlink($file);
                }
            }
        }
        set_session('captcha_mp3', $captcha_val);
    }
    return get_mp3_url();
    
}
if (!isset($_COOKIE['PHPSESSID'])) {
    exit("No cookie");
}

if (isset($_POST['captcha_val']) && get_session('captcha_val') != '') {
    if (get_session('captcha_val') == $_POST['captcha_val']) {
        //correct captcha value
        $correct_cnt = get_session('correct_cnt');
        if ($correct_cnt === '0' || $correct_cnt === '') {
            $correct_cnt = '1';
            set_session('submit_time', time());
        }
        $correct_cnt++;
        if (time() > get_session('submit_time') + TIME_LIMIT) {
            //timeout...
            $MSG = 'Timeout!!';
            set_session('correct_cnt', '0');
            set_session('submit_time', '');
        } else {
            set_session('correct_cnt', $correct_cnt);
            $MSG = "Correct Captcha!";
            if ($correct_cnt >= NEED_CORRECT) {
                $MSG = "Congratulation! Flag is ".FLAG;
            }
        }
    } else {
        //incorrect captcha value
        $MSG = "Incorrect captcha.. Plz retry!";
        set_session('correct_cnt', '0');
        set_session('submit_time', '');
    }
} else {
    set_session('correct_cnt', '0');
}

//set new captcha value
set_session('captcha_val', create_new_captcha());
?>
<!doctype html>
<html>
    <head>
        <meta charset='utf-8'>
    </head>
    <body>
        <input type="hidden" name="mp3_url" value="<?= create_mp3() ?>" />
        <h1>Captcha value</h1>

        <h2><?= get_session('correct_cnt') ?>/<?= NEED_CORRECT ?></h2>
        <?php
            if($MSG !== '') {
                echo "<h1>$MSG</h1>";
            }
        ?>
        <button id="mp3_play" onclick="playsound();">Sound Play</button>
        <form method="POST">
            <input type="text" name="captcha_val" autofocus />
            <input type="submit" value="Submit" />
        </form>
        <div id='panel'></div>
        <script src="./captcha.js"></script>
        <!--
            <a href="./index.php?source">Source code</a>
        -->
    </body>
</html>