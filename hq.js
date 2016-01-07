var io = require('socket.io-client');
require("date-utils");

var fs = require("fs");
var file = "./huobi.db";
var exists = fs.existsSync(file);

if(!exists) {
  console.log("Creating huobi file.");
  fs.openSync(file, "w");
}

var sqlite3 = require("sqlite3").verbose();
var db = new sqlite3.Database(file);

// 获取错误信息
exports.dumpError = function(err) {
    var errMsg = '';
    
    if (typeof err === 'object') {
        if (err.message) {
            errMsg = '\nMessage: ' + err.message;
        }
        if (err.stack) {
            errMsg += '\nStacktrace:';
            errMsg += '====================';
            errMsg += err.stack;
        }
    } else {
        errMsg = '\ndumpError :: argument is not an object';
    }
    
    return errMsg;
}

// 写入到错误日志文件
exports.quicklog = function(s) {
    var logpath = "./error.log";
    var fs = require('fs');
    s = s.toString().replace(/\r\n|\r/g, '\n'); // hack
    var fd = fs.openSync(logpath, 'a+', 0666);
    fs.writeSync(fd, s + '\n');
    fs.closeSync(fd);
}

exports.systemlog = function(s) {
    console.log(s);
}


var ws = "hq.huobi.com";
    
    var tday = parseInt(new Date().addHours(-24*7).getTime()/1000);
    var socket_api = {"version":1,"msgType":"reqKLine","requestIndex":Date.now(),"symbolId":"ltccny","from":tday,"to":-1,"period":"1min"};
    console.log(socket_api);

  var strMsg = '{"symbolList":{'
					+                   '"lastKLine":[{"symbolId":"ltccny","pushType":"pushLong","period":"1min"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"5min"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"15min"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"30min"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"60min"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"1day"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"1week"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"1mon"},'
					+                                  '{"symbolId":"ltccny","pushType":"pushLong","period":"1year"}'
					+                                  ']'
					+                  '},"version":1,"msgType":"reqMsgSubscribe","requestIndex":1404103038520}';

            
    var json = JSON.parse(strMsg);
 

    function requestEvent(data) {
        console.log(data);
    }
    function callback(data) {
        console.log(data);
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
        conn.emit("request", json);
        conn.emit("request", socket_api);
    });
    conn.on('error', function(){
        console.warn('error',arguments)
    });

