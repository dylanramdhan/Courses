// Serial port example from design pattern
const fs = require('fs');
const {SerialPort} = require('serialport')
const port = new SerialPort({ path: 'COM3', baudRate: 115200 });

const {ReadlineParser} = require('@serialport/parser-readline');

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));


fs.writeFile("data.txt", "", (err) => {
  if (err)
    console.log(err);
  else {
    console.log("File successful\n");
  }
});

// Read the port data
port.on("open", () => {
  console.log('Serial port now open');
});
parser.on('data', data =>{
  
  
  console.log('The word from ESP32:', data);
  fs.appendFile("data.txt", data, (err) => {
    if (err)
      console.log(err);
    else {
      console.log("File written successfully\n");
    }
  });
});


const express = require('express');
const http = require('http');


const app = express();
const server = http.createServer(app);   // Creates web server in node
const io = require('socket.io')(server); // Uses socket.io to pass data to client



// Serve the HTML page
app.get('/', (req, res) => {
    res.sendFile(__dirname+'/index.html');
});

// Listen for incoming WebSocket connections on the localhost port

io.on('connection', (socket) => {
    console.log('A user connected');
    filename = "data.txt";

    function sendDataToSocket(){
    fs.readFile(filename, function(err, data) {
      if (err) {
        res.writeHead(404, {'Content-Type': 'text/html'});
        return res.end("404 Not Found");
      } 
      data = data.toString();
      socket.emit('data', data);
    });
    }
    const intervalId = setInterval(sendDataToSocket, 1000);
    

    // Handle disconnections
    socket.on('disconnect', () => {
        console.log('A user disconnected');
        clearInterval(intervalId);
    });
});

// Start the server
server.listen(8081, () => {
    console.log(`Server is running on http://localhost:${8081}`);
    
});
