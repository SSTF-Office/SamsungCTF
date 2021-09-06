#!/bin/sh

sudo apt-get update && sudo apt-get install rng-tools -y

KEYRING_ADMIN=" --options ./admin_gpg.conf"
KEYRING_USER=" --options ./gpg.conf"
ADMIN_PASSPHRASE="YOUCANNOTFINDTHISPASSPHRASEITSVERYLONGRIGHTHUH"
FLAG="SCTF{b4by_PwN_w1th_crit1Cal_memOry_l3ak}"

cat >admin_gpg_key.script << EOF
%echo Generating a basic OpenPGP key
Key-Type: 1
Key-Length: 2048
Name-Real: admin
Name-Comment: account for sctf 2018
Name-Email: admin@2018.eatpwnnosleep.com
Expire-Date: 0
Passphrase: ${ADMIN_PASSPHRASE}
%commit
%echo done
EOF

cat >flag_sender_gpg_key.script << EOF
%echo Generating a basic OpenPGP key
Key-Type: 1
Key-Length: 2048
Name-Real: flagsender
Name-Comment: account for sctf 2018
Name-Email: flagsender@2018.eatpwnnosleep.com
Expire-Date: 0
Passphrase: ${ADMIN_PASSPHRASE}
%commit
%echo done
EOF

rm -rf "./gnupg"
mkdir -p "./gnupg"
gpg ${KEYRING_USER} --batch --gen-key admin_gpg_key.script 2>&1
gpg ${KEYRING_USER} --batch --gen-key flag_sender_gpg_key.script 2>&1
gpg --list-keys --options gpg.conf
rm -rf *.script

echo "${FLAG}" > flag
(echo "${ADMIN_PASSPHRASE}") | gpg ${KEYRING_USER} --default-key "flagsender@2018.eatpwnnosleep.com" --passphrase-fd 0 --output flag_to_admin.enc --batch --yes -se -r "admin@2018.eatpwnnosleep.com" --no-random-seed-file flag
(echo "${ADMIN_PASSPHRASE}") | gpg ${KEYRING_USER} --default-key "admin@2018.eatpwnnosleep.com" --passphrase-fd 0 --no-random-seed-file --decrypt flag_to_admin.enc

# home directory setup
sudo rm -rf src
sudo mkdir -p src
sudo chown root:root src
sudo ln -sf /dev/null src/.bash_history
sudo cp ../prob/leakit src/leakit
sudo chown root:root src/leakit
sudo chmod 755 src/leakit
sudo cp flag_to_admin.enc src

# database setup
sudo rm -rf src/database
sudo mkdir -p src/database
sudo chown root:root src/database
sudo chmod 733 src/database

sudo rm -rf src/.gnupg
sudo cp -R gnupg src/.gnupg
sudo chown 1000:1000 -R src/.gnupg
sudo chmod 700 src/.gnupg
#sudo cp gpg.conf src/gpg.conf
#sudo chmod 644 src/gpg.conf

sudo rm -rf src/.admin_gnupg
sudo cp -R gnupg src/.admin_gnupg
sudo chown root: -R src/.admin_gnupg
sudo chmod 755 src/.admin_gnupg
sudo chmod 644 src/.admin_gnupg/*
sudo cp admin_gpg.conf src/admin_gpg.conf
sudo chmod 644 src/admin_gpg.conf

# add script to crontab
PWD=$(pwd)
echo "*/10 * * * * ${PWD}/copybot.sh ${PWD}" > ./copybot.cron

sudo crontab < ./copybot.cron

