![image](https://github.com/user-attachments/assets/2d2aa734-7c50-47b7-842c-1d2b46c79abb)
Temperature and Humidity measurement is often useful in many applications like [Home Automation](https://circuitdigest.com/home-automation-projects), Environment Monitoring, [Weather station](https://circuitdigest.com/microcontroller-projects/esp12-nodemcu-based-iot-weather-station), etc. The most popularly used Temperature sensor next to [LM35](https://circuitdigest.com/tags/lm35) is the DHT11. In this repo, we will learn how to interface this **DHT11 with PIC16F877A** which is an 8-bit PIC Microcontroller. We will use this microcontroller to **read the values of Temperature and Humidity using DHT11** and display it on an LCD display.
## Features
- Reads temperature and humidity data from DHT11 sensor 
- Displays data on a 16x2 LCD module
- Includes complete Proteus simulation
- Written in C using MPLAB XC8 compiler
- Configurable for different hardware setups
## Hardware Requirements
- PIC16F877A microcontroller
- DHT11 temperature and humidity sensor
- 16x2 LCD module (HD44780 compatible)
- 4MHz crystal oscillator
- Basic components (resistors, capacitors, etc.)
## Software Requirements
- MPLAB X IDE
- XC8 compiler
- Proteus 8 Professional (for simulation)
## Getting Started
1. Clone this repository:
```git 
git clone https://github.com/CodingTR/Temperature-Humidity-Monitor-PIC16F877A-dht11.git
```
2. - Open the Proteus simulation file (`proteus/MyProject.pdsprj`) to view and run the simulation.
3. To build the firmware:
    - Open MPLAB X IDE
    - Create a new project and import the source files from the `src` directory
    - Set the XC8 compiler
    - Build the project
4. Program the HEX file to your PIC16F877A microcontroller.
## Usage
After powering up the circuit:
1. The system initializes and displays "DHT11 Sensor" on the first line
2. After successful sensor reading, it displays:
    - Temperature in °C on the first line
    - Humidity in % on the second line
3. Readings are updated
## Troubleshooting
- If the LCD doesn't display anything, check the contrast potentiometer
- If sensor readings fail, verify the DHT11 connection and timing
- Ensure all Proteus components have the correct properties set
