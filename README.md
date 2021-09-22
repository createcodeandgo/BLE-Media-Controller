# About the project
 
I was looking for a little useful project to get started with BLE. I thought a little bluetooth controller would be nice as I am always carrying around my Keyboard to control a Raspberry Pi when watching videos and listening to music.

An overview of the connections:

![A schematic of the boards and connections](/photos/drawing.png "Overview of setup")

It is just a little prototype taped to my PC right now. As it is Adafruit hardware, it is probably suitable to make a little plush controller like this one:

https://learn.adafruit.com/plush-game-controller

The Flora Board talks to the BLE module over UART. The 4 buttons for play, next, volume up/down are capacitive touch. The software reads the input from all pads in a for loop and sends the appropriate HID signal if a touch is detected.

The project on my PC:

![Photo shows the Adafruit Flora board and the Flora BLE taped to my PC. Also the control pads, which are crocodile clips with paper icons. There are playpause, media next, volume down and volume up](/photos/setup.jpg "Microcontroller on PC")
