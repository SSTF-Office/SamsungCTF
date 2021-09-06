var express = require('express');
var mongoose = require('mongoose');
var bodyParser = require('body-parser')

var conf = require('./config.js');
var UserModel = require('./Models/User.js');
var DiaryModel = require('./Models/Diary.js');
var control = require('./AccessControl.js');
var inspector = require('./inspect0r.js');

mongoose.Promise = global.Promise;
mongoose.connect(conf.db, {}, err => {
  if (err) {
    console.log(`error to connect db $(err)`);
  }
});

var app = express();

// Access Control from frontend
app.all('/*', (req, res, next) => {
  res.header('Access-Control-Allow-Origin' , conf.frontend );
  res.header('Access-Control-Allow-Headers',
    'Origin, X-Requested-With, Content-Type, Accept, x-access-token');
  next();
});

app.use( bodyParser.json() );
app.use( bodyParser.urlencoded({ extended: true }) );

// User Views
app.post('/signin', UserModel.signin);
app.post('/signup', UserModel.signup);

// Diary Views
app.get('/diaries', control.verify, DiaryModel.listDiary);
app.get('/diaries/:id', control.verify, DiaryModel.getDiary);
app.post('/diaries', control.verify, DiaryModel.addDiary);

// Inspect Views
app.post('/inspect/:id', control.verify, inspector.inspect);

var server = app.listen(3001, () => {
  console.log('listening on port 3001!');
});
