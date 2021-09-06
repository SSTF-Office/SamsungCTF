import os, string
import random
import numpy as np
import cv2
from PIL import Image, ImageFont, ImageDraw

class Captcha():
    def __init__ (self, **opts):
        self.font_path = opts['font_path']
        self.bg_path = opts['bg_path']
        self.rotate = opts['rotate']
        self.font_size = opts['font_size']
        self.bg_offset = opts['bg_offset']


    def generate(self, captcha_str, o_fname, captcha_size=(60, 100)):
        # load background image
        bg_img = cv2.imread(self.bg_path)
        #out_img = np.zeros(bg_img.shape, np.uint8)
        out_img = np.copy(bg_img)

        prev_h, prev_w = self.bg_offset

# ...................................
# ...................................
# ...................................

            # Get dominating color in the background where the captcha text will be written
            # the text will be printed on the background at the given background offset and
            # the number of colors in the text pixels will be counted.
            # The dominating color will be replaced by 'black'
            max_color = self.get_max_color(bg_img, txt_img, bg_offset, 1)
            to_color = (0, 0, 0) # black
            self.put_text(out_img, bg_img, txt_img, bg_offset, (max_color, to_color))

        out_img = out_img[h_min:h_max,w_min:w_max]
        out_img = cv2.resize(out_img, captcha_size,
                             interpolation=cv2.INTER_CUBIC)

        cv2.imwrite(o_fname + ".png", out_img)



    def get_max_color(self, bg_img, txt_img, offset, n=1):
        h_s, w_s = offset
        h_e = min(h_s + txt_img.shape[0], bg_img.shape[0])
        w_e = min(w_s + txt_img.shape[1], bg_img.shape[1])
        txt_img = txt_img[:h_e-h_s, :w_e-w_s, :]

# ...................................
# ...................................
# ...................................

        def _to_tuple(rgb):
            b = rgb & 0xff
            g = (rgb >> 8) & 0xff
            r = (rgb >> 16) & 0xff
            return (r, g, b)

        max_color = _to_tuple(colors[ccnt[-n]])

        return max_color


    def put_text(self, out_img, bg_img, txt_img, offset, color):
        threshold = 30
        h_s, w_s = offset
        h_e = min(h_s + txt_img.shape[0], bg_img.shape[0])
        w_e = min(w_s + txt_img.shape[1], bg_img.shape[1])
        txt_img = txt_img[:h_e-h_s, :w_e-w_s, :]

# ...................................
# ...................................
# ...................................

        return out_img

