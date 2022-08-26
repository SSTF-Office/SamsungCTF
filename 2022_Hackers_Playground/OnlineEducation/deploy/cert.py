import time
import pdfkit

template = """
<html>
<head>
<style>
.outer-border{{
    width:800px; height:500px; padding:20px; text-align:center; border: 10px solid #ffb300;
}}
.inner-dotted-border{{
    width:750px; height:450px; padding:20px; text-align:center; border: 5px solid #ffb300;border-style: dotted;
}}
.certification{{
    font-size:50px; font-weight:bold;    color: #ffb300;
}}
.certify{{
    font-size:25px;
}}
.name{{
    font-size:30px;    color: green;
}}
.fs-30{{
    font-size:30px;
}}
.fs-20{{
    font-size:20px;
}}
</style>
</head>
<body>
<div class="outer-border">
<div class="inner-dotted-border">
       <span class="certification">Certificate of Completion</span>
       <br><br>
       <span class="certify"><i>This is to certify that</i></span>
       <br><br>
       <span class="name"><b>{}</b></span><br/>
       <span class="fs-20">({})</span><br/><br/>
       <span class="certify"><i>has successfully completed the course</i></span> <br/><br/>
       <span class="fs-30">{}</span> <br/>
       <span class="certify"><i>by</i></span> <br/>
       <span class="certify">{}</span> <br/><br/>
       <span class="certify"><i>dated</i></span><br>
      <span class="fs-30">{}</span>
</div>
</div>
</body>
</html>
"""


def make_certificate(name, email, course, author):
    date = time.strftime('%d, %b, %Y', time.localtime())
    html = template.format(name, email, course, author, date)
    options = {'orientation': 'landscape', 'page-size': 'B6'}
    # Cool! HTML to PDF
    pdf = pdfkit.from_string(html, options=options)
    return pdf

