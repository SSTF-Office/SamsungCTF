docker build -t noleak_user .
docker rm -f noleak
docker run -d -p 7777:7777 --name noleak noleak_user
