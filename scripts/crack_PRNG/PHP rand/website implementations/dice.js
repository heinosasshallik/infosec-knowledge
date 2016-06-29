$(document).ready(function() {
    $('#dice-roll').click(function() {
        $(this).prop('disabled', true);
        var cs = $("#cseed").val(); //Gets the value of the element cseed
        var t = $("#dice-target").val(); //value over which it's rolled
        var a = parse($("#dice-amount").val());
        if (a > 500000000) {
            $("#dice-result").html("<span style='font-weight:bold;color:#ff4c4c'>Maximum bet is 500M tokens</span>");
            $(this).prop('disabled', false);
            return false;
        }
        $.ajax({
            type: "POST",
            url: "/do/dice",
            data: {
                amt: a,
                cseed: cs,
                "dir": $("#dice-over").hasClass("selected") ? "over" : "under",
                tgt: t
            },
            success: function(data) {
                try {
                    var res = JSON.parse(data);
                    var textColor = "#ff4c4c";
                    $("#sseed").val(res.sseed);
                    if (res.success === 1) {
                        refreshDicingList();
                        if (res.win === 1) {
                            textColor = "greenyellow";
                        }
                        $("#dice-roll").parent().effect('shake');
                        $('#dice-result').hide().html("<p>You rolled <span style='font-weight: bold;color:" + textColor + "'>" + res.roll + "</span>" + " and " + "<span style='font-weight: bold;color:" + textColor + "'>" + (res.win === 1 ? "Won" : "Lost") + " " + format(res.profit) + " Tokens</span></p>").fadeIn();
                        var origColor = $("#token-balance").css("color");
                        $("#tokens-row #tokens-section-balance").text(res.tokens);
                        $("#token-balance u").text(res.tokens);
                        $("#token-balance u").css({
                            "font-weight": "bold!important",
                            "color": textColor + '!important'
                        }).delay(2500).css({
                            "font-weight": "normal!important",
                            "color": origColor + '!important'
                        });
                    } else {
                        $('#dice-result').html("<span style='color:" + textColor + "'><b>Error:</b> " + res.error + "</span>");
                    }
                } catch (e) {
                    $('#dice-result').html("<span style='color:#ff4c4c'>" + data + "</span>");
                }
            },
            complete: function() {
                $('#dice-roll').prop('disabled', false);
            }
        });
        return false;
    });
    $("#dice-over").click(function() {
        $('#typeRoll').html('UNDER');
        $("#dice-under").addClass("selected");
        $("#dice-over").removeClass("selected");
        $('#dice-under').show();
        $('#dice-over').hide();
        var t = round1(parseFloat($("#dice-target").val()));
        if (isNaN(t)) {
            t = 45;
        } else if (t > 89) {
            t = 89;
        } else if (t < 10) {
            t = 10;
        }
        var m = round2(94 / t);
        $("#dice-target").val(t); //change dice-target to t
        if ($("#dice-under").hasClass("selected")) {
            $('.percchance').val(t - 1);
        } else {
            $('.percchance').val(100 - t - 1);
        }
        $("#dice-multiplier").val(m);
        $("#dice-profit").val(format(round1(parse($("#dice-amount").val()) * m) - $("#dice-amount").val()));
    });
    $("#dice-under").trigger("click");
    $("#dice-under").click(function() {
        $('#typeRoll').html('OVER');
        $("#dice-over").addClass("selected");
        $("#dice-under").removeClass("selected");
        $('#dice-under').hide();
        $('#dice-over').show();
        var t = round1(parseFloat($("#dice-target").val()));
        if (isNaN(t)) {
            t = 53;
        } else if (t > 90) {
            t = 90;
        } else if (t < 10) {
            t = 10;
        }
        var m = round2(-1 * 94 / (t - 100));
        $("#dice-target").val(t);
        if ($("#dice-under").hasClass("selected")) {
            $('.percchance').val(t - 1);
        } else {
            $('.percchance').val(100 - t - 1);
        }
        $("#dice-multiplier").val(m);
        $("#dice-profit").val(format(round1(parse($("#dice-amount").val()) * m) - $("#dice-amount").val()));
    });
    $("#dice-amount").on("change", function() {
        if ($('#dice-amount').val() > 500000000) {
            $('#dice-amount').val('500000000');
        }
        var realamt = round1(parseFloat($(this).val()));
        $(this).val(realamt);
        $("#dice-profit").val(format(round1(realamt * round2(parseFloat($("#dice-multiplier").val()))) - realamt), 1);
    });
    $("#dice-amount").val("50000");
    $("#dice-amount").trigger("change");
    $("#dice-target").on("change", function() {
        var t = round1(parseFloat($("#dice-target").val()));
        var m;
        if ($("#dice-over").hasClass("selected")) {
            if (isNaN(t)) {
                t = 55;
            } else if (t > 90) {
                t = 90;
            } else if (t < 10) {
                t = 10;
            }
            m = round2(-1 * 94 / (t - 100));
        } else {
            if (isNaN(t)) {
                t = 45;
            } else if (t > 89) {
                t = 89;
            } else if (t < 10) {
                t = 10;
            }
            m = round2(94 / t);
        }
        if ($("#dice-under").hasClass("selected")) {
            $('.percchance').val(t);
        } else {
            $('.percchance').val(100 - t);
        }
        $('#dice-target').val(t);
        $("#dice-multiplier").val(m);
        $("#dice-profit").val(format(round1(parse($("#dice-amount").val()) * m) - $("#dice-amount").val()));
    });
    $("#dice-multiplier").on("change", function() {
        var m = round2(parseFloat($(this).val()));
        if (isNaN(m)) {
            m = 2;
        } else if (m > 94) {
            m = 94;
        } else if (m < 1.04) {
            m = 1.04;
        }
        var t = round1(100 - 94 / m);
        if ($("#dice-under").hasClass("selected")) {
            t = round1(100 - t);
        }
        $(this).val(m);
        $("#dice-target").val(t);
        $("#dice_profit").val(format(parse("" + ($("#dice-amount").val() * m)) - $("#dice-amount").val()));
    });
    $("#new-cseed").click(function() {
        $("#cseed").val(cseed());
    });

    function cseed() {
        var chars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        var cs = '';
        for (var i = 16; i > 0; --i) cs += chars[Math.round(Math.random() * (chars.length - 1))];
        return cs;
    }
    $(".cseed").val(cseed());
    $("#verify-dice").click(function() {
        $.get('/modal/provably-fair', function(html) {
            $.fallr.show({
                content: html,
                width: $(window).width() - ($(window).width() * 0.3),
                height: $(window).height() - ($(window).height() * 0.15),
                closeOverlay: true,
                icon: "info",
                buttons: {
                    button1: {
                        text: 'OK',
                        danger: false,
                        onclick: function() {
                            $.fallr.hide();
                        }
                    }
                }
            });
        });
        return false;
    });
    $('#2xwager').click(function() {
        $('#dice-amount').val($('#dice-amount').val() * 2);
        $("#dice-amount").trigger("change");
    });
    $('#maxwager').click(function() {
        $('#dice-amount').val('500000000');
        $("#dice-amount").trigger("change");
    });
    $('#2xpot').click(function() {
        $('#dice-amount').val($('#dice-amount').val() * 2);
        $("#dice-amount").trigger("change");
    });
    $('#maxpot').click(function() {
        $('#dice-amount').val('500000000');
        $("#dice-amount").trigger("change");
    });
});

