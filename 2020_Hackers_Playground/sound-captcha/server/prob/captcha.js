function playsound() {
    let mp3_url = "";

    let hidden = document.querySelector('input[name="mp3_url"]');
    if (hidden) {
        mp3_url = hidden.value;
    } else {
        return;
    }
    let html5use = false;
    let html5audio = document.createElement('audio');

    //if html5 audio is available
    if (html5audio.canPlayType && html5audio.canPlayType("audio/mpeg")) {
        let wav = new Audio(mp3_url);
        wav.id = "mp3_audio";
        wav.autoplay = true;
        wav.controls = false;
        wav.autobuffer = false;
        wav.loop = false;
        let old = document.getElementById('mp3_audio');
        if (old) {
            old.remove();
        }
        document.getElementById('panel').appendChild(wav);
        html5use = true;
    }
    
    //if html5 is not available.
    if (!html5use) {
        let object = document.createElement('object');
        object.id = 'mp3_object';
        object.classid = 'classid="clsid:22d6f312-b0f6-11d0-94ab-0080c74c7e95';
        object.height = '0';
        object.width = '0';
        object.style = 'width:0; height:0;'
        let param1 = document.createElement('param');
        param1.name = 'AutoStart';
        param1.value = '1';
        

        let param2 = document.createElement('param');
        param2.name = 'Volume';
        param2.value = '0';

        let param3 = document.createElement('param');
        param3.value = 'PlayCount';
        param3.name = '1';

        let param4 = document.createElement('param');
        param4.name = 'FileName';
        param4.value = mp3_url;

        object.appendChild(param1);
        object.appendChild(param2);
        object.appendChild(param3);
        object.appendChild(param4);

        let embed = document.createElement('embed');
        embed.id = 'mp3_embed';
        embed.src = mp3_url;
        embed.autoplay = "true";
        embed.hidden = 'true';
        embed.volume = '100';
        embed.type = 'audio/x-wav';
        embed.style = 'display:inline;'

        object.appendChild(embed);
        let mp3_object = document.getElementById('mp3_object');
        if (mp3_object) {
            mp3_object.remove();
        }
        document.getElementById('panel').appendChild(object);
    }
    
}