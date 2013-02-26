var express  = require('express');
var routes   = require('routes');
var socketio = require('socket.io');
var fs       = require('fs');
var http     = require('http');

var server   = '127.0.0.1';
var port     = 3000;
var log_file = '/tmp/tmp.log';
var interval = 5000;

var app = express();
var io  = socketio.listen(app.listen(port));

var options = {
    host: '127.0.0.1',
    port: 80,
    path: '/api/get/scoreboard/all'
};

app.configure(function(){
    app.set('views', __dirname + '/views');
    app.set('view engine', 'ejs');
    app.use(express.bodyParser());
    app.use(express.methodOverride());
    app.use(app.router);
    app.use(express.static(__dirname + '/static'));
});

app.configure('development', function(){
    app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
});

app.configure('production', function(){
    app.use(express.errorHandler());
});

app.get('/', function(req, res){
    res.render('index', {
        title:  'Apache HTTP Server Status Realtime Monitor with mod_mruby',
        server: server,
        port:   port
    });
});

function call_api() {
    http.get(options, function(res) {
        console.log("Got response: " + res.statusCode);
        res.setEncoding('utf8');
        res.on('data', function (chunk) {
            console.log('BODY: ' + chunk);
            var json_hash = JSON.parse(chunk); 
            io.sockets.emit('change', json_hash.result);
        });
    }).on('error', function(e) {
        console.log("Got error: " + e.message);
    });
    setTimeout(call_api, interval);
}

call_api();

console.log("start app.js http://%s:%s", server, port);
