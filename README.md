# LGR1.0
[Background]
In the fall of 2021, I saw that the NHL sells a goal horn that goes off whenever the specified team scores. I realized that I could do it cheaper and
better. Using a combination of previous Arduino skills and skills learned in the Intro CS the semester before, I endeavored to make an internet connected goal horn using the NHL API. When a goal was scored, the Arduino would set off an airhorn and large flashing red light.

[Organization]
This project represents a project developed in the winter of 2021-2022 as an application of the skills learned in Intro CS. It contains two main files. The LGR file contains the code to be run on the Arduino Mega. This board controls the LCD and the relays that then control the light and horn. The Wifi_Board file holds the code that worked with an ESP8266 board to send HTTPS requests to the API and receive the JSON files. This code then uses a library to parse the JSON Object into a combination of dictionary and lists simulating an object. The ESP8266 then passes this data back to the Mega board via a serial connection where game data is displayed and requisite actions are taken.

[Capabilities]
