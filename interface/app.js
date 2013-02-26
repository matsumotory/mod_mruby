var express  = require('express');
var routes   = require('routes');
var socketio = require('socket.io');
var fs       = require('fs');
var http     = require('http');

var server   = '127.0.0.1';
var port     = 3000;
var log_file = '/tmp/tmp.log';

var app = express();
var io  = socketio.listen(app.listen(port));

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
    title:  'api_get',
    server: server,
    port:   port
  });
});

var options = {
  host: '127.0.0.1',
  port: 80,
  path: '/api/get/scoreboard/idle_worker'
};

function get_api() {
  http.get(options, function(res) {
    console.log("Got response: " + res.statusCode);
    res.setEncoding('utf8');
    res.on('data', function (chunk) {
      console.log('BODY: ' + chunk);
      io.sockets.emit('change', chunk);
    });
  }).on('error', function(e) {
    console.log("Got error: " + e.message);
  });
  setTimeout(get_api, 5000);
}

get_api();

console.log("start app.js http://%s:%s", server, port);
