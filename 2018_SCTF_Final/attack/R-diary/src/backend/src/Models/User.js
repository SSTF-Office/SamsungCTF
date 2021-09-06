var mongoose = require('mongoose');

var control = require('../AccessControl.js');
var conf = require('../config.js');
var Schema = mongoose.Schema;
var ObjectId = Schema.ObjectId;

var User = mongoose.model('User', new Schema({
  userid: String,
  password: String
}));

const signin = (req, res) => {
  User.findOne({
    userid: req.body.userid,
    password: req.body.password
  })
  .then(user => {
    if (!user) {
        res.status(401);
        res.send(JSON.stringify({ reason: { html: "Invaild username or password!" }}));
    } else {
        res.status(200);
        control.sign({ id: user.userid })
        .then(tok => {
        res.send(JSON.stringify({
          username: user.userid,
          token: tok
        }))});
    }})
  .catch(err => {
        res.status(500);
        res.send(JSON.stringify({ reason: { html: err } }));
  });
};

const signup = (req, res) => {
  res.status(503);
  res.send(JSON.stringify({ reason: { html: "Signup is Unavailable" } }));
}

module.exports = {
  User,
  signin,
  signup
};
