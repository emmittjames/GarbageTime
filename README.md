# Garbage Time :D

## Table of contents
* [General Info](#general-info)
* [Technologies](#technologies)
* [Sample Images](#sample-images)

## General Info
#### Website link: [garbagetime1.netlify.app](https://garbagetime1.netlify.app/)
This project is based around using an [IOT LoRa board](https://heltec.org/project/wifi-lora-32-v3/) to broadcast sensor data from a trashcan to a [LoRaWAN](https://en.wikipedia.org/wiki/LoRa) network ([The Things Network](https://www.thethingsnetwork.org/)). The sensor data is collected by an ultrasonic sensor that measures the distance from the lid of the trash can to its bottom, indicating how full or empty the trash can is. This data is then retrieved from The Things Network by the backend and sent to the frontend for display on a map.
	
## Technologies
Project created with:
* JavaScript - Frontend
* HTML - Frontend
* CSS - Frontend
* Python - Backend
* C++ - Sensor
* [Heltec LoRa board](https://heltec.org/project/wifi-lora-32-v3/) - Transmitting to LoraWAN Network
* [The Things Network](https://www.thethingsnetwork.org/) - LoraWAN Network
* [Ultrasonic Sensor](https://www.adafruit.com/product/4007) - Measuring distance
* [Google Maps API](https://developers.google.com/maps) - Displaying data

Project hosted with:
* [Netlify](https://www.netlify.com) - Frontend hosting
* [Render](https://render.com/) - Backend hosting

## Sample Images

#### Setup on a trash can lid
<img width="525" alt="Trash can setup" src="https://github.com/user-attachments/assets/c722a8fc-aa1b-4ffe-ab54-0c1cc59289d0">

#### Mostly empty bin
<img width="1440" alt="Screenshot 2024-08-28 at 7 48 40 PM" src="https://github.com/user-attachments/assets/418a6d9e-b1e6-4ae4-941b-5ff85e02ee86">

#### Medium bin
<img width="1440" alt="Screenshot 2024-08-28 at 7 47 12 PM" src="https://github.com/user-attachments/assets/e95578cd-0517-4957-9605-a724b44b291a">

#### Mostly full bin
<img width="1440" alt="Screenshot 2024-08-28 at 7 49 30 PM" src="https://github.com/user-attachments/assets/27f09c50-1945-4d5b-92fb-badad7438d27">

