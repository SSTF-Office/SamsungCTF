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
        w_min = 10000
        w_max = 0
        h_min = 10000
        h_max = 0

        # load background image
        bg_img = cv2.imread(self.bg_path)
        #out_img = np.zeros(bg_img.shape, np.uint8)
        out_img = np.copy(bg_img)

        prev_h, prev_w = self.bg_offset
        for idx, char in enumerate(captcha_str):
            # load font
            font = ImageFont.truetype(self.font_path, self.font_size)

            # create txt image and put char
            char_height = int(self.font_size * 1.2)
            char_width = int(self.font_size * 1.2)
            char_size = (char_height, char_width)
            char_offset = (0, 0)

            new_img = np.zeros((char_height, char_width, 3), np.uint8)
            txt_img = Image.fromarray(new_img, 'RGB')
            txt_draw= ImageDraw.Draw(txt_img)
            txt_draw.text(char_offset, char, font=font, fill=(255, 255, 255))

            if self.rotate:
                txt_img = txt_img.rotate(self.rotate,
                                         expand=True).resize(char_size)

            txt_img = np.fromstring(txt_img.tobytes(), dtype=np.uint8)
            txt_img = txt_img.reshape((char_height, char_width, 3))

            # get rectangle that fits char and cut txt image
            hs, ws = np.where(txt_img[:,:,0] > 30)
            h_s = hs.min()
            h_e = hs.max()
            w_s = ws.min()
            w_e = ws.max()
            txt_img = txt_img[h_s:h_e, w_s:w_e, :]

            # put txt image to background
            bg_offset = [prev_h, prev_w]

            prev_w = bg_offset[1] + w_e

            h_min = min(h_min, bg_offset[0])
            h_max = max(h_max, bg_offset[0] + h_e)
            w_min = min(w_min, bg_offset[1])
            w_max = max(w_max, bg_offset[1] + w_e)

            # Get dominating color in the background where the captcha text will be written
            # the text will be printed on the background at the given background offset and
            # the number of colors in the text pixels will be counted.
            # The dominating color will be replaced by 'black'
            max_color = self.get_max_color(bg_img, txt_img, bg_offset, 1)
            to_color = (0, 0, 0)
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

        assert(np.any(txt_img > 100))

        tt = bg_img[h_s:h_e, w_s:w_e, :][txt_img > 100].view()
        tt.shape = -1,3
        ifl = tt[..., 0].astype(np.int)*256*256 \
            + tt[..., 1].astype(np.int)*256 \
            + tt[..., 2].astype(np.int)
        colors, inv = np.unique(ifl, return_inverse=True)

        zus = np.array([colors[-1]+1])
        colplus = np.hstack((colors, zus))
        ccnt = np.histogram(ifl, colplus)[0]
        ccnt = ccnt.argsort()

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

        def _distance(a, b):
            return np.linalg.norm(a-b)

        max_color, to_color = color
        xs, ys = np.where(txt_img[:,:,0] > 200)
        for i in range(xs.shape[0]):
            h, w = h_s + xs[i], w_s + ys[i]
            pixel = bg_img[h, w, :]

            if _distance(pixel, max_color) < threshold:
                out_img[h, w, :] = to_color

        return out_img


