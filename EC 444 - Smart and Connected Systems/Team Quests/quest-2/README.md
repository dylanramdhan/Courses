# Quest 2: Carmin Smart Watch

Authors: Nick Ramondo, Mete Gumusayak, Houjie Xiong, Dylan Ramdhan

Date: 2023-10-08

### Summary
  We were tasked with building a wearable device to act similar to Smart Watch devices commonplace today. A Smart Watch will commonly display the time of day, include a stopwatch feature, provide the user an alert in extreme temperatures, and measure & record physical activity and body temperature in real time. Our device will connect an accelerometer, a thermsister, push buttons, a buzzer, and an alphanumeric display via an ESP32 that will juggle all of these functions at once. 

## Self Assessment

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Displays clock time on alpha display as HR:MIN | 1 |  1     | 
| Functions as a simple activity timer (start, stop, reset) | 1 |  1     | |
| Provides continuous reporting of steps per 10s interval to laptop | 1 |  1     | 
| Provides continuous reporting of body temperature per 10s interval to laptop | 1 |  1     | 
| Data at laptop plotted as stripcharts | 1 |  1     | 
| Provides alert function | 1 |  1     | 


### Solution Design

Our device will display a default start time on the alphanumeric display and then begin to count up, following standard 24hr clock rules. The alphanumeric display is being powered by the ESP32 and is in communication with the I2C bus of the ESP32. Our device also has push buttons that can change what is displayed to a built-in stopwatch mode. The ESP32 will read for the press of the start/stop button and will display elapsed time between presses. The stopwatch can also be reset with a press of the reset button.

Our device is also equipped to scan user/enviromment inputs via a thermistor and an accelerometer. The ESP32 will read the voltage drop across a GPIO and this value is then converted to a temperature value in degrees Celsius. Similarly to the alphanumeric display, the accelerometer is connected to the ESP32 via the I2C bus. The accelerometer will read changes in acceleration, then this value is then interpreted and reported in code.

A NodeJS server is created on Port 8080 which reads the serial port output from the ESP32. This data is written into a file in CSV format. Another NodeJS server reads this CSV and sends it to a socket, this data is sent every 1 second. A script in an HTML file takes the data sent from the input socket and parses it. When the data is updated it displays the new points of body temperature and number of steps from the interval of the last 10 seconds. The data is only updated when the activity timer is activated on the ESP32, otherwise, the display does not change.

The last key feature of our design will alert the user to extreme changes in body temperature using a buzzer. While we did not obtain a buzzer, we incorporated an LED to alert the user visually. The basic function is the same- if body temperature goes above 38.4 degrees Celsius ([The temperature you should call your doctor at](https://www.mayoclinic.org/diseases-conditions/fever/symptoms-causes/syc-20352759#)) then a GPIO corresponding to an LED is activated and the LED will flash.

### Sketches/Diagrams
<p align="center">
<img src="./images/Screenshot 2023-10-06 at 2.44.18 PM.png" width="50%">
</p>
<p align="center">
Circuit Diagram
</p>



### Supporting Artifacts
- [Link to video technical presentation](https://youtu.be/Vbotk3tcmm0).
- [Link to video demo](https://youtu.be/BKkA0vObES4).
Note in the video the threshold for body temperature was changed for demo purposes


### Modules, Tools, Source Used Including Attribution


## Example Code Use
...

We used the following examples from the Skills included in Quest 2:

***Examples***

[Alphanumeric I2C](https://github.com/BU-EC444/04-Code-Examples/tree/main/i2c-display)

[Timer](https://github.com/espressif/esp-idf/tree/17ac4ba/examples/peripherals/timer_group)

[Accelerometer](https://github.com/BU-EC444/04-Code-Examples/tree/master/i2c-accel)

[Serial to Node](https://github.com/BU-EC444/04-Code-Examples/tree/main/serial-esp-to-node-serialport)

[CanvasJS](https://canvasjs.com/javascript-charts/stacked-column-chart/)




