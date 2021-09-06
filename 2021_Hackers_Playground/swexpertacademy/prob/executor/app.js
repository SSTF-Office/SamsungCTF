const http = require('http');
const async = require('async');
const os  = require('os');
const path = require('path');
const fs = require('fs');
const { exec } = require('child_process');

const printables = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
const randStr = (len) => {
    let ret = '';
    while (len--) {
        ret += printables[parseInt(Math.random() * printables.length)];
    }
    return ret;
}

const PORT = process.env.PORT || 3000;
const TMPFILENAMELEN = 10;
const TC_SET = [0,1,2];
http.createServer((req, res) => {
    if (req.url === '/') {
        let code = '';
        req.on('data', chunk => {
            code += chunk;
        })
        req.on('end', ()=> {
            console.log('Submitted code"""', code, '""""');
            let filePath = path.join(os.tmpdir(), randStr(TMPFILENAMELEN));
            let codeFilePath = `${filePath}.c`;
            let execFilePath = `${filePath}.bin`;
            let outFilePath = `${filePath}.out`;
            async.waterfall([callback => {
                fs.writeFile(codeFilePath, code, callback);
            },
            callback => {
                console.log("Compile!");
                exec(`gcc -o ${execFilePath} ${codeFilePath} -std=c99`, {
                    timeout: 3000
                }, callback);
            },
            (stdout, stderr, callback) => {
                console.log("Execute!");
                async.map(TC_SET, (index, callback)=>{
                    console.log("index", index);
                    exec(`bash -c ${execFilePath} < /server/input${index}.txt`, {
                        timeout: 3000,
                        encoding: 'utf8'
                    }, (err, stdout, stderr) => {
                        if (err && err.signal && err.signal == "SIGTERM") {
                            callback("Time Limit Exceed");
                        } else {
                            callback(err, stdout, stderr);
                        }
                    });
                }, callback);
            },
            (output, callback) => {
                console.log('Stdout """', output, '"""');
                async.map(TC_SET, (index, callback)=> {
                    fs.readFile(`/server/output${index}.txt`, 'utf8', (err, answer)=> {
                        callback(err, output[index].trim() === answer.trim());
                    });
                }, callback);
            },
            (correctData, callback) => {
                res.setHeader('Content-Type', "application/json");
                res.write(JSON.stringify(correctData));
                res.end();
                callback(null);
            }
            ], err => {
                if (err) {
                    console.log('Err', err);
                    res.setHeader('Content-Type', "application/json");
                    res.write(JSON.stringify({
                        "error": err.toString().replace(/\/usr\/[\S]+\/ld: /g, '')
                    }));
                    res.end();
                } else {
                    console.log("No errror!");
                }
                fs.unlink(codeFilePath, ()=>{});
                fs.unlink(execFilePath, ()=>{});
                fs.unlink(outFilePath, ()=>{});
            });
        });
    } else {
        res.statusCode = 404;
        res.end("Not Found");
    }

}).listen(PORT, ()=> {
    console.log(`Server is running on ${PORT}. Internal Access(inter docker communication) is only available.`);
});