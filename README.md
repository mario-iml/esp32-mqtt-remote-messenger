# ESP32 MQTT Remote Messager

## Introduction

This is a present I made for my Long Distance Girlfriend.
It subscribes to an MQTT broker and displays the messages it receives on an LCD display.

Features:

- Display incoming messages on an LCD display
- LED indicator (a slow pulse means there is a message waiting)
- Ultrasonic sensor to detect if the box is open -> Show last message on LCD and turn LED indicator off
- Connect to WiFi

Hardware:

- ESP32
- LCD Display
- LED
- Ultrasonic Sensor

## Setup

1. Clone this repo
2. Copy `secrets.example.h` to `secrets.h` and fill in the values
3. Upload the code to the ESP32 (e.g., with Arduino IDE)

## Circuit

I sadly don't have a proper circuit diagram, but here is an image of the circuit.

![Circuit](media/circuit.png)

## Demo

Short video demo of the project. I know it's not the most beautiful, but it works :D.

https://github.com/user-attachments/assets/26d945f9-6a95-49b0-be4f-e9dcd2006c4b

