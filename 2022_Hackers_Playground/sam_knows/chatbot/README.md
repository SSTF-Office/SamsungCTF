# Chatbot Service

## How to run
```
$ docker-compose up
```

## How to change corpus data
- Train new conversation data with train directory
- Change ```db.sqlite3``` file with new one
- Remove previous docker container
    ```
    $ docker rm [container-id]
    ```
- Run the docker again