var mongoose = require('mongoose');
var conf = require('./config.js');
var UserModel = require('./Models/User.js');
var DiaryModel = require('./Models/Diary.js');

mongoose.Promise = global.Promise;
mongoose.connect(conf.db, {}, err => {
  if (err) {
    console.log(`error to connect db $(err)`);
    process.exit()
  }
});

new UserModel.User({
    userid: conf.admin_id,
    password: conf.admin_pw
})
.save()
.then(savedUser => {
    console.log(savedUser);
})
.catch(err => {
    console.error(err);
    process.exit()
});

new DiaryModel.Diary({
  title: "Todo List for final release",
  date: "03/01/2018 00:00",
  author: "admin",
  body: `This is a check list for final release.
- Add some more functions.
- Change backend server to release mode.
- Currently bot is running on the backend server. Move it to new server.
- Check whether nodejs's version is lastest.
After finish all of above, I can release final version.`,
  ispub: true
}).save()
.then(l => {
  console.log(l);
})
.catch(err => {
  console.error(err);
  process.exit()
});

new DiaryModel.Diary({
  title: "The secret of the fox",
  date: "02/27/2018 00:00",
  author: "admin",
  body: `
  Finally, I found secret of the fox!

  Ring-ding-ding-ding-dingeringeding!
  Gering-ding-ding-ding-dingeringeding!
  Wa-pa-pa-pa-pa-pa-pow!
  Hatee-hatee-hatee-ho!
  Joff-tchoff-tchoffo-tchoffo-tchoff!
  Tchoff-tchoff-tchoffo-tchoffo-tchoff!
  Jacha-chacha-chacha-chow!
  Chacha-chacha-chacha-chow!
  Fraka-kaka-kaka-kaka-kow!
  A-hee-ahee ha-hee!`,
  ispub: true
}).save()
.then(l => {
  console.log(l);
})
.catch(err => {
  console.error(err);
  process.exit()
});

new DiaryModel.Diary({
  title: "My secret",
  date: "03/01/2018 00:00",
  author: "admin",
  body: `I love to hide my secret to many places.
  This is some hints to find them!
  - I stored one of them into my local storage!
  - The another one is located at shell!`,
  ispub: true
}).save()
.then(l => {
  console.log(l);
  process.exit()
})
.catch(err => {
  console.error(err);
  process.exit()
});
