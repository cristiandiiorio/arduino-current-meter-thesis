#official Nginx image as a base
FROM nginx:latest

#static website files into the container
COPY ./webpage /usr/share/nginx/html

#expose port 80
EXPOSE 80
