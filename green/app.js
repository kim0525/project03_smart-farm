var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var SerialPort = require('serialport');
var parsers = SerialPort.parsers;
var parser = new parsers.Readline({
  delimiter: '\r\n'
});

var sp = new SerialPort('/dev/ttyUSB0', {
  baudRate: 9600
});

sp.pipe(parser);

sp.on('open', () => console.log('Port open'));

parser.on('data', function(data)
{
	if(data.substring(0,4) == "TEMP"){
		var temperature = data.substring(4);
		io.emit('temp', temperature);
		console.log('temerature: ' + temperature + '℃');
	}
	else if(data.substring(0,4) == "HUMI"){
		var humidity = data.substring(4);
		io.emit('humi', humidity);
		console.log('himidity: ' + humidity + '%');
	}
	else if(data.substring(0,4) == "MOIS"){
		var moisture = data.substring(4);
		io.emit('mois', moisture);
		console.log('moisture: ' + moisture);
	}
	else if(data.substring(0,5) == "SERVO"){
		if     (data.substring(5) == "0")	ventState = "Vent-shutter is Closed";
		else if(data.substring(5) == "1")	ventState = "Vent-shutter is Opened to position1";
		else if(data.substring(5) == "2")	ventState = "Vent-shutter is opened to position2";
		io.emit('vent', ventState);
		console.log(ventState);
	}
	else if(data.substring(0,5) == "DCFAN"){
		if     (data.substring(5) == "0")	fanState = "Fan is turned Off";
		else if(data.substring(5) == "1")	fanState = "Fan is turned On";
		io.emit('fan', fanState);
		console.log(fanState);
	}
	else if(data.substring(0,5) == "VALVE"){
		if     (data.substring(5) == "0")	valveState = "Water-valve is Closed";
		else if(data.substring(5) == "1")	valveState = "Water-valve is Opened";
		io.emit('valve', valveState);
		console.log(valveState);
	}
	else;
});

app.get('/fan_on',function(req,res)
{
	sp.write('DCFAN1\n\r', function(err){
		if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: FAN On');
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('Fan on\n');
	});
});

app.get('/fan_off',function(req,res)
{
	sp.write('DCFAN0\n\r', function(err){
		if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: FAN Off');
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('Fan off\n');
	});
});

app.get('/water_on',function(req,res)
{
	sp.write('VALVE1\n\r', function(err){
		if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: Watwer valve open');
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('Valve open\n');
	});
});

app.get('/water_off',function(req,res)
{
	sp.write('VALVE0\n\r', function(err){
		if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: Watwer valve close');
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('Valve close\n');
	});
});

app.get('/vent_pos0',function(req,res)
{
	sp.write('SERVO0\n\r', function(err){
		if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: Vent shutter close');
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('Vent close\n');
	});
});

app.get('/vent_pos1',function(req,res)
{
	sp.write('SERVO1\n\r', function(err){
		if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: Vent shutter open untill position1');
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('Vent open to position1\n');
	});
});

app.get('/vent_pos2',function(req,res)
{
	sp.write('SERVO2\n\r', function(err){
		if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: Vent shutter open untill position2');
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('Vent open to position2\n');
	});
});

app.use(express.static(__dirname + '/public'));

http.listen(3000, function(){
    console.log('listening on *:3000');
});
