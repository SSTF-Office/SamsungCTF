
const express = require('express');
const cookieParser = require('cookie-parser');
const path = require('path');
const app = express();
const PORT = 3000;

app.use(cookieParser());
app.set('views', path.join(__dirname, "view"));
app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    let rawJwt = req.cookies.jwt || {};

    try {
        let jwtPart = rawJwt.split('.');

        let jwtHeader = jwtPart[0];
        jwtHeader = Buffer.from(jwtHeader, "base64").toString('utf8');
        jwtHeader = JSON.parse(jwtHeader);
        jwtHeader = JSON.stringify(jwtHeader, null, 4);
        rawJwt = {
            header: jwtHeader
        }

        let jwtBody = jwtPart[1];
        jwtBody = Buffer.from(jwtBody, "base64").toString('utf8');
        jwtBody = JSON.parse(jwtBody);
        jwtBody = JSON.stringify(jwtBody, null, 4);
        rawJwt.body = jwtBody;

        let jwtSignature = jwtPart[2];
        rawJwt.signature = jwtSignature;

    } catch(error) {
        if (typeof rawJwt === 'object') {
            rawJwt.error = error;
        } else {
            rawJwt = {
                error: error
            };
        }
    }
    res.render('index', rawJwt);
});

app.use(function(err, req, res, next) {
    console.error(err.stack);
    res.status(500).send('Something wrong!');
});

app.listen(PORT, (err) => {
    console.log(`Server is Running on Port ${PORT}`);
});