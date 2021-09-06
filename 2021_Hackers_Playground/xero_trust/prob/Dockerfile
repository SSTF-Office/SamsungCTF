FROM php:7.3.3-apache 
RUN apt-get update && apt-get upgrade -y 
RUN docker-php-ext-install mysqli 
RUN apt-get install xxd

RUN openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/ssl/private/ssl-cert-snakeoil.key -out /etc/ssl/certs/ssl-cert-snakeoil.pem -subj "/CN=localhost"

RUN a2enmod rewrite
RUN a2ensite default-ssl
RUN a2enmod ssl

EXPOSE 443

COPY seed.sh /tmp/seed.sh
CMD bash /tmp/seed.sh
