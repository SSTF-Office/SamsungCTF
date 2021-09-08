docker build -t crashcollector_image .
docker rm -f crashcollector
docker run -d -p 7778:7778 --name crashcollector crashcollector_image
