import requests
import re

url_format = 'http://english-e-reader.net/download?link={}&format=txt'
booklist = ['the-bird-of-happiness-and-other-wise-tales-herdon-tim',
             'the-coral-island-r-m-ballantyne']

for i, book in enumerate(booklist):
    r = requests.get(url_format.format(book))
    contents = re.sub(r'(\s+)', ' ', r.text).lstrip().rstrip()

    with open('ebook{}.txt'.format(i), 'wb') as f:
        f.write(contents)
