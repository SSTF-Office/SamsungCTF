(echo "<?php"; echo "define('SALT', '`xxd -l 16 -p /dev/urandom`');") > /var/www/salt.php
apache2-foreground
