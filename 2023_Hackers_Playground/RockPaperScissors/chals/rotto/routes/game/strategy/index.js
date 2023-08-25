const validator = require("validator");

function escapeAllValues(data) {
  for (const [key, value] of Object.entries(data)) {
    if (typeof(value) === 'string') data[key] = validator.escape(value);
    else if (typeof(value) === 'object') escapeAllValues(value);
  }  
}

module.exports = async function (fastify, opts) {
  // Routes
  fastify.get('/', {
    onRequest: [fastify.authenticate]
  }, async (request, reply) => {
    await reply.sendFile('strategy.html');
  })

  fastify.get('/:strategyId', {
    onRequest: [fastify.authenticate]
  }, async (request, reply) => {
    const {strategyId} = request.params;
    const username = request.user.username;
    const strategies = fastify.mongo.db.collection('strategies');
    const isAdmin = request.user.admin || false;
    let ss = {};
    if (isAdmin) {
      ss = await strategies.findOne({ _id: new fastify.mongo.ObjectId(strategyId) });
    }
    else {
      ss = await strategies.findOne({ username: username, _id: new fastify.mongo.ObjectId(strategyId) });
    }
    await reply.send({strategy: ss});
  })

  fastify.get('/list', {
    onRequest: [fastify.authenticate]
  }, async (request, reply) => {
    const strategies = fastify.mongo.db.collection('strategies');
    const username = request.user.username;
    const ss = await strategies.find({ username: username });
    const alls = await ss.toArray();
    await reply.send({ strategies: alls });
  })
  fastify.post('/',
    {
      onRequest: [fastify.authenticate]
    }, async (request, reply) => {
      let {name, strategy} = JSON.parse(request.body);
      try {
        escapeAllValues(strategy);
        const strategies = fastify.mongo.db.collection('strategies');
        const res = await strategies.insertOne({
          username: request.user.username,
          name: name,
          strategy: strategy
        });
        await reply.send({ message: 'Saved', _id: res.insertedId });
      } catch (e) {
        console.error(e);
        await reply.code(500).send({ message: 'Internal Server Error' });
      }
    }
  )
}