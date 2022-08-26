## Online Education

### Overview

| Type              | Contents            |
|:-----------------:|:--------------------|
| Difficulty        | Medium              |
| Category          | Web                 |
| Tags              | Web, XSS            |
| Author            | hahah               |

### Description
 > I made an online education service!
 > Watching education videos is so boring :(

### Deployment
 * config.py and flag files should be redacted

### Solving strategy
 This is an online education service with three videos.
 Users should watch the videos, and they can select playrate from 0.5 to 1.5.
 After completing the education courses, users can receive a certificate PDF file.

 The goal of this challenge is to make an admin session and visit "/flag" by exploiting following bugs.

 First, we can skip videos by sending "rate" value as a negative number.
 As the third video is extremely long (~25 hours), we should skip it to solve the challenge.

 Second, the certificate PDF generator (pdfkit) uses a headless browser to render an HTML page and save it to a PDF file.
 As we can inject javascript code in the email address (bypassing email check using new-line),
 we can trigger XSS in the headless browser.

 Then, we can read any local files using XmlHttpRequest.
 Thus, we can leak flask secret key by reading `/home/app/config.py` file via XSS.
 Finally, we can make a session cookie with "is\_admin = True".


