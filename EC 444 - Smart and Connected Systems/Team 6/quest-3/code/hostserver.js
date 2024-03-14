// Quest 3: Straba Social Media Hub

// Authors: Nick Ramondo, Dylan Ramdhan, Mete Gumusayak, Houjie Xiong

// Serial port example from design pattern
const fs = require('fs');

// Hosted on RPi
var dgram = require('dgram');

// Port and IP
var PORT = 3333;
var HOST = '192.168.1.37';

// Create socket
var server = dgram.createSocket('udp4');
// Create server that listens on a port
server.on('listening', function () {
  var address = server.address();
  console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// open file
fs.writeFile("data.txt", "", (err) => {
  if (err)
    console.log(err);
  else {
    console.log("File successful open\n");
  }
});
var d = -1
// On connection, print out received message
server.on('message', function (message, remote) {
  console.log(remote.address + ':' + remote.port +' - ' + message);
  // Send Ok acknowledgement
  server.send(`${d}`,remote.port,remote.address,function(error){
    if(error){
      console.log('MEH!');
    }
    else{
      console.log('Sent:OK!');
    }
  });
  
  console.log('The word from ESP32:', message);
  fs.appendFile("data.txt", message, (err) => {
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
const serversocket = http.createServer(app);   // Creates web server in node
const io = require('socket.io')(serversocket); // Uses socket.io to pass data to client



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
      data = data = data.toString();
      if (err) {
        res.writeHead(404, {'Content-Type': 'text/html'});
        return res.end("404 Not Found");
      }
      
      var lines = data.split("\n");
      // graphs data points
      var graphs = [];
      graphs[0] ={
          type: "bar",
          name: "Total steps",
          axisYIndex: 0,
          showInLegend: true,
          dataPoints: [],
      };
      graphs[1] ={
          type: "bar",
          name: "Body Temperature",
          axisYIndex: 1,
          showInLegend: true,
          dataPoints: [],
      };

      // Loop through each line in the CSV file
      // temp, steps, id
      var ids = [];
      var idpts = [];
      for (var i = 0; i < lines.length; i++) {
          var parts = lines[i].split(",");
          if (parts.length === 3) {
              var temp = parseFloat(parts[0]);
              currstep = parseInt(parts[1]);
              var id = parseInt(parts[2])
              // if did not have id before add it to list, if it does add steps to list
              if( !ids.includes(id)){
                  ids.push(id);
                  idpts[id] = {t: temp, step: currstep}
              }
              else {
                  idpts[id] = {t: temp, step: idpts[id].step+currstep}
              }
          }
      }

      // add temps ids and steps to list of data
      var pts = []
      for(var i in ids){
          pts.push([idpts[ids[i]].step,idpts[ids[i]].t,ids[i]])
      }

      // sort in order of who has most steps
      pts.sort(sortFunction);
      
      function sortFunction(a, b) {
          if (a[0] === b[0]) {
              return 0;
          }
          else {
              return (a[0] < b[0]) ? -1 : 1;
          }
      }
    for(var i in ids){
        graphs[0].dataPoints.push({ y: pts[i][0], label: "id: "+pts[i][2]}); // steps
        graphs[1].dataPoints.push({ y: pts[i][1], label: "id: "+pts[i][2]}); // body temp
    }
      var dataSeries = [];
      for (var graph in graphs) {
          dataSeries.push(graphs[graph]);
      }
      
      if(pts.length > 0){
        d = pts.pop()
        d = d[2]
        d = d.toString();
      }
      else{
        d= -1
      }

      socket.emit('data', dataSeries);

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
serversocket.listen(8081, () => {
    console.log(`Server is running on http://localhost:${8081}`);
    
});


// Bind server to port and IP
server.bind(PORT, HOST);
