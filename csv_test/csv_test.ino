#include <string.h>
const char *csv = ",65953,2022-01-06,03:00:00 GMT,NORMAL,UNPLAYED,,,,9,NYR,New York,Rangers,142,VGK,Vegas,Golden Knights,1243,T-Mobile Arena,,,,,,,,,,,,,,,,,,,,";
const int date = 0, gameTime = 1, awayTeam = 5, homeTeam = 9;
char *val = NULL;
String myArray[15];
int iterator = 0;
char hour = NULL;
char mins = NULL;
char *name = NULL;
char *time = NULL ;

  // put your setup code here, to run once:
  
  
void setup(){
    name = strtok(csv, ",");
    Serial.begin(9600);
    while(name != NULL)
    {
        name = strtok(NULL, ",");
        myArray[iterator] = name;
        iterator ++;
         
    }
    //strcopy (myArray[gameTime], time);
    //time = myArray[gameTime];
    hour = char(strtok(time, ":"));
    mins = strtok(NULL, ":");
    hour = hour - 5;
    if (hour <= 12){
      hour = 12 + hour;
    }
    
    Serial.println("Date: " + myArray[date]);
    Serial.println("Time: " + myArray[gameTime]);
    Serial.println("Home: " + myArray[homeTeam]);
    Serial.println("Away: " + myArray[awayTeam]);
    
}


void loop(){}
