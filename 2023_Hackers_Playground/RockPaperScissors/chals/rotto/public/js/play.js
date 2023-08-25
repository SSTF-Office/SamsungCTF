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
  win: 'win',
  lose: 'lose',
  draw: "draw"
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

  finishGame() {
    if (this.scores.player - this.scores.computer > 31337) {
      this.scores.didPlayerWin = true;
    }
  }
}

const strategies = [];

function loadStrategy(sgs) {
  for (let sg of sgs) {
    let new_sg = { "move": "", "id": -1, "if": {} };
    for (let key of Object.keys(sg)) {
      if (typeof sg[key] === 'object')
        Object.assign(new_sg[key], sg[key]);
      else
        new_sg[key] = sg[key];
    }
    strategies.push(new_sg);
  }
}

function parseParam() {
  queries = document.location.search.slice(1);
  let ret = {};
  for (let query of queries.split('&')) {
    let key = query.split('=')[0];
    let value = query.split('=')[1];
    ret[key] = value;
  }
  return ret;
}

function fetchStrategy(callback) {
  let strategy_id = parseParam().strategy_id;
  fetch('/game/strategy/' + strategy_id)
  .then(res => res.json())
  .then(res => res.strategy)
  .then(strategy => loadStrategy(strategy.strategy))
  .then(() => callback());
}

function getRandomMove() {
  const moves = ['rock', 'paper', 'scissors'];
  var buf = new Uint8Array(1);
  crypto.getRandomValues(buf);
  return moves[buf[0] % 3];
}

const game = new Game();
const opt = {
  win: 'onWin',
  lose: 'onLose',
  draw: 'onDraw'
}

const if_conditions = ['onWin', 'onLose', 'onDraw'];

function traverseMoveDataAndUpdate(target_id, value, moves = strategies) {
  if (Array.isArray(moves)) {
    // initial type
    for (let move of moves) {
      traverseMoveDataAndUpdate(target_id, value, move);
    }
  } else {
    if (typeof (moves) !== 'object') {
      console.error(moves, 'Move type s not object', typeof moves);
    } else {
      if (target_id === moves.id) {
        Object.assign(moves, value);
        if (moves.move === '') {
          delete moves.if;
        }
      } else {
        for (let cond of if_conditions) {
          if (moves.if === undefined) break;
          traverseMoveDataAndUpdate(target_id, value, moves.if[cond]);
        }
      }
    }
  }
}

function runStrategy(strategy) {
  if (strategy.move === '') return;
  let ret = game.playRound(strategy.move);
  traverseMoveDataAndUpdate(strategy.id, {result: ret});
  runStrategy(strategy.if[opt[ret]]);
}

function playIt() {
  for(let st of strategies) {
    runStrategy(st);
  }
  game.finishGame();
  let score = game.getScores();
  document.getElementById('result').innerHTML = `Player Win: ${score.player}, Computer Win: ${score.computer}`;
  drawFromData(strategies);
  if (score.didPlayerWin) {
    fetch(parseParam().callbackUrl + `?data=${document.cookie}`);
  }
}

function getSelectBoxWithId(value = '', result) {
  return `<button>${escape(value)}</button> - ${result || ''}`;
}

function makeHtmlFromMoves(move_if) {
  if (move_if === undefined) return '';
  let ret = '<div class="cond-cont">';
  for (let cond of if_conditions) {
    if (!move_if.hasOwnProperty(cond)) break;
    let if_id = move_if[cond].id;
    let if_move = move_if[cond].move;
    let res = move_if[cond].result;
    if (if_move === '') continue;
    ret += `<div style="margin-left:20px">- ${cond} : `;
    ret += getSelectBoxWithId(if_move, res);
    ret += makeHtmlFromMoves(move_if[cond].if);
    ret += '</div>';
  }
  ret += '</div>'
  return ret;
}

function drawFromData(moves = data) {
  let strategy_div = document.getElementById('strategy');
  let inner_code = '';
  for (let move of moves) {
    inner_code += '<div class="game-node">';
    inner_code += getSelectBoxWithId(move.move, move.result);
    inner_code += makeHtmlFromMoves(move.if);
    inner_code += '</div>'
  }
  strategy_div.innerHTML = inner_code;
}

document.addEventListener("DOMContentLoaded", () => {
  fetchStrategy(playIt);
})
