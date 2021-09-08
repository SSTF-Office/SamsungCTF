var mongoose = require('mongoose');
var Schema = mongoose.Schema;
var ObjectId = Schema.ObjectId;

var Diary = mongoose.model('Diary', new Schema({
  title: String,
  date: Date,
  author: String,
  body: String,
  ispub: Boolean
}));

const listDiary = (req, res) => {
  Diary.find({
    author: req.userid,
    ispub: true
  })
  .then(lst => {
    return lst.map(diary => ({
      id: diary._id,
      name: diary.title
    }));
  })
  .then(lst => {
    res.status(200);
    res.send(JSON.stringify({ data: lst }));
  })
  .catch(err => {
    res.status(401);
    res.send(JSON.stringify({ reason: { html: err } }));
  });
}

const getDiary = (req, res) => {
  const id = req.params.id;
  if (!mongoose.Types.ObjectId.isValid(id)) {
    res.status(404);
    res.send(JSON.stringify({
      reason: { html: `${id}: Not Found` }
    }));
  }
  Diary.findOne({
    _id: id
  })
  .then(diary => {
    if (!diary) {
      res.status(404);
      res.send(JSON.stringify({
        reason: { html: `${id}: Not Found` }
      }));
    } else {
      res.status(200);
      res.send(JSON.stringify({
        data: {
          title: diary.title,
          date: diary.date,
          author: diary.author,
          body: diary.body
        }
      }));
    }
  })
  .catch(err => {
    res.status(500);
    res.send(JSON.stringify({ reason: { html: err } }));
  });
}

const addDiary = (req, res) => {
 if (req.body.ispub === true) {
    res.status(500);
    res.send(JSON.stringify({ reason:
      { html: "Fail to write public diary due to some reason." } }));
 }
 new Diary({
    title: req.body.title,
    date: req.body.date,
    author: req.userid,
    body: req.body.body,
    ispub: req.body.ispub
  })
  .save()
  .then(savedDiary => {
    res.status(200);
    res.send(JSON.stringify({
      msg: { html: "Saved!" },
      id: savedDiary._id
    }));
  })
  .catch(err => {
    res.status(500);
    res.send(JSON.stringify({ reason: { html: err } }));
  })
}

module.exports = {
  Diary,
  listDiary,
  getDiary,
  addDiary
}
