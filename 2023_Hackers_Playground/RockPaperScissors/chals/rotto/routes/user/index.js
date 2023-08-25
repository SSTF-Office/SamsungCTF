'use strict'
var crypto = require("crypto");

function make_hash(data) {
  return crypto.createHash('sha256').update(data).digest('hex');
}

const DOMAIN = process.env.PRODUCTION ? process.env.DOMAIN : '.' + process.env.DOMAIN.slice(0, process.env.DOMAIN.indexOf(':'));

module.exports = async function (fastify, opts) {

  fastify.get('/signup', async (request, reply) => {
    try {
      await request.jwtVerify();
      await reply.redirect('/game');
    } catch (e) {
      console.error(e);
    }
    await reply.sendFile('signup.html');
  })

  fastify.post('/signup', async (request, reply) => {
    const { username, password, password2 } = JSON.parse(request.body);
    if (password !== password2) {
      await reply.status(400).send({ message: 'Password not match' });
    } else {
      const users = fastify.mongo.db.collection('users');
      let existUser = await users.findOne({ username: username });
      if (existUser) {
        await reply.status(409).send({ message: 'Username exists' });
      } else {
        const res = await users.insertOne({
          username: username,
          password: make_hash(password)
        });
        const token = await reply.jwtSign({
          username: username
        });
        reply.setCookie('token', token, {
          domain: DOMAIN || 'localhost',
          path: '/',
          sameSite: true,
          httpOnly: true
        }).code(200).send({ message: 'ok' });
      }
    }
  })

  fastify.get('/signin', async (request, reply) => {
    try {
      await request.jwtVerify();
      await reply.redirect('/game');
    } catch (e) {
      console.error(e);
    };
    await reply.sendFile('signin.html');
  })

  fastify.post('/signin', async (request, reply) => {
    const { username, password } = JSON.parse(request.body);
    const users = fastify.mongo.db.collection('users');
    if (typeof(username) !== 'string' || typeof(password) !== 'string'){
      return reply.code(400).send({ message: "Username, password should be string."});
    }
    const hashedpw = make_hash(password);
    console.log(username, hashedpw);
    let user = await users.findOne({
      username: username,
      password: hashedpw
    });
    console.log(user);
    if (user) {
      const token = await reply.jwtSign({ username: username });
      const refreshToken = await reply.jwtSign({ username: username }, {expiresIn: '1d'});
      reply.setCookie('token', refreshToken, {
        domain: DOMAIN || 'localhost',
        path: '/',
        sameSite: true,
        httpOnly: true
      })
      .code(200).send({ message: 'ok' });
    }
    return reply.code(401).send({ message: "User/password not found" });
  })

  fastify.get('/logout', (request, reply) => {
    reply.setCookie('token', '', {
      domain: DOMAIN || 'localhost',
      path: '/',
      sameSite: true
    }).redirect('/user/signin');
  })

  fastify.get('/status', {
    onRequest: [fastify.authenticate]
  }, (request, reply) => {
    reply.send({message: request.user.username});
  })
}
