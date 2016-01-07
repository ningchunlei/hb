var io = require('socket.io-client');
require('date-utils')

//var xday = new Date(nw Date().toFormat("YYYY-MM-DD HH:MI"));

var tday = parseInt(new Date(new Date().toFormat("YYYY-MM-DD HH:MI")).addHours(-24*7).getTime()/1000);
var t_day = parseInt(new Date(new Date().toFormat("YYYY-MM-DD HH:MI")).addHours(-24*6).getTime()/1000);

var ws = "hq.huobi.com";
    var socket_api = {"version":1,"msgType":"reqKLine","requestIndex":Date.now(),"symbolId":"ltccny","from":tday,"to":t_day,"period":"5min"};
    console.log(socket_api);

    //socket.emit('request',JSON.parse(history));
    function requestEvent(data) {
        callback(data);
    }
    function callback(data) {
        console.log(data.payload);
        //do something
    }
    var conn = io.connect(ws, {
        "force new connection": 0,
        "sync disconnect on unload": 0,
        "reconnection delay": 1000,
        "reconnection delay max" : 5000,
        "max reconnection attempts": 999999
    });
    conn.on('request', requestEvent);
    conn.on('message', callback);
    conn.on('connect', function(){
        conn.emit("request", socket_api);
    });
    conn.on('error', function(){
        console.warn('error',arguments)
    });

