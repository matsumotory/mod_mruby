
var fs = require("fs");
var http = require('http');
var log = console.log;

fs.readFile("./hello.txt",'utf-8',function(error,data){
    if(error){
        return console.log(error);
    }else{
        var server = http.createServer(function (request, response) {
            response.writeHead(200, {'Content-Type': 'text/plain'});
            response.write(data+'\n');
            response.end();
        }).listen(3333);
    }
});
