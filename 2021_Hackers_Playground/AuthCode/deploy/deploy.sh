#!/bin/bash

id='authcode'
external_port=1337
internal_port=1337

echo '0. Setup'
echo '1. Docker: Build an image'
echo '2. Docker: Run a container'
echo '3. Docker: Run a container (detached)'
echo '4. Docker: Restart the container'
echo '5. Docker: Clean (remove image)'
echo '6. Docker: shell (/bin/sh)'
echo -n ' > '

read select
echo ""

pushd "${0%/*}"/server > /dev/null

if [ ${select} -eq 0 ]; then
	#should be updated according to each challenge
	pushd ../../src
	make
	popd
	echo "Done."
elif [ ${select} -eq 1 ]; then
	docker build -t ${id} .
elif [ ${select} -eq 2 ]; then
	docker run -p ${external_port}:${internal_port} --name ${id} ${id}
elif [ ${select} -eq 3 ]; then
	echo docker run -p ${external_port}:${internal_port} -d --name ${id} ${id}
	docker run -p ${external_port}:${internal_port} -d --name ${id} ${id}
elif [ ${select} -eq 4 ]; then
	docker restart ${id}
elif [ ${select} -eq 5 ]; then
	docker kill ${id}
	docker rm ${id}
	docker rmi ${id}
elif [ ${select} -eq 6 ]; then
	docker exec -it ${id} /bin/sh
fi

echo ""

popd > /dev/null
