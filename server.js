const express = require('express');
const app = express();

const http = require('http');
const server = http.createServer(app);

const { Server } = require('socket.io');
const io = new Server(server);

const smoothie = require('smoothie')

const { SerialPort, ReadlineParser } = require('serialport');

// Configure the serial port
const port = new SerialPort({port: '/dev/cu.usbmodem14101', baudRate: 9600, path: '/dev/cu.usbmodem14101', autoOpen: true}); // Replace 'COMX' with your Arduino's serial port

// Create a parser to read lines from the Arduino
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

app.use(express.static('./'))

app.get('/', (req, res) => {
  res.sendFile(__dirname+'/index.html');
});

server.listen(3000, () => {
  console.log('listening on *:3000');
});

port.on('open', () => {
  console.log('A device connected');
});

port.on('close', () => {
  console.log('A device disconnected');
});


parser.on('readable', function () {
  var data = parser.read()
  io.emit('graph new data', {'time': Date.now(), 'value': data});
  console.log('Data:', data);
})