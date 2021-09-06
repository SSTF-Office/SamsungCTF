const express = require('express')
const app = express()
const deap = require('deap')
const port = 3000

app.use(express.static('public'))

function fromBase(data) {
    return Buffer.from(data, 'base64').toString()
}

function checkIfAdmin(user) {
    let userParsed = deap.merge({}, user)
    if(userParsed.admin) {
        delete userParsed.__proto__.admin
        return true
    } else {
        delete userParsed.__proto__.admin
        return false
    }
}

app.get('/flag/:user', (req, res) => {
    try {
        let user = JSON.parse(fromBase(req.params.user))
        if (user.admin) {
            throw "HACKER"
        }
        if(checkIfAdmin(user)) {
            return res.json({flag: process.env.FLAG})
        }
    } catch(e) {
        return res.json({flag: 'hacker'})
    }
    return res.json({flag: 'no'})
})

if(!process.env.FLAG) {
    console.log("You must set flag as an environmental variable! Exiting.")
    return;
}

app.listen(port, () => console.log(`[*] Running on ${port}!`))
