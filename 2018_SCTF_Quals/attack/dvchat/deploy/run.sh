docker build -t dvchat_image .
docker rm -f dvchat
docker run -d -p 7779:7779 --name dvchat dvchat_image
