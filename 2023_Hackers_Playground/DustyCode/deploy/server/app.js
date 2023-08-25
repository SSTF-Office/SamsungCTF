const Hapi = require('@hapi/hapi');
const Joi = require('@hapi/joi');
const regex = require('email-regex')({exact: true});
const { exec } = require("child_process");

const server = Hapi.server({
    port: 3000,
    host: '0.0.0.0'
});

const API_KEY = "binary_glibberish";

server.route({
    method: 'GET',
    path: '/zebra',
    handler: async (request, h) => {
        const { email } = request.query;
        const foo = () => {
            return new Promise((resolve, reject) => {
                decrypt(email, (err, plainEmail) => {
                    if (err) {
                        reject(err);
                    } else {
                        resolve(plainEmail);
                    }
                })
            });
        };
        const resp = await foo();
        return h.response(resp);
    },
    options: {
        validate: {
            query: Joi.object({
                apiKey: Joi.string().equal(API_KEY).required(),
                email: Joi.string().email().required()
            })
        }
    }
})

function decrypt(encryptedStr, callback) {
    const regex = new RegExp(`^[^\\.\\s@:](?:[^\\s@:]*[^\\s@:\\.\`])?@[^\\.\\s@]+(?:\\.[^\\.\\s@]+)*$`);
    if (encryptedStr && !regex.test(encryptedStr)) {
        try {
            exec('java -cp bin/decryptor.jar com.Main ' + encryptedStr, function(err, stdout, stderr) {
                if (stdout) {
                    let values = stdout.split(':');
                    return callback(null, values[1]);
                } else {
                    return callback(null, encryptedStr);
                }
            })
        } catch(e) {
            return callback(null, encryptedStr);
        }
    } else {
        return callback(null, encryptedStr);
    }
}

(async () => {
    try {
        await server.start();
        console.log(`Server running at: ${server.info.uri}`);
    } catch (error) {
        console.log(error);
        process.exit(1);
    }
})();