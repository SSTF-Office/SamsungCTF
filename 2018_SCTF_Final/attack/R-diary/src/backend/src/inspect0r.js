var mongoose = require('mongoose');
var validUrl = require('valid-url');
var Url = require('url');
var Schema = mongoose.Schema;
var ObjectId = Schema.ObjectId;

var Inspect = mongoose.model('Inspect', new Schema({
  url: String,
  timeout: Number,
  done: Boolean
}));

const inspect = (req, res) => {
  const url = decodeURIComponent(req.url.slice(9));
  const req_time = parseInt(req.body.time, 10);

  console.log(url);
  if (!validUrl.isUri(url)){
    res.status(500);
    res.send(JSON.stringify({ reason: { html: "Not a valid url!" } }));
    return;
  } else if (Url.parse(url).hostname !== "localhost") {
    res.status(500);
    res.send(JSON.stringify({ reason: { html: "Not a valid url!" } }));
    return;
  } else if (Url.parse(url).port !== "3000") {
    res.status(500);
    res.send(JSON.stringify({ reason: { html: "Not a valid url!" } }));
    return;
  } else if (req_time === NaN) {
    res.status(500);
    res.send(JSON.stringify({ reason: { html: "Not a valid duration" } }));
    return;
  } else if (req_time > 60 * 10) {
    res.status(500);
    res.send(JSON.stringify({ reason: { html: "Too long" } }));
    return;
  }

  new Inspect({
    url: url,
    timeout: req_time,
    done: false
  }).save()
  .then(url => {
    console.log("new -> " + url);
    res.status(200);
    res.send(JSON.stringify({
      msg: {html: "My bot will inspect that diary!" },
    }));
  })
  .catch(err => {
    res.status(500);
    res.send(JSON.stringify({ reason: { html: err } }));
  })
}

module.exports = {
  inspect
}
