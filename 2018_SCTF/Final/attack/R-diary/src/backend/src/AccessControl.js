var jwt = require('jsonwebtoken-as-promised');
var conf = require('./config.js');

const sign = (data) => jwt.sign(data, conf.secret);

const verify = (req, res, next) => {
  var token = req.headers['x-access-token'];
  if (!token) {
    res.status(401);
    res.send(JSON.stringify({ reason: { html: 'Unauthentified user' }}));
    return;
  }
  jwt.verify(token, conf.secret, { typ: 'JWT' })
  .then(dec => {
    req.userid = dec.id;
    next();
  })
  .catch(err => {
      res.status(401);
      res.send(JSON.stringify({ reason: { html: 'Fail to verify user' }}));
  });
}

module.exports = {
  verify,
  sign
}
