import json
from channels.generic.websocket import WebsocketConsumer
from chatterbot import ChatBot
from django.conf import settings

class ChatConsumer(WebsocketConsumer):

    chatterbot = ChatBot(**settings.CHATTERBOT)

    def connect(self):
        self.accept()

    def disconnect(self, close_code):
        pass

    def receive(self, text_data):
        text_data_json = json.loads(text_data)
        message = text_data_json['message']
        reply =  self.chatterbot.get_response(message)
        print(reply)

        self.send(text_data=json.dumps({
            'message': message,
            'reply': reply.text
        }))