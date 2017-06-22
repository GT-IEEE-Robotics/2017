function button_down(index) {
    $.ajax({
        type: "POST",
        url: "192.168.7.2:5000/command",
        data: {"i": parseInt(index), "d": parseInt($("input#dutycycle").val())},
        error: function() { alert("Error!") },
        dataType: "json" 
    });
}
function button_up(index) {
    $.ajax({
        type: "POST",
        url: "192.168.7.2:5000/release",
        data: null,
        error: function() { alert("Error!") },
        dataType: "json" 
    });

}
