const express = require("express");
const app = new express();
const path = require('path');
const http = require('http');
const server = http.createServer(app);
const io = require('socket.io')(server);
 
var ind;
 
const SerialPort = require('serialport');
const Readline = SerialPort.parsers.Readline;
 
var port = new SerialPort("COM5", {baudRate: 9600}, function(err)
{
    if(err)
    {
        return console.log("Error on create: ", err.message);
    }
});
 
const parser = port.pipe(new Readline( { delimiter: '\r\n'}));
 
port.on('error', function(err)
{
    console.log("Any Error:", err.message);
});
 
parser.on('data', function(data){
    console.log(data);
    var msg ={
        "time": data.split(":")[0] + ":" + data.split(":")[1] + ":" + data.split(":")[2],
        "date": data.split(":")[3] + "/" + data.split(":")[4] + "/" + data.split(":")[5]
    }
    socket.emit('time',msg);
});
 
app.use(express.static(path.join(__dirname + '/public')));
 
var socket= io.of("/arduino").on("connection", function(socket)
{
    console.log("Arduino user connected");
    socket.on('disconnect', () =>
    {
        console.log("Arduino user disconnected");
    });
});
 
server.listen(3000);    
console.log("Express server listening at 3000: http://127.0.0.1:3000/index.html");