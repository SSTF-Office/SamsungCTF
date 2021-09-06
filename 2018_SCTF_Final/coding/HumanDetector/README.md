# Human Detector

## Description
Are you a hooooman or a bot?
Only bots can pass this test.
Prove that your are one of the certified bots.

- the detector uses background image and font: [background](), [font]()
- charset: A-Z (ascii_uppercase)
- part of the leaked detector code:

```python
class Captcha():
    def __init__ (self, **opts):
        self.font_path = opts['font_path']
        self.bg_path = opts['bg_path']
        self.rotate = opts['rotate']
        self.font_size = opts['font_size']
        self.bg_offset = opts['bg_offset']

# ...................................
# ...................................
# ...................................

    def generate(self, captcha_str, o_fname, captcha_size=(60, 100)):

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

# ...................................
# ...................................
# ...................................
```

## Challenge setup
```/bin/sh
$ ./setup.sh
$ ./docker-compose build
$ ./docker-compose up
```

## Solve

### Training Dataset Generation
First, you should generate training dataset with the given background image and
font. If you first access the server and gets the captcha image, you can
recognize the rough size of the original image. From this, you can generate
training dataset.


### Preprocessing
Since raw images are hard to be recognized by the model, you should preprocess
the images first. By recognizing the color of the text is black, you can
transfer all the other pixels to white.


### Training
By googling, you can find many captch solving models.
For example, https://github.com/JackonYang/captcha-tensorflow can be utilized.
You can train your model slightly modifying the codes.

### Solve
The server gives a captcha image with 100 characters. You can preprocess the
given captcha as in the preprocessing step (convert an image to black and white
one), predict the labels using your model, and send the answer to the server.

