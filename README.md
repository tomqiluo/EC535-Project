# EC535-Project: Air Quality Monitoring System

## Overview

The Air Quality Monitoring System is an embedded system based on the BeagleBone platform, designed to monitor air quality in real-time. The system is capable of measuring various parameters, such as CO2, CO, VOC, temperature, and humidity levels, providing crucial information for maintaining a safe and healthy environment. Click [here](https://drive.google.com/file/d/1234567890123456789/view?usp=sharing) to see a demo of the project.
![Air Quality Monitoring System](/Media/image.jpg)

## Features

* Integration of multiple sensors:
    * CCS811 sensor for CO2 and VOC detection
    * SI7021 sensor for temperature and humidity measurement
    * Flying-fish MQ-7 sensor for CO level monitoring
* 4.3" LCD screen for data display
* Color-coded LED system for real-time air quality status indication:
    * Green: Safe environment
    * Yellow: Changing environment
    * Red: Dangerous CO levels
* Suitable for various environments, including residential, commercial, and industrial settings

## Prerequisites

* BeagleBone Black (or compatible) embedded platform
* CCS811 air quality sensor
* SI7021 temperature and humidity sensor
* Flying-fish MQ-7 gas sensor
* 4.3" LCD screen
* Red, yellow, and green LEDs
* Jumper wires and breadboard (or PCB for a more permanent solution)

## Getting Started

1. Assemble the hardware components as per the provided circuit diagram.
2. Connect the sensors and LEDs to the appropriate pins on the BeagleBone Black.
3. Attach the 4.3" LCD screen to the BeagleBone Black.
4. In *EC535-Project/Sensor*, run
    `make`
to generate a exectuable file **sensor**.
5. In *EC535-Project/LCD*, run
    `qmake`
to generate a **Makefile**.
Then, run
    `make`
to Generate a exectuable file **lcd**.
6. Connect the PC to Baglebone Black, and transfer **sensor** and **lcd** to it.
7. In the same directory as you store those two executables, run
    `./sensor &`
    and
    `./lcd`.
8. Enjoy the program.

## Contributors
* Ahmet Caliskan [aeclskn@bu.edu]: Qt Program
* Qi Luo [tomqiluo@bu.edu]: Sensors

## Acknowledgements
This is our culminating project for ENG EC535. We have made this repository public in the hopes of aiding others in achieving success in embedded systems. The two of us have gained a wealth of knowledge from this course.
