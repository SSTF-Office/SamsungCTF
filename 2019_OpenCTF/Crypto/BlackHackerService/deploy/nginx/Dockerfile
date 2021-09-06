FROM nginx:latest

RUN rm /etc/nginx/conf.d/default.conf && rm /etc/nginx/nginx.conf
COPY nginx.conf /etc/nginx/
COPY sites-enabled/app /etc/nginx/conf.d/default.conf
