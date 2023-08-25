'use strict'

var net = require('net');

const BOT_PORT = process.env.BOT_PORT || '1338';
const BOT_HOST = process.env.BOT_HOST || 'bot';

module.exports = async function (fastify, opts) {

  fastify.post('/',
    {
      onRequest: [fastify.authenticate]
    }, async (request, reply) => {
      const { url } = JSON.parse(request.body);
      if (!url.startsWith('/game/play-strategy?strategy_id=')) {
        reply.code(400).send({ message: 'Not a valid url' });
      }
      else {
        try {
          const client = net.Socket();
          client.connect(BOT_PORT, BOT_HOST);
          client.on('data', data => {
            let recv = data.toString().trim();
            if (recv == "Please send me a URL to open.") {
              client.write(`http://rotto:3001${url}` + '\n');
              client.destroy();
            }
            console.log(recv);
          });
          reply.send({ message: 'ok' });
        } catch (e) {
          reply.code(500).send({ message: 'bot has problem' });
        }
      }
    })
};