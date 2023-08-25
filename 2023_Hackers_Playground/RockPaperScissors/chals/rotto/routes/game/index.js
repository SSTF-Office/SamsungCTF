// Parent Move class
class Move {
  constructor() { }

  winsAgainst(move) {
    throw new Error('winsAgainst method must be implemented');
  }
}

// Rock class
class Rock extends Move {
  constructor() {
    super();
  }

  winsAgainst(move) {
    return move instanceof Scissors;
  }
}

// Paper class
class Paper extends Move {
  constructor() {
    super();
  }

  winsAgainst(move) {
    return move instanceof Rock;
  }
}

// Scissors class
class Scissors extends Move {
  constructor() {
    super();
  }

  winsAgainst(move) {
    return move instanceof Paper;
  }
}

const GameResult = {
  win: 'You win!',
  lose: 'You lose!',
  draw: "It's a tie!"
};

// Game class
class Game {
  constructor() {
    this.scores = {
      player: 0,
      computer: 0,
    };
  }

  playRound(playerMove) {
    const moves = {
      rock: Rock,
      paper: Paper,
      scissors: Scissors,
    };

    const PlayerMoveClass = moves[playerMove.toLowerCase()];
    const ComputerMoveClass = moves[getRandomMove()];

    const playerMoveInstance = new PlayerMoveClass();
    const computerMoveInstance = new ComputerMoveClass();

    if (playerMoveInstance.winsAgainst(computerMoveInstance)) {
      this.scores.player++;
      return GameResult.win;
    }

    if (computerMoveInstance.winsAgainst(playerMoveInstance)) {
      this.scores.computer++;
      return GameResult.lose;
    }

    return GameResult.draw;
  }

  getScores() {
    return this.scores;
  }

  resetScores() {
    this.scores = {
      player: 0,
      computer: 0,
    };
  }
}

var crypto = require("crypto");

function getRandomInt(maxNumber = 3) {
  let rand = crypto.randomBytes(1).toString('hex');
  let randInt = parseInt(rand, 16);
  return randInt % maxNumber;
}

function getRandomMove() {
  const moves = ['rock', 'paper', 'scissors'];
  return moves[getRandomInt(3)];
}

const game = new Game();

module.exports = async function (fastify, opts) {
  fastify.get('/',
    {
      onRequest: [fastify.authenticate]
    },
    (request, reply) => {
      reply.sendFile('game.html');
    })

  fastify.get('/play',
    {
      onRequest: [fastify.authenticate]
    }, (request, reply) => {
      const { move } = request.query;

      const result = game.playRound(move);
      reply.send({ result });
    });

  fastify.post('/play/',
    {
      onRequest: [fastify.authenticate]
    }, (request, reply) => {
      const moves = JSON.parse(request.body);
      let results = [];
      for (let move of moves) {
        let result = game.playRound(move.move);
        results.push(result);
      }
      reply.send({ results });
    });

  fastify.get('/score',
    {
      onRequest: [fastify.authenticate]
    }, (request, reply) => {
      const scores = game.getScores();
      reply.send({ scores });
    });

  fastify.get('/reset',
    {
      onRequest: [fastify.authenticate]
    }, (request, reply) => {
      game.resetScores();
      reply.send({ message: 'Scores reset!' });
    });

  fastify.get('/play-strategy',
    {
      onRequest: [fastify.authenticate]
    }, (request, reply) => {
      reply.sendFile('play.html');
    })
}