function round1(x) {
    return Math.round(x);
}

function round2(x) {
    return Math.round(x * 100) / 100;
}

function format(n) {
    return format(n, 2);
}

function format(n, r) {
    if (n >= 1000000000) {
        return +Math.round((n / 1000000000) * 100) / 100 + "B";
    } else if (n >= 10000000) {
        return +Math.round((n / 1000000) * 100) / 100 + "M";
    } else if (n >= 100000) {
        return +Math.round((n / 1000) * 100) / 100 + "K";
    } else {
        return (r === 2 ? round2(n) : round1(n));
    }
}

function parse(s) {
    if (/[a-z].*[a-z]/i.test(s)) {
        return 0;
    }
    s = s.toLowerCase();
    if (s.charAt(s.length - 1) === "k")
        return parseFloat(s) * 1000;
    else if (s.charAt(s.length - 1) === "m")
        return parseFloat(s) * 1000000;
    else if (s.charAt(s.length - 1) === "b")
        return (parseFloat(s) * 1000000000);
    else
        return parseFloat(s);
}

function refreshDicingList() {
    $('#tbodyDice').html('');
    $.getJSON('/do/ajax_dice', function(data) {
        $.each(data.diceList, function(index, elem) {
            $('#tbodyDice').append('<tr><td>' + elem.id + '</td><td>' + elem.poker_player + '</td><td>' + elem.created_on + '</td><td>' + elem.dice_amount + '</td><td>' + elem.dice_target + '</td><td>' + elem.dice_roll + '</td><td>' + elem.dice_profit + '</td><td>' + elem.dice_sseed + '</td><td>' + elem.dice_cseed + '</td></tr>');
        });
    });
}