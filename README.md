# Sensor-Project
## Description:

Developed an Arduino code for an ESP32 device that integrates sensors and Bluetooth technology to provide real-time data monitoring and visualization. The device collects information from various sensors and wirelessly transmits it to a companion mobile app via Bluetooth. This enables users to conveniently access and analyze the data on their smartphones.
The companion app offers multiple data retrieval options, including instantaneous value, mean value of multiple readings, and real-time chart display. Users can request the current sensor readings, calculate the average value over a specific period, or visualize the data in a dynamic chart, providing them with comprehensive insights into the sensor data.

## Code breakdown:

The code starts by including the necessary libraries and defining constants for sensor pins, Bluetooth server name, and team ID.

Various functions are declared, such as "receivedData," "getValuesFR," "getValuesDHT," "getAveragesDHT," and "getSensors." These functions handle different aspects of data retrieval and communication.

The "chartFR" function is responsible for continuously reading the photoresistor value and sending it as JSON data over Bluetooth. It runs until the "stopChart" flag is set.

Similarly, the "chartDHT" function continuously reads temperature and humidity values from the DHT11 sensor and sends them as JSON data over Bluetooth. It also runs until the "stopChartDHT" flag is set.

The "getAveragesDHT" function calculates the average temperature and humidity values based on a specified number of readings and sends them as JSON data over Bluetooth.

The "getValuesFR" function retrieves the light value from the photoresistor and sends it as JSON data over Bluetooth.

The "getValuesDHT" function reads the temperature and humidity values from the DHT11 sensor and sends them as JSON data over Bluetooth.

The "getSensors" function prepares JSON data containing information about the connected sensors (DHT11 and photoresistor) and sends it over Bluetooth.

The "receivedData" function handles incoming data received over Bluetooth. It parses the JSON data, determines the action to be taken based on the received commands, and calls the appropriate functions accordingly.

The "setup" function initializes the serial communication, Bluetooth, and sensor configurations.

The "loop" function continuously checks for incoming data over Bluetooth and calls the "receivedData" function when data is available.

## Project Requirements and Usage

This project includes code for an ESP32 device that communicates with a mobile app. However, please note that the mobile app used for this project is not publicly available. It requires a team ID and specific configuration settings to establish a Bluetooth connection with the ESP32 device.

While the mobile app's code is not included in this repository, the ESP32 code can still be useful for learning purposes or as a reference for similar projects. You can modify the code to work with other Bluetooth-enabled apps or adapt it to your specific requirements.

Please be aware that without the compatible mobile app and team ID, the full functionality of this project may not be accessible.

