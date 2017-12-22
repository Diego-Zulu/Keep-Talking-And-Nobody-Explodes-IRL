# Keep-Talking-And-Nobody-Explodes-IRL
College project porting the videogame "Keep Talking And Nobody Explodes" with arduino and raspberry pi communicating through WiFi.

Proyecto universitario realizado en base al videojuego "Keep Talking And Nobody Explodes" con arduinos y una raspberry pi comunicándolos.

## Python
Este proyecto se realizó con Python3.
Para correrlo se requieren las siguientes librerías instadas:
1. python_vlc==2.2.6100
2. paho_mqtt==1.3.1
3. Django==1.11.7
4. django-bootstrap3==9.1.0
5. django-static-jquery==2.1.4

Adicionalmente se requieren instalados los paquetes vlc, mosquitto y mosquitto-clients.

## Atención
Si no tiene una pantalla lcd 16x2 I2C conectada a la raspberrypi o una raspberrypi se recomienda comentar los métodos del lcd_controller dentro de ServerMQTT/game/
