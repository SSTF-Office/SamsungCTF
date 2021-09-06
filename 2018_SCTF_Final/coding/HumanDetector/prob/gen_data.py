import argparse
import cv2
import os
import string
import random
from captcha import Captcha


def gen_helper(arg):
    global font_path
    global bg_path
    global o_dir
    global captcha_size
    char, char_idx = arg

    rotate = random.randint(-15, 15)
    font_size = random.randint(700, 800)
    bg_offset = (random.randint(0, 400), random.randint(0, 1000))

    c = Captcha(
        font_path=font_path,
        bg_path=bg_path,
        rotate=rotate,
        font_size=font_size,
        bg_offset=bg_offset
    )

    o_fname = os.path.join(o_dir, "{0}_{1}-{2}".format(char,
                                                       str(bg_offset),
                                                       char_idx))
    c.generate(char, o_fname=o_fname, captcha_size=captcha_size)


def gen_data(o_dir_prefix, charset, char_num):
    global o_dir
    data = [(char, char_idx) for char in charset for char_idx in range(char_num)]

    o_dir = os.path.join(o_dir_prefix)
    if not os.path.exists(o_dir):
        os.makedirs(o_dir)

    pool_size = cpu_count()
    chunk_size = len(data) // pool_size + 1
    os.system('taskset -cp 0-%d %s > /dev/null' % (pool_size, os.getpid()))

    pool = Pool(pool_size)
    with Pool(pool_size) as pool:
        aa = list(pool.imap_unordered(gen_helper, data, chunk_size))


if __name__ == "__main__":
    from multiprocessing import cpu_count, Pool
    parser = argparse.ArgumentParser()
    parser.add_argument('--data_dir', type=str, default='',
                        help='Directory for storing input data')
    parser.add_argument('--char_num', type=int, default=100,
                        help='Directory for storing input data')
    FLAGS, unparsed = parser.parse_known_args()

    bg_path = "prob/backgrounds/background.png"
    font_path = "prob/fonts/font.ttf"
    root_dir = FLAGS.data_dir
    char_num = FLAGS.char_num

    charset = string.ascii_uppercase
    captcha_size = (60, 100)

    gen_data(root_dir, charset, FLAGS.char_num)

