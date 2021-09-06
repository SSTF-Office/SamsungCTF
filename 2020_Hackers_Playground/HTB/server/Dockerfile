FROM ubuntu:bionic
ENV DEBIAN_FRONTEND noninteractive

ADD html/ /var/www/html

RUN apt update
RUN apt list --upgradable
RUN apt update

#Installing the Nginx Web Server
RUN apt install nginx -y

#Installing PHP and Configuring Nginx to Use the PHP Processor
RUN apt install php-fpm php-mysql -y

RUN apt install -y \
  nano \
  python \
  curl \
  git \
  cron \
  wget \
  iputils-ping \
  zip \
  unzip \
  php-curl \
  php-gd \
  php-xml \
  php-mbstring \
  php-soap

RUN apt clean

# Install Composer.
RUN curl -sS https://getcomposer.org/installer | php
RUN mv composer.phar /usr/local/bin/composer

# Install Drush via composer.
RUN composer global require drush/drush:8

# Configure composer bin path for drush inside container and from exec.
RUN echo 'export PATH="$HOME/.composer/vendor/bin:$PATH"' >> /root/.bashrc
ENV PATH /root/.composer/vendor/bin:$PATH

COPY ./config/php-docker.ini /etc/php/7.2/fpm/php.ini
COPY ./config/default-docker.conf /etc/nginx/sites-available/default
RUN sed -i "s/user  nginx;/user  www-data;/g" /etc/nginx/nginx.conf

RUN mkdir /var/run/php

ADD ./config/run.sh /run.sh
RUN chmod 755 /run.sh

# Configure git
RUN mv /var/www/html/_git /var/www/html/.git
#RUN chown -R www-data:www-data /var/www/

# Configure suid
RUN chmod +x /var/www/html/admin/private/rocket/rocket.sh
RUN chmod +x /var/www/html/admin/private/rocket/start
RUN useradd flag
RUN chown flag:flag /var/www/html/admin/private/rocket/flag
USER flag
RUN chmod 400 /var/www/html/admin/private/rocket/flag
USER root
RUN chmod u+s /var/www/html/admin/private/rocket/check

# Configure tmp
RUN chmod -R 733 /tmp/

EXPOSE 80

CMD ["/run.sh"]
