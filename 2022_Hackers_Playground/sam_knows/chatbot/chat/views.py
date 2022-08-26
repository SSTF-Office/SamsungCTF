from django.shortcuts import render

import json
from chatterbot import ChatBot
from chatterbot.trainers import ChatterBotCorpusTrainer
from django.conf import settings
from django.http import JsonResponse, HttpResponse
from django.views import View
from django.utils.decorators import method_decorator
from django.views.decorators.csrf import csrf_exempt


def index(request):
    return render(request, 'index.html')

def room(request, room_name):
    return render(request, 'room.html', {
        'room_name': room_name
    })


# @method_decorator(csrf_exempt, name='dispatch')
# class ChatterBotTrainApiView(View):
#     """
#     Provide an API endpoint to train ChatterBot.
#     """

#     chatterbot = ChatBot(**settings.CHATTERBOT)

#     def train_with_corpus(self):
#         import os 
#         print(os.getcwd())
#         trainer = ChatterBotCorpusTrainer(self.chatterbot)
#         trainer.train(
#             os.getcwd() + '/../chatterbot_corpus/data/english'
#         )

#     def post(self, request, *args, **kwargs):
#         """
#         Request to start run
#         """

#         self.train_with_corpus()

#         return HttpResponse(status=200)
