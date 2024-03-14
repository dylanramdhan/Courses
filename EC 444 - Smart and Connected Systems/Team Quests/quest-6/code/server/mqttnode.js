
//Team6-Xiong-Ramdhan-Gumusayak-Ramondo - 12/9 



const mqtt = require('mqtt');
const express = require('express');
const http = require('http');
var moment = require('moment-timezone'); // Import the moment-timezone library
var Engine = require('tingodb')();
var dgram = require('dgram');

const host = '192.168.1.11'
// const host = 'broker.emqx.io'
const port = '1883'
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`

const connectUrl = `mqtt://${host}:${port}`



const app = express();
const server = http.createServer(app);   // Creates web server in node
const io = require('socket.io')(server); // Uses socket.io to pass data to client

var db = new Engine.Db(__dirname+'/mydb', {});
var server1 = dgram.createSocket('udp4');


//MQTT
const client = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  // username: 'emqx',
  // password: 'public',
  protocolVersion: 5, // using MQTT v5
  reconnectPeriod: 1000,
})

const topic = '/topic/temp'
const publish_topic = '/topic/set'
client.removeAllListeners();

// subscribe to temperatures
client.on('connect', () => {
  console.log('Connected')
  client.subscribe([topic], () => {
    console.log(`Subscribed to topic '${topic}'`)
  })
})

// listen for temperatures
client.on('message', (topic, payload) => {
  payload = payload.toString()
  console.log('Incoming data from topic ' + topic)
  console.log('Received Message:', payload + '\n')
  const currTime = moment().format();
  const values = payload.split(' ');
  const thermID = parseInt(values[0]);
  const temp = parseFloat(values[1]);


  var logCollection = db.collection('data');
  var logEntry = { thermID: thermID, temp: temp, time: currTime };
  logCollection.insert(logEntry, function (err, result) {
    if (err) throw err;
  });

})


////////////////////////////////////////////////////////
// Serve HTML page
////////////////////////////////////////////////////////




// Serve the HTML page
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

// Listen for incoming WebSocket connections on the localhost port
io.on('connection', (socket) => {
  console.log('A user connected');

  data = [];

  // on connecting to browser send last 60 minutes of data points
  
  beforetime = moment().subtract(60, 'minute').format()
  db.collection('data', function (err, collection) {
    if (err) throw err;
    collection.find(({
      time: { $gt: beforetime },
    })).toArray(function (err, returned_data) {
      if (returned_data) {
        console.log(returned_data)
        for (let i = 0; i < returned_data.length; i++) {
          const element = returned_data[i];
          
          // update 
          data.push([element['thermID'], element['temp'], element['time']])

        }
        
        //send to client
        socket.emit('data', data);
      }
    });
  });


  // send single data point to browser
  client.on('message', (topic, payload) => {
    payload = payload.toString()
    console.log('Incoming data from topic ' + topic)
    console.log('Received Message:', payload + '\n')
    const currTime = moment().format();
    const values = payload.split(' ');
    const thermID = parseInt(values[0]);
    const temp = parseFloat(values[1]);
    var d = [thermID, temp, currTime]
    socket.emit('data', d);

  })

  // send request in tempurature change
  socket.on("data", (data) => {
    // Parse the received data 
    console.log("data received")
    client.publish(publish_topic, `${data[0]} ${data[1]}`, { qos: 0, retain: false }, (error) => {
      if (error) {
        console.error(error)
      }else{
        console.log("sent",`${data[0]} ${data[1]}`)
      }
    })
  });

  // Handle disconnections
  socket.on('disconnect', () => {
    console.log('A user disconnected');
  });
});

// Start the server
server.listen(8081, () => {
  console.log(`Server is running on http://localhost:${8081}`);

});
