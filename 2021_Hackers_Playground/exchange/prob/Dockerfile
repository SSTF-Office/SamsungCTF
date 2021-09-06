FROM php:7.3.3-apache 
RUN apt-get update && apt-get upgrade -y 
RUN apt-get install xxd

RUN a2enmod rewrite

EXPOSE 80

COPY seed.sh /tmp/seed.sh
CMD bash /tmp/seed.sh
