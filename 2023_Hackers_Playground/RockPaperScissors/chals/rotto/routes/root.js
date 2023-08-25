'use strict'

module.exports = async function (fastify, opts) {
  fastify.get('/', async function (request, reply) {
    try{
      await request.jwtVerify();
      await reply.redirect('/game');
    } catch(e) {
      console.error(e);
    }
    return reply.sendFile('index.html');
  })
}
