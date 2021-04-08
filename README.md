# Particle photon door alarm
This is an embedded system project; the project's goal is to create an alarm system that will detect if a door is opened and send an email to the owner that their door has been opened.
It will also sound an alarm and send an IR signal or LED strips, making them flash blue and red.

Video of a demo is TBA
##Project status
<b>This project is not supported but should work anyways.</b>

## Installation
Parts needed:
* Particle Photon
* HC-SR04 Ultrasonic Distance Sensor
* Active or passive buzzer (Piezo Speaker)
* Momentary Switch (Button)
* 5mm IR Transmitter (or any size with two-pin would work in theory)
* 2x 3mm LED (I used green and red) or 1 RGB LED (need small adjustments in code)
* Standard jumper cables
* 3x 220Ω Resistor
  3x 1kΩ Resistor
* Breadboard

Follow this schematic: <br>
[Fritzing](https://i.imgur.com/ffz09w8.jpg)

Uploading the code to Particle Photon can be done in two ways, either via particle photons cloud IDE or visual studio codes.
'Particle Workbench' extension. https://www.particle.io/workbench#installation

```
To open the Serial monitor in the Particle Workbench, you must open its
command palette by cmd + shift + p on macOS or ctrl + shift + p on Linux or Windows.
If that does not work, then you can cd in with the terminal and enter:
"particle serial monitor --follow". 

Must have Particle CLI installed for it to work.
https://docs.particle.io/tutorials/developer-tools/cli/
```

To get an email, you need to make a particle event.
* Go to https://build.particle.io and setup and login to your Particle Photon
* Go to https://www.mailgun.com/ and make a free account; you need to enter in a cc
* After logging in, go to https://console.particle.io/integrations and make a new event. Call it mailgunEvent.
* Click on the custom template tab and fill in the required information, then save it.
```JSON
{
  "event": "mailgunEvent",
  "deviceID": "",
  "url": "https://api.mailgun.net/v3/YOUR_DOMAIN_NAME_FROM_MAILGUN/messages",
  "requestType": "POST",
  "noDefaults": true,
  "rejectUnauthorized": false,
  "form": {
    "to": "{{my_email}}",
    "from": "Notifications <Notifications@YOUR_DOMAIN_NAME_FROM_MAILGUN>",
    "subject": "{{subject}}",
    "text": "{{body}}"
  },
  "auth": {
    "username": "api",
    "password": ""
  }
}
```


## Contributing
Pull requests are welcome. For significant changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)

