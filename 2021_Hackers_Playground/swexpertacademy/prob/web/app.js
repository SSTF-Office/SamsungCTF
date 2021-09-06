const createError = require('http-errors');
const http = require('http');
const express = require('express');
const path = require('path');
const fs = require('fs');

const app = express();
const executorUrl = process.env.EXECURL || 'http://localhost:3001';

let filter = (code) => {
  return code.match(/include|system|exec|open|fscanf|read|fread|fget|getc|asm|pragma|#/);
};

let getResponse = (code, callback) => {
  let fullCode = `#include <stdio.h>
  int main() {
    ${code}
  }`;
  const req = http.request(executorUrl, {
    method: 'POST',
    timeout: 9000
  }, (res) => {
    let data = '';
    res.on('data', (chunk) => {
      data += chunk;
    });
    res.on('end', ()=> {
      callback(data);
    });
  });
  req.on('error', (err) => {
    callback(err);
  });
  req.write(fullCode);
  req.end();
};

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', function(req, res) {
  res.render('index', {title: 'SW Expert Academy'});
});

app.post('/code', function(req, res) {
  let code = req.body.code;
  let banned = filter(code);
  if (banned) {
    res.setHeader('Content-Type', "application/json");
    res.write(JSON.stringify({
        "error": `Dangerous keyword (${banned}) is not allowed`
    }));
    res.end();
  } else {
    getResponse(code, (stdout) => {
      res.send(stdout);
    });
  }
})

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
