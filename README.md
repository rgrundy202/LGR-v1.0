# LGR1.0
## Background
In the fall of 2021, I saw that the NHL sells a goal horn that goes off whenever the specified team scores. I realized that I could do it cheaper and
better. Using a combination of previous Arduino skills and skills learned in the Intro CS the semester before, I endeavored to make an internet connected goal horn using the NHL API. When a goal was scored, the Arduino would set off an airhorn and large flashing red light. My friends favorite team is the New York Rangers, hence the title LGR (Let's Go Rangers). The team designation is hard coded in but later versions will have to option to use other code.

## Organization
This project represents a project developed in the winter of 2021-2022 as an application of the skills learned in Intro CS. It contains two main files. The LGR file contains the code to be run on the Arduino Mega. This board controls the LCD and the relays that then control the light and horn. The Wifi_Board file holds the code that worked with an ESP8266 board to send HTTPS requests to the API and receive the JSON files. This code then uses a library to parse the JSON Object into a combination of dictionary and lists simulating an object. Arduino language does not allow for objects. The ESP8266 then passes this data back to the Mega board via a serial connection where game data is displayed and requisite actions are taken.

## Capabilities
Right now the box will connect to the wifi and check if there is a game currently. If there is not a game, the box will query the NHL API for the next game and display that on the LCD. This includes the start time in EST, the two teams, and where they are playing. Future versions might have a count down but this one was supposed to be simple. If there is currently a game, the box loads the game data from the NHL API and displays the current time and score. Future versions may include displaying the shots but the LCD is small and has a limited resolution. Every time the box checks the score (once every 5 seconds), it checks if there is a change in score for the Ranger, if there is, it displays Goal! and fires the relays controlling the horn and the light. After the game ends, it displays the final score for the next 10 mins before restarting the whole loop and displaying the next game. 

## Problems
1. The Arduino language is not object oriented. This makes the code incredibly difficult to debug and every time you wish to pull a variable from the JSON you have to read through the plaintext output to figure out what the path to the variable is, which ends up being just a string of indices and keys that is somewhat arbitrary.
2. The ESP8266 Arduino library has trouble communicating with my schools wifi network, in that it works on every other network except for the one where I live 80% of the year. It catches the network error and identitfies it, I think it trips the firewall at the school. 
3. The horn I chose was a 3 trumpet airhorn from Harbor Freight. It draws 10 amps DC. I could not find a rectifier rated to that current within my price range. I am also not an electrician and 10 amps scare me. I tried to run it at lower current but the air compressor needs peak current to start. So the light work and the relay for the horn would fire, the horn just wouldn't go off.

## Future
This project has been deprecated and is being entirely rewritten in python on the same ESP8266 board. All future improvements to the code will be in python and in the separate project [LGR2.0][notalinkyet.com]. The board can be flashed to run python however a new LCD solution is needed 
