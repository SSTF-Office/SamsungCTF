from PIL import Image, ImageDraw, ImageFont
from random import random, choice

def generate(string, path):
    length = len(string)
    arr = [choice([1, 2, 3])]
    for i in xrange(length):
        ch = [1,2,3]
        ch.remove(arr[-1])
        arr.append(choice(ch))

    size = (70 + length * 40, 150)
    font = ImageFont.truetype('font.ttf', 100)
    RI = Image.new('RGBA', size, "#ffffff")
    GI = Image.new('RGBA', size, "#ffffff")
    BI = Image.new('RGBA', size, "#ffffff")

    act = [ImageDraw.Draw(RI), ImageDraw.Draw(GI), ImageDraw.Draw(BI)]
    for i in xrange(length):
        offset = (35 * i + 20 * random(), 25 + 20 * random() - 20)
        color = arr[i] - 1
        act[color].text(offset, string[i], font = font, fill = "#%06x"%(0xff << (16 - 8 * color)))

    Image1 = Image.blend(RI, GI, 2.0/3)
    Image2 = Image.blend(RI, BI, 2.0/3)
    _image = Image.blend(Image1, Image2, 0.5)
    _image.save(path+'.png','png', dpi=(600,600))
    return string

def gen(length, path):
    integers = '0123456789'
    newsz = int(length/2.0 + 0.5)

    op = choice(['+', '-', 'x'])
    arg1 = ''.join(choice(integers) for _ in xrange(newsz))
    arg2 = ''.join(choice(integers) for _ in xrange(newsz))

    generate(arg1 + op + arg2, path)

    if op == '+':
        res = int(arg1) + int(arg2)
    elif op == '-':
        res = int(arg1) - int(arg2)
    elif op == 'x':
        res = int(arg1) * int(arg2)
    return str(res)

