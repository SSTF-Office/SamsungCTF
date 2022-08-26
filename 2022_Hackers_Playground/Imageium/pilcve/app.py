#! /usr/bin/python3
from flask import Flask, send_file, redirect, url_for, request
from PIL import Image, ImageMath
from PIL import __version__ as pv
from io import BytesIO
from urllib.parse import quote_plus as qp
import os, random

app = Flask(__name__)
app._static_folder = ''

modes = {
    "" : '<option value="" {}> </option>',
    "r" : '<option value="r" {}>R</option>',
    "g" : '<option value="g" {}>G</option>',
    "b" : '<option value="b" {}>B</option>',
    "r%2Bg" : '<option value="r%2Bg" {}>R + G</option>',
    "r%2Bb" : '<option value="r%2Bb" {}>R + B</option>',
    "g%2Bb" : '<option value="g%2Bb" {}>G + B</option>',
    "r%2Bg%2Bb" : '<option value="r%2Bg%2Bb" {}>R + G + B</option>',
    "r%26g" : '<option value="r%26g" {}>R & G</option>',
    "r%26b" : '<option value="r%26b" {}>R & B</option>',
    "g%26b" : '<option value="g%26b" {}>G & B</option>',
    "r%26g%26b" : '<option value="r%26g%26b" {}>R & G & B</option>',
    "r%5Eg" : '<option value="r%5Eg" {}>R ^ G</option>',
    "r%5Eb" : '<option value="r%5Eb" {}>R ^ B</option>',
    "g%5Eb" : '<option value="g%5Eb" {}>G ^ B</option>',
    "r%5Eg%5Eb" : '<option value="r%5Eg%5Eb" {}>R ^ G ^ B</option>',
    "r%7Cg" : '<option value="r%7Cg" {}>R | G</option>',
    "r%7Cb" : '<option value="r%7Cb" {}>R | B</option>',
    "g%7Cb" : '<option value="g%7Cb" {}>G | B</option>',
    "r%7Cg%7Cb" : '<option value="r%7Cg%7Cb" {}>R | G | B</option>'
}


@app.route("/", methods=["GET", "POST"])
def index():
    mode = request.form.get("modes", "")

    return """
<html>
 <body>
  <h3>Safe channel mixer</h3>
  <form action="#" method='POST'>
    <label for="lang">Select mode:</label>
    <select name="modes" id="idmode">
      """ + "\n      ".join([v.format('selected="1"' if mode==k else "") for k,v in modes.items()]) + """
    </select>
    <input type="submit" value="Submit" />
  </form>
  <img src="static/original.jpeg" alt="original"/>
  <img src="dynamic/modified?mode={}" alt="modified image"/>
  <h5>powered by Pillow {}</h6>
 </body>
</html>
""".format(mode, pv)


badlist = ["sleep", "time", "eval", "complile", "passwd"]
errors = ["invalid syntax (<string>, line 1)",
        "not image object: None",
        "unexpected EOF while parsing (<string>, line 1)",
        "source code string cannot contain null bytes",
        "EOL while scanning string literal (<string>, line 1)",
        "source code string cannot contain null bytes",
        "invalid character in identifier (<string>, line 1)"
        "name 'null' is not defined"]


@app.route("/dynamic/modified", methods=['GET'])
def modified():
    mode = request.args.get('mode', '')

    img = Image.open("./original.jpeg")
    buf = BytesIO()

    try:
        if mode:
            for w in badlist:
                if w in mode:
                    raise Exception(random.choice(errors))
            else:
                r,g,b = img.split()
                img = ImageMath.eval(mode, r=r, g=g, b=b)
                assert type(img) == Image.Image, "not image object: {}".format(img)
                img = img.convert("RGB")

        img.save(buf, "jpeg")
        buf.seek(0)
        return send_file(buf, attachment_filename='modified.jpeg', mimetype='image/jpeg')

    except Exception as e:
        r = """ -------------------------------------\n ("{}")\n ImageMath error {}\n""".format(mode, e)
        app.logger.error(r)
        return(r)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80)

