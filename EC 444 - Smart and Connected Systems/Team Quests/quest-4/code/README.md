# Code Readme
In the './carv2/udp_server/upd_server.c' file, we have created a function that makes the vehicle to stop when an obstacle is detected within 40 cm within range, to allow slow down. This function also adjusts and controls the ultrasonic sensors, LIDAR, PID, wheel speed, and Alpha Display. In order to control the vehicle, we have made the ESP32 the HOST, as it receives data from the client via anyone that has access to the website. 
The './carv2/udp_server/remote-control' file is the nodejs file we need to run on our laptop to send command to the esp32. <br>


- In our code, we have a buttons that control the vehicle and and its many functions. Utilize Node Server to wirelessly communicate functions. <br>
    - 'A', 'W', 'S', 'D' to control, Left, Forward, Back, Right, respectively <br>
    - 'M': Manuel Mode<br>
    - 'G': Automatic Mode <br>
    - 'Q': Immediately stop the vehicle <br>
    - The HOST (ESP32) uses SOCKET.io to recieve data from the Node Server to the ESP32 memory. <br>

- PID: 
    - Uses the wheel speed sensors to determines the speed of the vehicle.
    - Finds the difference in the distance between the vehicle speed and the target speed. <br> 
    - Used for the controlling of the speed, and the manuevering of the wheels. <br>

- Ultrasonic Sensors:
    - Finds the difference in the distance between the two sensors. <br>
    - Calculates the turning angle, and multiple by constants. <br>
    - Keeps the vehicle '30cm' - '50cm' away from the wall, while simutaneously keeping a straight path. <br>

- LIDAR:
    - Calculates the variable by multipling by constants. <br>
    - Sets motor power based on the varibale, and stops within 20 cm of an obstacle. <br>
    - Also, calculates the distance between the obstacle ahead and the distance of the vehicle.

- ALPHANUMERIC DISPLAY:
    - Displays the elasped time.
