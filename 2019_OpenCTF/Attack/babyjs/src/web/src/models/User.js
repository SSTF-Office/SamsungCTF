var mongoose = require('mongoose');
var Schema = mongoose.Schema;
mongoose.set('useCreateIndex', true)
// Define collection and schema for User
var User = new Schema({
  username: {
    type: String
  },
  password: {
    type: String
  },
  level: {
    type: String
  },
},{
	collection: 'users'
});

module.exports = mongoose.model('User', User);
