# Garbage Time :D

## Table of contents
* [General Info](#general-info)
* [Technologies](#technologies)
* [Sample Images](#sample-images)

## General Info
#### Website link: [garbagetime1.netlify.app]([https://uvawater.net](https://garbagetime1.netlify.app/))
This website allows users to view ratings for water foutnains around grounds at UVA by clicking on markers displayed on a map. Users can also submit their own ratings via the rating form or any other kind of feedback via the contact form.

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
* [Ultrasonic Sensor](https://www.adafruit.com/product/4007)
* [Google Maps API](https://developers.google.com/maps)

Project hosted with:
* [Netlify](https://www.netlify.com) - Frontend hosting
* [Render](https://render.com/) - Backend hosting

## Cloning
1. Clone the repository
2. [Download](https://www.postgresql.org/download/) and install PostgreSQL on your machine with the default port of 5432
3. Create a database in PostgreSQL
4. Create a .env file in the server directory
5. Put the following into the .env file using your PostgreSQL credentials:
```
DATABASE_URL = "postgres://Your_PostgreSQL_username:Your_PostgreSQL_password@localhost:5432/Your_PostgreSQL_database_name"
PORT = "3000"
```
6. Create a file called env.js in the client directory
7. Put the following into env.js:
```
const BACKEND_URL = "https://localhost:3000"
```
8. Everything should be ready to work on your localhost!

## Sample Images

#### Viewing ratings on the map
<img width="1440" alt="Viewing ratings" src="https://user-images.githubusercontent.com/90576219/216737244-dd2f58f1-1dfe-47fb-bbb1-f2856571b41e.png">

#### Submitting a water fountain rating
<img width="1440" alt="Submitting rating" src="https://user-images.githubusercontent.com/90576219/216737257-750f673f-5447-4047-9f72-cd9d08849670.png">

#### Giving feedback on the site
<img width="1440" alt="Submitting feedback" src="https://user-images.githubusercontent.com/90576219/216737404-2fcf3b53-143b-4d7b-8227-9dcc17d944c9.png">

## License
Distributed under the MIT License. See ```LICENSE``` for more information

