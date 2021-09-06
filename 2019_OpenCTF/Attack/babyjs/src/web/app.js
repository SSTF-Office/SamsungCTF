//1
var bodyParser = require('body-parser');
var express = require('express');
var session = require('express-session') 
var FileStore = require('session-file-store')(session);
var app = express();
var port = 30002;
// Session
app.use(session({
  store: new FileStore({}),
  secret: 'HelI@MyFr!3nDzzzzzzzzz',
  resave: false,
  saveUninitialized: true
}))


// Mongoose connection with mongodb
var mongoose = require('mongoose')
var mongoUrl = "mongodb://mongo:27017/test"
mongoose.Promise = require('bluebird');

var connectWithRetry = function() {
  return mongoose.connect(mongoUrl, function(err) {
    if (err) {
      console.error('Failed to connect to mongo on startup - retrying in 5 sec', err);
      setTimeout(connectWithRetry, 5000);
    }
  });
};
connectWithRetry();

// Required application specific custom router module
var itemRouter = null;
var loginRouter = null;

// Use middlewares to set view engine and post json data to the server

app.set('view engine', 'ejs');
app.use(express.static('public'));
app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());
app.use('/login', require('./src/routes/loginRoutes'));
app.use('/items', require('./src/routes/itemRoutes'));
app.use('/items/bag', require('./src/routes/bagRoutes'));

//Catch Express error
app.use(function(err, req, res, next) {
  console.error("<<<<" + err + ">>>>");
  res.set('Content-Type', 'text/plain')
  res.status(500).send("Internal Error..");
});

//Catch javascript runtime error
process.on('uncaughtException', function(err) {
  console.log("Caught exception: ", err);
});

// Define home route
app.get('/', function (req, res) {
  res.redirect('/items');
});

// Start the server
app.listen(port, function(){
  console.log('Server is running on Port: ',port);
});
