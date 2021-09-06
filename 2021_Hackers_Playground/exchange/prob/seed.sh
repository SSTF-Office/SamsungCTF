(echo "<?php"; echo "define('KEY', '`xxd -l 16 -p /dev/urandom`');"; echo "define('IV', '`xxd -l 16 -p /dev/urandom`');") > /var/www/salt.php
apache2-foreground
