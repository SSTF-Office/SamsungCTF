var codearea = document.getElementById('codearea');

document.getElementById('runcode').addEventListener('click', (e)=>{
    fetch('/code', {
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        method: 'post',
        body: 'code=' + encodeURIComponent(codearea.value)
        
    })
    .then(res => res.json())
    .then(json => {
        if (!Array.isArray(json)) {
            if (json.error) {
                return alert(json.error);
            } else {
                return alert(JSON.stringify(json));
            }
        }
        try {
            var total = json.length;
            var correct = 0;
            for (var i=0; i< total; i++) {
                correct += json[i] === true ? 1 : 0;
            }
            return alert(`You passed ${correct} Test Cases of ${total}`);
        } catch(e) {
            alert(res);
        }
    })
    .catch(error => alert("ERR:", error));
    e.stopPropagation();
    e.preventDefault();
    return false;
})