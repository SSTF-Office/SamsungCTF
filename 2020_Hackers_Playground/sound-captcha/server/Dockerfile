FROM php:7.3.20-apache
WORKDIR /var/www/html
COPY prob /var/www/html
ENV APACHE_RUN_USER=www-data \
    APACHE_RUN_GROUP=www-data \
    APACHE_LOG_DIR=/var/log/apache2
RUN chown -R www-data:www-data /var/www/html
COPY apache-conf.txt /tmp
RUN cat /tmp/apache-conf.txt >> /etc/apache2/apache2.conf
RUN rm /tmp/apache-conf.txt
EXPOSE 80
CMD ["apache2-foreground"]