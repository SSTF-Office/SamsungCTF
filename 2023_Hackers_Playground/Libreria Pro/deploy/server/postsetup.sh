#!/bin/bash
pushd ../..
docker-compose exec db psql -U postgres -f /root/post.sql
popd
