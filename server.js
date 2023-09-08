const express = require('express');
// import express from 'express'
const app = express();
const http = require('http');
// import http from 'http'
const server = http.createServer(app);
const { Server } = require('socket.io');
const io = new Server(server);
const smoothie = require('smoothie')

app.use(express.static('./'))

app.get('/', (req, res) => {
  res.sendFile(__dirname+'/index.html');
});

server.listen(3000, () => {
  console.log('listening on *:3000');
});

setInterval(function () {
  io.emit('graph new data', {'time': Date.now(), 'value': Math.random()})
  console.log('emitting value');
}, 1000);