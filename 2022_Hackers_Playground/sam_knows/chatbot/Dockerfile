FROM python:3.7

RUN pip3 install --upgrade setuptools
RUN pip3 install django chatterbot channels chatterbot-corpus spacy==2.3.5
RUN python -m spacy download en

WORKDIR /usr/src/app

COPY . .

ENV PYTHONUNBUFFERED=0

CMD ["python3", "manage.py", "runserver", "0.0.0.0:8001"]

EXPOSE 8001
