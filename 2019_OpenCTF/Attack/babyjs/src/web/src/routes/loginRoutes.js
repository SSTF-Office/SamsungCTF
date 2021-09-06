var express = require('express');
var app = express();
var loginRouter = express.Router();
var crypto = require('crypto');
var User = require('../models/User');

// Defined register route
loginRouter.route('/').get(function (req, res) {
  res.render('login/login');
});
// Defined login route
loginRouter.route('/post').post(function (req, res) {
  req.body.password = crypto.createHash('sha256').update(req.body.password).digest('base64');
  var user = new User(req.body); 
  
  User.findOne({username:user.username, password:user.password},function (err, users){
    if(err){
      console.log(err);
    }
    else if(users) {
      console.log(users.username);
      req.session.username = users.username;
      req.session.uid = users._id;
      
      res.redirect('/items');
    }
    else{
      res.redirect('/login');
    }
  });
  ;
});

// Defined register route
loginRouter.route('/register').get(function (req, res) {
  res.render('login/register');
});

// Defined register post route
loginRouter.route('/register/post').post(function (req, res) {
  var user = new User(req.body);
  user.password = crypto.createHash('sha256').update(req.body.password).digest('base64');
 
  if(user.username.length <= 4) {
     console.log(user.username);
     return res.render('login/register',{msg:"username length > 4"});// 
  }else {
     User.findOne({username:user.username},function (err, users){
      if(err){
        console.log(err);
      }
      else if(users) {
        console.log(users.username);
        return res.render('login/register',{msg:"Username already exists"});// 
      }
      else{
         user.save()
        .then(user => {
          console.log("Register:", user.username);
          res.redirect('/login');
        })
        .catch(err => {
          res.status(400).send("unable to save to database");
        });
      }
    });
  }
});

module.exports = loginRouter;
