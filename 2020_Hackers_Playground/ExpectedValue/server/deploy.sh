#!/bin/bash

id='expected_value'
external_port=1337
internal_port=1337

echo '0. Setup'
echo '1. Docker: Build an image'
echo '2. Docker: Run a container'
echo '3. Docker: Restart the container'
echo '4. Docker: Clean (remove image)'
echo '5. Docker: shell (/bin/sh)'
echo -n ' > '

read select
echo ""

pushd "${0%/*}"/server > /dev/null

if [ ${select} -eq 0 ]; then
	#should be updated according to each challenge
	echo "Done."
elif [ ${select} -eq 1 ]; then
	sudo docker build -t ${id} .
elif [ ${select} -eq 2 ]; then
	sudo docker run -p ${external_port}:${internal_port} -d --name ${id} ${id}
elif [ ${select} -eq 3 ]; then
	sudo docker restart ${id}
elif [ ${select} -eq 4 ]; then
	sudo docker kill ${id}
	sudo docker rm ${id}
	sudo docker rmi ${id}
elif [ ${select} -eq 5 ]; then
	sudo docker exec -it ${id} /bin/sh
fi

echo ""

popd > /dev/null
