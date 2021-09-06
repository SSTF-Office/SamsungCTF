var express = require('express');
var session = require('express-session')
var Bag = require('../models/Bag');
var Item = require('../models/Item');
var bagRouter = express.Router();

// Defined Makeit route
bagRouter.route('/').get(function (req, res) {
  if(typeof req.session.uid === "undefined")
      return res.redirect('/login');

  Item.find({uid:req.session.uid}, function (err, itms){
    var bag = new Bag();
    if (req.query.names && req.query.names.length >= 100) {
      req.query.names.length = 100;
    }
    for(var i=0; i < itms.length; i++){ 
	console.log(req.session.username + " - " + itms[i].name + ":" + itms[i].value)
        bag.add(itms[i].name, itms[i].value);
    }
    res.render("bag", {names:req.query.names, bag:bag});
  });
});

module.exports = bagRouter;
