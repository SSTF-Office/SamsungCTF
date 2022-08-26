$(document).ready(function(){
    var video = $("#eduVideo")[0];
    var maxRate = 1.0;
    var started = false;


    function updateVideoInfo(){
        var toHHMMSS = (secs) => {
            var sec_num = parseInt(secs, 10)
            var hours   = Math.floor(sec_num / 3600)
            var minutes = Math.floor(sec_num / 60) % 60
            var seconds = sec_num % 60

            return [hours,minutes,seconds]
                .map(v => v < 10 ? "0" + v : v)
                .filter((v,i) => v !== "00" || i > 0)
                .join(":")
        }
        var cur = toHHMMSS(video.currentTime);
        var dur = toHHMMSS(video.duration);
        var speed = video.playbackRate.toFixed(2);
        if (isNaN(video.duration)){
          $("#videoInfo").text("Your browser is not supported");
        }
        else{
          $("#videoInfo").text(`${cur} / ${dur} (x${speed})`);
        }
    }

    $("#eduVideo").on(
        "timeupdate",
        function(event){
            updateVideoInfo();
        });

    $("#eduVideo").on(
        "ended",
        function(event){
            $.ajax({dataType: "json",
                url: "/status",
                data:JSON.stringify({action: "finish", rate: maxRate}),
                method: "POST",
                contentType: "application/json; charset=utf-8",
            }).done(function(json){
                alert(json.msg);
                document.location='/main';
            });
        });

    $("#btnPlay").click(function(){
        video.play();
        if (!started){
            $.ajax({dataType: "json",
                url: "/status",
                data:JSON.stringify({action: "start"}),
                method: "POST",
                contentType: "application/json; charset=utf-8",
            }).done(function(json){
                started = true;
            });
        }
    });

    $("#btnStop").click(function(){
        video.pause();
    });

    $("#btnSlow").click(function(){
        if (video.playbackRate > 0.5)
            video.playbackRate -= 0.25;
        updateVideoInfo();
    });

    $("#btnFast").click(function(){
        if (video.playbackRate < 1.5)
            video.playbackRate += 0.25;
        updateVideoInfo();
        if (video.playbackRate > maxRate)
            maxRate = video.playbackRate;
    });

    $("#btnCert").click(function(){
        window.open('/cert');
    });
});
