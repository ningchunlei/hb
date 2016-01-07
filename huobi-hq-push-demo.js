//
// 火币实时行情push演示
// 注意socket.io-client版本是用的0.9.16版本的，最新版本的socket.io可能有兼容问题，连接不上推送服务器。
// 在node命令行下执行:node huobi-hq-push-demo.js
// node.js下socket.io版本请用0.9.16：安装命令：npm install socket.io-client@0.9.16。
// 其他语言请用找开源是socket.io实现，即可连接火币实时行情推送服务器。
//

var fs = require("fs");
var file = "./huobi.db";
var exists = fs.existsSync(file);

if(!exists) {
  console.log("Creating huobi file.");
  fs.openSync(file, "w");
}

var sqlite3 = require("sqlite3").verbose();
var db = new sqlite3.Database(file);

var g_isConnect = 0;

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


// 检查推送的数据完整性
exports.checkConnection = function() {
    try {
        exports.systemlog("checkConnection start");

        if(g_isConnect == 2)
        {
            exports.systemlog("checkConnection checking");
            exports.connect();
        }
    } catch(err) {
        var errMsg = exports.dumpError(err);
        exports.quicklog(errMsg);
    }
};

var io = require('socket.io-client');
require("date-utils");

exports.connect = function() {
    try {
        if(g_isConnect == 3)
        {
            console.log('websocket client is connecting to push server:');
            return;
        }
        
        g_isConnect = 3;
        var socket = io.connect("hq.huobi.com", {
        "force new connection": 0,
        "sync disconnect on unload": 0,
        "reconnection delay": 1000,
        "reconnection delay max" : 5000,
        "max reconnection attempts": 999999
        });
 
        //var option = {'force new connection': 0, reconnection: true};
        //var socket = io.connect('hq.huobi.com:80', option);
    
        console.log('websocket client connecting to push server:');

        socket.on('connect', function(){
            g_isConnect = 1;
            console.log('websocket client connect to push server:' + socket.socket.sessionid);

            // 莱特币
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
            //socket.emit('request', json);
		
            var tday = parseInt(new Date().addHours(-24*7).getTime()/1000);
	    //var history = '{"version":1,"msgType":"reqKLine","requestIndex":1404103038520,"symbolld":"ltccny","period":"1min","from":0,"to":-1}';
            var history = {"version":1,"msgType":"reqKLine","requestIndex":Date.now(),"symbolId":"ltccny","from":0,"to":-1,"period":"1min"};
            console.log(history);
            socket.emit('request',history);
        });
        
        socket.on('disconnect', function(){
            g_isConnect = 2;
            console.log('websocket client disconnect from push server:' + socket.socket.sessionid);
        });
    
        socket.on('reconnect', function(){
            g_isConnect = 1;
            console.log('websocket client reconnect from push server:' + socket.socket.sessionid);
        });
        
        socket.on('message', function(data) {
             console.log(data);
//           try{ 
//	   db.serialize(function() {
//		var table = "CREATE TABLE IF NOT EXISTS "
//                     +data.payload.symbolId+"_"+data.payload.period
//                     +  " (rtime DATETIME UNIQUE,"
//                     +  "  open DECIMAL(10,2),"
//                     +  "  high DECIMAL(10,2),"
//                     +  "  low DECIMAL(10,2),"
//                     +  "  close DECIMAL(10,2)"
//		     + ");";
//                db.run(table);
//                db.run("create index IF NOT EXISTS index_name_"+data.payload.period+" on "+data.payload.symbolId+"_"+data.payload.period+" (rtime);");
//                db.each("select count(*) as count  from "+data.payload.symbolId+"_"+data.payload.period+" where rtime=?",[data.payload.time],function(err,rows){
//                        if(rows.count==0){
//                            var stmt = db.prepare("insert into " + data.payload.symbolId+"_"+data.payload.period +" values (?,?,?,?,?)");
//                            stmt.run(data.payload.time,data.payload.priceOpen,data.payload.priceHight,data.payload.priceLow,data.payload.priceLast);
//                            stmt.finalize(); 
//                        }
//                });
//                
//	    });
//            }catch(err){
//                console.log(err);
//	    }
            //db.close();
            console.log(JSON.stringify(data));
        });
        
    } catch(err) {
        var errMsg = exports.dumpError(err);
        exports.quicklog(errMsg);
    }
};


exports.connect();

process.on("uncaughtException", function (err) {
  //打印出错误
  console.log(err);
  //打印出错误的调用栈方便调试
  console.log(err.stack);
});

//var g_checkTimerEvent = setInterval(exports.checkConnection,
//            5000);
