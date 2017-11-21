function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

function startGame() {
    var id = $("#input_game").val();
    if (id == 0) {
        var options = $('#input_game').children('option')
        id = options[getRandomInt(1, options.length)].value;
    }
    $.ajax({
        url: URL_START_GAME.replace('0', id),
        success: function (result) {
            Notification.show(Notification.TYPE.INFO, 'Iniciando el nivel!');
            STARTING = true;
        }
    });
}

function restartGame() {
    $.ajax({
        url: URL_RESTART_GAME,
        success: function (result) {
            Notification.show(Notification.TYPE.WARNING, 'Reiniciando el nivel!');
            RESTARTING = true;
        }
    });
}

function endGame() {
    $.ajax({
        url: URL_END_GAME,
        success: function (result) {
            Notification.show(Notification.TYPE.WARNING, 'Finalizando el nivel!');
            FINISHING = true;
        }
    });
}

$("#button_start").click(startGame);
$("#button_restart").click(restartGame);
$("#button_end").click(endGame);

function setTime(time) {
    var m = Math.floor(time / 60);
    var s = time - (m * 60);
    var time_s = m + ':' + (s < 10 ? '0' + s : s);
    $("#input_text").val(time_s);
}

function setGameView(active) {
    if (active) {
        $("#game_on").hide();
        $("#game_off").show();
        $("#table_game_div").show();
    } else {
        $("#game_on").show();
        $("#game_off").hide();
        $("#table_game_div").hide();
    }
}

var LAST_CHECK_DATE = null
function reloadGameSelect() {
    var time = new Date();
    if (LAST_CHECK_DATE == null || (time - LAST_CHECK_DATE) > 30*1000) {
        LAST_CHECK_DATE = time;
        $.ajax({
            url: URL_GAMES_GET_ALL,
            success: function (result) {
                var games = result
                $("#input_game").empty();
                $("#input_game").append(new Option('- Aleatorio -', 0, true, true));
                games.forEach(function (game) {
                    $("#input_game").append(new Option(game.fields.name, game.pk, false, false));
                });
            }
        });
    }
}

var LAST_GAME_ID = null
function reloadGameTable(game) {
    if (game == null) {
        LAST_GAME_ID = null;
        $("#table_game tbody").empty();
        return;
    }
    if (game.id != LAST_GAME_ID) {
        LAST_GAME_ID = game.id;
        $("#table_game tbody").empty();
        $("#table_game tbody").append(
            "<tr class='text-center'>" +
                "<td>" + game.name + "</td>" +
                "<td>" + game.difficulty + "</td>" +
                "<td>" + game.time + "</td>" +
                "<td>" + game.strikes + "</td>" +
            "</tr>"
        );
    }
}

var FINISHING = false;
var STARTING = false;
var RESTARTING = false;
function reloadHasEnded() {
    if (STARTING) {
        Notification.showTime(
            Notification.TYPE.SUCCESS,
            'El nivel a empezado!',
            2000
        );
    } else if (FINISHING) {
        Notification.showTime(
            Notification.TYPE.DANGER,
            'El nivel a terminado!',
            2000
        );
    } else if (RESTARTING) {
        Notification.showTime(
            Notification.TYPE.SUCCESS,
            'El nivel se a reiniciado!',
            2000
        );
    }
    FINISHING = false;
    STARTING = false;
    RESTARTING = false;
}

var GAME = null;
setInterval(function () {
    $.ajax({
        url: URL_CURRENT_GAME,
        success: function (result) {
            GAME = result;
        },
        error: function (error) {
            GAME = null;
        }
    });
    if (GAME != null && GAME.active) {
        setTime(GAME.time);
        reloadGameTable(GAME.game);
        setGameView(true);
    } else {
        reloadGameSelect();
        setGameView(false);
    }
    reloadHasEnded();
}, 1000);
