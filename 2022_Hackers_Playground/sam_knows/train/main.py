from chatterbot import ChatBot
from chatterbot.trainers import ListTrainer, ChatterBotCorpusTrainer
import re
import timeit
import argparse

global chatbotCore

def main():
    global chatbotCore
    while True:
        q = input('Q: ')
        start = timeit.default_timer()
        res = chatbotCore.get_response(q)
        end = timeit.default_timer()
        print(res)
        print("Response Duration: ", end - start)


def train_with_corpus():
    global chatbotCore
    trainer = ChatterBotCorpusTrainer(chatbotCore)
    trainer.train(
        'train_data.english'
    )
    # trainer.export_for_training('./export_corpus.yml')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Sam knows it')
    parser.add_argument('--train', action='store_true',
                        help='Train with data')
    args = parser.parse_args()
    chatbotCore = ChatBot('Sam2',
                        logic_adapters=[
                            {
                                'import_path': 'chatterbot.logic.BestMatch',
                                'default_response': ['I am sorry, but I do not understand.', 'What do you do on weekend?',
                                                    'I wish I can eat something', 'Do you have some time on weekend?'],
                                'maximum_similarity_threshold': 0.85,
                                'read_only': True
                            }
                        ])
    if args.train:
        start = timeit.default_timer()
        train_with_corpus()
        stop = timeit.default_timer()
        print("Learn Time: ", stop - start)
    else:
        main()
