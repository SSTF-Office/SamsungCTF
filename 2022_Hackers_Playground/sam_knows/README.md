## Sam knows

### Overview

| Type              | Contents         |
|:-----------------:|:-----------------|
| Difficulty        | Easy             |
| Category          | Misc             |
| Tags              | AI               |
| Author            | sw00k.kim        |


### Description
I made a chat bot with the Big(not that big) data based on me!

### Deploy
```bash
$ cd chatbot
$ docker-compose up -d --build
```

### Solving Strategy

1. Do chat with a sam
2. Sam will give some hint while talk
3. Try to get secret from the conversation

### Idea

There are many AI services around. But sometimes sensitive data might included in the data set. Those sensitive data might be leaked.

### Directory Tree
```bash
$ tree
.
├── chatbot
│   ├── chat
│   │   ├── admin.py
│   │   ├── apps.py
│   │   ├── consumers.py
│   │   ├── __init__.py
│   │   ├── migrations
│   │   │   ├── __init__.py
│   │   ├── models.py
│   │   ├── routing.py
│   │   ├── templates
│   │   │   ├── index.html
│   │   │   └── room.html
│   │   ├── tests.py
│   │   ├── urls.py
│   │   └── views.py
│   ├── chatbot
│   │   ├── asgi.py
│   │   ├── __init__.py
│   │   ├── settings.py
│   │   ├── urls.py
│   │   └── wsgi.py
│   ├── db.sqlite3
│   ├── docker-compose.yml
│   ├── Dockerfile
│   ├── manage.py
│   └── README.md
├── README.md
└── train
    ├── db.sqlite3
    ├── main.py
    ├── README.md
    ├── requirements.txt
    └── train_data
        └── english
            ├── ai.yml
            ...

9 directories, 49 files
```