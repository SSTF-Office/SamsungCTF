'use strict'

const path = require('path')
const AutoLoad = require('@fastify/autoload')

// Pass --options via CLI arguments in command to enable these options.
module.exports.options = {
}

const MONGODB_HOST = process.env.MONGODB_HOST || 'localhost:27017';
const DATABASE = process.env.DATABASE || 'game';
const JWT_SECRET = process.env.JWT_SECRET || 'jwt_secret';

module.exports = async function (fastify, opts) {
  // Place here your custom code!

  // Do not touch the following lines

  // This loads all plugins defined in plugins
  // those should be support plugins that are reused
  // through your application
  fastify.register(AutoLoad, {
    dir: path.join(__dirname, 'plugins'),
    options: Object.assign({}, opts)
  })

  // This loads all plugins defined in routes
  // define your routes in one of these
  fastify.register(AutoLoad, {
    dir: path.join(__dirname, 'routes'),
    options: Object.assign({}, opts)
  })

  fastify.register(require('@fastify/static'), {
    root: path.join(__dirname, 'public'),
    prefix: '/public/'
  })

  fastify.register(require('@fastify/cookie'), {
    hook: 'onRequest'
  })

  fastify.register(require('@fastify/mongodb'), {
    forceClose: true,
    url: `mongodb://${MONGODB_HOST}/${DATABASE}`
  })

  fastify.register(require('@fastify/jwt'), {
    secret: JWT_SECRET,
    cookie : {
      cookieName: 'token'
    },
    sign: {
      expiresIn: '10m'
    }
  })

  fastify.decorate('authenticate', async (request, reply) => {
    try {
      await request.jwtVerify();
    } catch (err) {
      reply.code(401).send({message: 'Unauthorized'});
    }
  })
}
