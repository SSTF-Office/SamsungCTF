#!/bin/bash

DEPLOY_DIR="$1/"
SRC_DIR="$1/src/"

# database setup
rm -rf "${SRC_DIR}/database"
mkdir -p "${SRC_DIR}/database"
chown root:root "${SRC_DIR}/database"
chmod 733 "${SRC_DIR}/database"

rm -rf "${SRC_DIR}/.gnupg"
cp -R "${DEPLOY_DIR}/gnupg" "${SRC_DIR}/.gnupg"
chown 1000:1000 -R "${SRC_DIR}/.gnupg"
chmod 700 "${SRC_DIR}/.gnupg"

