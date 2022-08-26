from flask import Flask, request, jsonify

app = Flask(__name__)
HoF = [[9000,'can_you@beat.me']]


@app.route('/hof', methods=['POST'])
def hof():
    global HoF
    if len(HoF) > 100:
        # not too many!
        HoF = HoF[:10]
    params = request.get_json()
    score = int(params['score'])
    email = params['email']
    i = 0
    while i < len(HoF):
        if HoF[i][0] < score:
            break
        i += 1
    HoF.insert(i, [score, email])
    result = {'rank': i+1}
    return jsonify(result)

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=9988, debug=False)
