#!/bin/bash
docker exec -it sso /opt/keycloak/bin/kc.sh export --realm sctf --file /tmp/sctf_realm.json
docker cp sso:/tmp/sctf_realm.json .

echo "extracted.. -> ./sctf_realm.json"
