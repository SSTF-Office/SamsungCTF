var express = require('express');
var session = require('express-session')
var app = express();
var itemRouter = express.Router();

// Require Item model in our routes module
var Item = require('../models/Item');

// Defined get data(index or listing) route
itemRouter.route('/').get(function (req, res) {
  if(typeof req.session.uid === "undefined")
    return res.redirect('/login');
  
  Item.find({uid:req.session.uid}, function (err, itms){
    if(err){
      console.log(err);
    }
    else {
      res.render('main', {itms: itms, session:req.session});
    }
  });
});
// Defined store route
itemRouter.route('/add/post').post(function (req, res) {
  if(typeof req.session.uid === "undefined")
    return res.redirect('/login');

  var item = new Item(req.body);
  item.uid = req.session.uid;
  item.save()
    .then(item => {
    res.redirect('/items');
    })
    .catch(err => {
    res.status(400).send("unable to save to database");
    });
});

// Defined delete | remove | destroy route
itemRouter.route('/delete/:id').get(function (req, res) {
  if(typeof req.session.uid === "undefined")
    return res.redirect('/login');

  Item.findByIdAndRemove({uid:req.session.uid, _id: req.params.id},
	   function(err, item){
		if(err) res.json(err);
		else res.redirect('/items');
	});
});

module.exports = itemRouter;