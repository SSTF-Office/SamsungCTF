var strategy_id = 0;
var data = [];
var mode = 'single';
var current_strategy_id = -1;
const if_conditions = ['onWin', 'onLose', 'onDraw'];

function clearSG(clearData = false) {
  document.getElementById('strategy').innerHTML = '';
  if (clearData) {
    data = [];
    document.getElementById('strategy-name').value = '';
  }
}

function playMulti() {
  fetch('/game/play/', { method: 'POST', body: JSON.stringify(data) })
    .then(res => res.json())
    .then(ret => {
      loadScore();
    })
}

function saveStrategy() {
  let name = document.getElementById('strategy-name').value;
  if (data.length === 0 || name.length === 0) alert('No data or name to save');
  else {
    let body = {name: name, strategy: data};
    fetch('/game/strategy', { method: 'POST', body: JSON.stringify(body) })
      .then(res => res.json())
      .then(ret => {
        alert(ret.message);
        clearSG(true);
        changeMode('list-strategy');
        loadStrategy(ret._id);
      });
    }
}

function clearResult() {
  document.getElementById('result').innerHTML = '';
}

function loadScore() {
  fetch('/game/score').then(res => res.json()).then(ret => {
    document.getElementById('score').innerHTML = `
                    Player: ${ret.scores.player}, Computer: ${ret.scores.computer}
                `
  })
}

function showContaier(containers) {
  const cands = ['strategy-cont', 'save-cont', 'run-cont', 'turn-cont', 'strategy-name-cont'];
  for (let cand of cands) {
    if (containers.includes(cand)) {
      document.getElementById(cand).classList.remove('hide');
    } else {
      document.getElementById(cand).classList.add('hide');
    }
  }
}

function loadStrategy(strategy_id) {
  clearSG();
  current_strategy_id = strategy_id;
  fetch('/game/strategy/' + strategy_id)
    .then(res => res.json())
    .then(res => res.strategy)
    .then(res => drawFromData(res.strategy))
    .catch(err => alert(err.message));
}

function listStrategies(strategies) {
  let st_cont = document.getElementById('strategy');
  let code = "<ul>";
  for (let strategy of strategies) {
    code += `<li class="strategy-list"><a onclick="loadStrategy('${strategy._id}')">${strategy.name}</a></li>`;
  }
  code += "</ul>";
  st_cont.innerHTML = code;
}

function initListStrategy() {
  fetch('/game/strategy/list')
    .then(res => res.json())
    .then(res => listStrategies(res.strategies));
}

function clearAll() {
  clearResult();
  clearSG();
}

function changeMode(e) {
  if (e === 'list-strategy') { // New strategy
    showContaier(['strategy-cont', 'run-cont']);
    if (mode !== e) clearAll();
    initListStrategy();
  } else if (e === 'new-strategy') {
    showContaier(['strategy-cont', 'save-cont', 'turn-cont', 'strategy-name-cont']);
    if (mode !== e) {
      clearAll();
      mode = e;
      drawFromData();
    }
  } else {
    showContaier(['turn-cont']);
  }
  mode = e;
}

function handleResult(result) {
  let result_div = document.getElementById('result');
  result_div.innerHTML = `<div class="res">${result}</div>`;
  loadScore();
}

function onConditionalSelectBox(e) {
  let move_id = parseInt(e.id.split('-')[2], 10);
  const value = { move: e.value }
  let _cond_cont = e.parentNode.querySelector('.cond-cont');
  if (_cond_cont) {
    traverseMoveDataAndUpdate(move_id, value);
    drawFromData();
    return;
  }
  value.if = {
    onWin: {
      id: strategy_id++,
      move: ''
    },
    onLose: {
      id: strategy_id++,
      move: ''
    },
    onDraw: {
      id: strategy_id++,
      move: ''
    }
  };
  traverseMoveDataAndUpdate(move_id, value);
  drawFromData();
  return;
}

function traverseMoveDataAndUpdate(target_id, value, moves = data) {
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



function onButtonClick(e) {
  if (mode === 'single') {
    fetch(`/game/play?move=${e}`).then(res => res.json()).then(ret => handleResult(ret.result));
  } else if (mode === 'new-strategy') {
    data.push({
      move: e,
      id: strategy_id++,
      if: {
        onWin: {
          id: strategy_id++,
          move: ''
        },
        onLose: {
          id: strategy_id++,
          move: ''
        },
        onDraw: {
          id: strategy_id++,
          move: ''
        }
      }
    });
    drawFromData();
  }
}

function getSelectBoxWithId(if_type = '', value = '', strategy_id) {
  let isList = mode === 'list-strategy' ? 'disabled' : '';
  if (isList) return `<button>${value}</button>`;
  let selectBox = `<select id="str-${if_type}-${strategy_id}" value="${value}" onchange="onConditionalSelectBox(this)">` +
    '<option value="">Select</option>' +
    `<option ${value === 'rock' ? "selected" : ""} value="rock">rock</option>` +
    `<option ${value === 'paper' ? "selected" : ""} value="paper">paper</option>` +
    `<option ${value === 'scissors' ? "selected" : ""} value="scissors">scissors</option>` +
    '</select>';
  // strategy_id += 1;
  return selectBox;
}

function makeHtmlFromMoves(move_if) {
  if (move_if === undefined) return '';
  let ret = '<div class="cond-cont">';
  for (let cond of if_conditions) {
    if (!move_if.hasOwnProperty(cond)) break;
    let if_id = move_if[cond].id;
    let if_move = move_if[cond].move;
    if (if_move === '' && mode === 'list-strategy') continue;
    ret += `<div style="margin-left:20px">- ${cond} : `;
    ret += getSelectBoxWithId(cond, if_move, if_id);
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
    inner_code += getSelectBoxWithId('init', move.move, move.id);
    inner_code += makeHtmlFromMoves(move.if);
    inner_code += '</div>'
  }
  strategy_div.innerHTML = inner_code;
}

function onStrategyButtonClick(e) {
  document.getElementById(e).classList.toggle('selected');
}

function testStrategy() {
  location.href = `/game/play-strategy?strategy_id=${current_strategy_id}`;
}

function sendToBot() {
  fetch('/bot', {
    method: 'POST',
    body: JSON.stringify({url: `/game/play-strategy?strategy_id=${current_strategy_id}`}),
    redirect: 'follow'
  }).then(res => res.json())
  .then(res => alert(res.message))
  .catch(e => {
    console.log(e);
  });
}


document.addEventListener("DOMContentLoaded", () => {
  fetch('/user/status')
    .then(res => res.json())
    .then(res => document.getElementById('gnb').innerHTML = "Hello, " + res.message);
  loadScore();
})