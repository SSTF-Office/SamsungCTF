FROM php:8.2-apache

RUN apt update
RUN apt install -y libpq-dev
RUN pecl install apcu
RUN docker-php-ext-configure pgsql -with-pgsql=/usr/local/pgsql 
RUN docker-php-ext-install pgsql pdo_pgsql
RUN docker-php-ext-enable apcu pdo_pgsql

EXPOSE 80
