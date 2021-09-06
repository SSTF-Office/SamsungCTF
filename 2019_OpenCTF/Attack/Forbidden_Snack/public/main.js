$(document).ready(function() {
    let name = ""
    $("#welcome").click(function() {
        $(this).replaceWith($("#name"))
        $("#name").toggleClass("fadein")
    })
    $("#next").click(function() {
        name = $("#name_field").val()
        $("#greet").text(`So tell me ${name}, do you want to buy a flag?`)
        $("#name").replaceWith($("#flag"))
        $("#flag").toggleClass("fadein")
    })
    $("#giveMeFlag").click(function() {
        $.ajax({
            type: "GET",
            url: `/flag/${btoa(JSON.stringify({username: name}))}`,
            success: parseFlag,
            dataType: "json"
        })
    })
    $("#nah").click(function() {
        $("#flag").replaceWith($("#res"))
        $('#res_text').text('KBYE!')
        $("#res").toggleClass("fadein")
    })
});

function showResult(text) {
    $("#flag").replaceWith($("#res"))
    $("#res_text").text(text)
    $("#res").toggleClass("fadein")
}

var counter = 0;

function hint(v) {
    $("#hint").text(v)
}

function parseFlag(data) {
    switch(data.flag) {
        case "no":
            showResult('Hey man you are not an admin!')
            hint("Btw check this out, cool song https://youtu.be/Qq4j1LtCdww?t=55")
            break
        case "hacker":
            showResult('HACKER DETECTED!')
            break
        default:
            showResult(data.flag)
    }
}