all: help
.PHONY: build run clean stop help

NAME := escape
PORT := 5051

build:
	@docker build --tag $(NAME):latest .

run:
	@docker run -d --name $(NAME) --rm -p $(PORT):5051 $(NAME):latest

stop:
	@docker stop $(NAME)

clean:
	@docker rm -f $(NAME) || true
	@docker rmi -f $(NAME) || true

help:
	@echo "Available targets: build, run, stop, clean"
