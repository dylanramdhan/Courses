//Team6-Xiong-Ramdhan-Gumusayak-Ramondo - 12/1   

var dgram = require('dgram');


var Engine = require('tingodb')();
var db = new Engine.Db('./mydb', {});

// Port and IP
var PORT1 = 3001;            // Port of node server for UDP traffic
var HOST1 = '192.168.1.10';  // IP of node server
// var HOST1 = '10.239.123.157';

var keys = ['password1', 'password2'];
// Create socket
var server1 = dgram.createSocket('udp4');

const express = require('express');
const app = express();
var moment = require('moment-timezone'); // Import the moment-timezone library

// connect to meter
var PORT = [3333, 4444];
var HOST = ['192.168.1.14', '192.168.1.24'];
var meter1 = dgram.createSocket('udp4');
var meter2 = dgram.createSocket('udp4');
var meters = [meter1, meter2]
var PORT2 = 3333;
var HOST2 = '192.168.1.29';
var fob1 = dgram.createSocket('udp4');

// Gobal variable
var esp32_message = '0000 0001 0002 0003'; // This is the form of the payload

// UDP traffic from ESP32, no acknowlegement or error checking
// Create server that listens on a port
server1.on('listening', function () {
  var address = server1.address();
  console.log('UDP Server1 listening on ' + address.address + ":" + address.port);
});

function updateTime() {
  // update timer
  db.collection('status', function (err, collection) {
    if (err) throw err;
    collection.find(({
      status: 0,
    })).toArray(function (err, returned_data) {

      if (returned_data) {
        for (let i = 0; i < returned_data.length; i++) {
          const element = returned_data[i];
          element['time'] -= 1;
          if (element['time'] == 0) {
            element['status'] = 1;
            element['FobID'] = -1;
          }
          // update db
          collection.update({ _id: element._id }, { $set: { time: element['time'], status: element['status'] } }, { upsert: false }, function (err, numReplaced, upsert) {
            if (err) {
              console.error('Error updating document:', err);
            }
          });

          console.log("ID", element['meterID'], "Time", element['time'], "status", element['status'], "fobid", element['FobID'])
          // send to meter
          var c = keys[element['meterID'] - 1] + " " + element['status']
          // console.log(i,meters.length)
          meters[element['meterID'] - 1].send(c, PORT[element['meterID'] - 1], HOST[element['meterID'] - 1], function (error) {
            if (error) {
              console.log('MEH!');
            }
            else {
              console.log('Sent:OK!');
            }
          });
        }
      }
    });
  });


  db.collection('status', function (err, collection) {
    if (err) throw err;
    collection.find(({
      status: 1,
    })).toArray(function (err, returned_data) {
      if (returned_data) {
        for (let i = 0; i < returned_data.length; i++) {
          const element = returned_data[i];
          // update db

          // send to meter
          var c = keys[element['meterID'] - 1] + " " + element['status']
          console.log("ID", element['meterID'], "Time", element['time'], "status", element['status'], "fobid", element['FobID'])
          // console.log(i,meters.length)
          meters[element['meterID'] - 1].send(c, PORT[element['meterID'] - 1], HOST[element['meterID'] - 1], function (error) {
            if (error) {
              console.log('MEH!');
            }
            else {
              console.log('Sent:OK!');
            }
          });

        }
      }
    });
  });
}

const intervalId = setInterval(updateTime, 1000);


const http = require('http');


const server = http.createServer(app);   // Creates web server in node
const io = require('socket.io')(server); // Uses socket.io to pass data to client



// Serve the HTML page
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

// Listen for incoming WebSocket connections on the localhost port

io.on('connection', (socket) => {
  console.log('A user connected');

  data = [];
  function sendDataToSocket() {
    console.log("starting ")
    db.collection('status', function (err, collection) {
      if (err) throw err;
      collection.find(({
        status: 1,
      })).toArray(function (err, returned_data) {
        // console.log(returned_data)
        if (returned_data) {
          for (let i = 0; i < returned_data.length; i++) {
            const element = returned_data[i];
            // update db

            // send to meter
            data.push([element['meterID'], element['status'], element['time'],element['FobID']])
            // console.log("ID", element['meterID'], "Time", element['time'], "status", element['status'], "fobid", element['FobID'])
            // console.log(i,meters.length)

          }
          // console.log("data status 1",data)
        }
      });
      // console.log("data status 1ifasdf",data)
    });
    // console.log("data is",data)
    db.collection('status', function (err, collection) {
      if (err) throw err;
      collection.find(({
        status: 0,
      })).toArray(function (err, returned_data) {
        // console.log(returned_data)
        if (returned_data) {
          for (let i = 0; i < returned_data.length; i++) {
            const element = returned_data[i];
            // update db

            // send to meter
            data.push([element['meterID'], element['status'], element['time'],element['FobID']])
            // console.log("ID", element['meterID'], "Time", element['time'], "status", element['status'], "fobid", element['FobID'])
            // console.log(i,meters.length)

          }
        }
      });
    });
    console.log("sending data",data)
    // data = data.toString();
    socket.emit('data', data);
    data = []
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


// On connection, print out received UDP message and format
server1.on('message', function (message, remote) {
  server1.emit("hello");
  console.log('Incoming data from: ' + remote.address + ':' + remote.port + ' - ' + message);
  const currTime = moment().format();    // Get current time from node server
  message = message.toString();          // Convert message to string
  const values = message.split(' ');     // Split into four values
  const meterID = parseInt(values[0]);
  const FID = parseInt(values[1]);
  stat = 0 // 0 is occupied, 1 is free,
  db.collection('status', function (err, collection) {
    console.log(meterID)
    if (err) throw err;
    collection.find(({
      meterID: meterID,
    })).toArray(function (err, returned_data) {
      if (err) throw err;
      if (returned_data.length == 0) { // meter does not exist
        stat = 0 // meter is empty
        var logCollection = db.collection('status');
        var logEntry = { time: 20, meterID: meterID, meterKey: keys[meterID - 1], status: stat, FobID: FID };
        logCollection.insert(logEntry, function (err, result) {
          if (err) throw err;
        });
      }

      const element = returned_data[0]; // should be 1 data point
      if (element) {
        var c = String(keys[meterID - 1]) + " " + String(element['status'])
      }else{
        var c = String(keys[meterID - 1]) + " " + String(stat)
      }
      console.log("sending", c, "to the fob")


      fob1.send(c, PORT2, HOST2, function (error) {
        if (error) {
          console.log('MEH!');
        }
        else {
          console.log('Sent:OK!');
        }
      });

      if (element && element['status'] == 1) { // parking is free
        stat = 0
        collection.update({ _id: element._id }, { $set: { time: 20, status: stat } }, { upsert: false }, function (err, numReplaced, upsert) {
          if (err) {
            console.error('Error updating document:', err);
          }
        });
      }


      // }
    });
  });

  var logCollection = db.collection('logs');
  var logEntry = { time: currTime, meterID: meterID, FobID: FID, status: stat };
  logCollection.insert(logEntry, function (err, result) {
    if (err) throw err;
    // console.log('Sensor data logged at:', currTime);
  });



});

// Bind server to port and IP
server1.bind(PORT1, HOST1);
