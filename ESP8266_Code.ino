/* Changes Needed
 *  -Consolidate and create connect function
 *  -Clean up URL
 *  -Add Docstring
 * 
 * 
 */
//JSON constants
#define ARDUINOJSON_USE_LONG_LONG 1
#define ARDUINOJSON_DEFAULT_NESTING_LIMIT 20

//ESP and JSON Libraries
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <string.h>

//Network Constants
const String networkName = "NAME", password = "PASSWORD";
const String nhlAPI = "statsapi.web.nhl.com", teamID = "3";

//TimeShift Constants
String month = "", day = "", hrs = "";
const int timeZone = -5;
int homeID;
int awayID;
String timeleft = "";

//Game control var
String timeRemaining = "";
bool gameInProg = false;
char period[1];
int gameCode = 0;
String gameURL = "GET https://statsapi.web.nhl.com/api/v1/game/";

//JSON Objects
DynamicJsonDocument nextGame(3527);
DynamicJsonDocument currentGame(4500);
DynamicJsonDocument team(3400);
DynamicJsonDocument gameToday(3000);

//Creates Wifi Client named client
WiFiClient client;


void setup()
{
  //Connect to wifi and api
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  /*Display error messages

     -None
     -Wifi Connection Error

  */
  while (not Serial.available()){}
  //String checkString = stripString(Serial.readString());
  
  //Wifi Timeout Iterator
  //-questionable about use due to stall watchdog
  int iter = 0;

  //Connects to wifi
  //WiFi.hostname
  WiFi.begin(networkName, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    //Waits a minute then gives error
    if (iter > 120) {
      Serial.println("\nWifi Connection Error");
      delay(10000);
    }
    delay(500);
    iter ++;
  }
  //while (not client.connect(nhlAPI, 80)) {}
  /*
     Must make Arduino read till newline so that
     incidental reboot serial transimission is
     filtered

     -Still need to resolve serial error

  */
  //while (not Serial.available()){}
  Serial.println("\nNone");
}





void loop() {
  //Serial.println(gameURL);
  while (not Serial.available()) {}
  //pauses to wait for input
  char input = Serial.read();
  //reads input char from Arduino
  

  switch (input) {
    //Switchboard which can connect commands and strings of commands

    case '?': {
        //is there a game?
        
        //initial test: good
        
        //debug code still there
        client.connect(nhlAPI, 80);
        //Serial.println(gameURL);
        client.println("GET https://statsapi.web.nhl.com/api/v1/schedule?teamId=3");
        client.println("Host: statsapi.web.nhl.com");
        //Serial.println(client.readString());
        DeserializationError error = deserializeJson(gameToday, client);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
    
        }
        const int gamesToday = gameToday["totalGames"];


        if ( gamesToday != 0) {
          const int gameState = gameToday["dates"][0]["games"][0]["status"]["codedGameState"];
          //Serial.println(gameState);
          if (gameState == 3 || gameState == 4) {
            Serial.println("true");
            break;
          }
        }

        Serial.println("false");
        break;
      }


    case 'H': {
        client.connect(nhlAPI, 80);
        //Serial.println(gameURL);
        client.print(gameURL);
        client.print(gameCode);
        client.println("/linescore");
        client.println("Host: statsapi.web.nhl.com");
        //Serial.println(client.readString());

        DeserializationError error = deserializeJson(currentGame, client);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          //return;
        }
        const int homeScore = currentGame["teams"]["home"]["goals"];
        Serial.println(homeScore);
        break;
      }


    case 'A': {
        client.connect(nhlAPI, 80);
        client.print(gameURL);
        client.print(gameCode);
        client.println("/linescore");
        client.println("Host: statsapi.web.nhl.com");
        //Serial.println(client.readString());

        DeserializationError error = deserializeJson(currentGame, client);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          //return;
        }
        const int awayScore = currentGame["teams"]["away"]["goals"];
        Serial.println(awayScore);
        break;
      }


    case 'P': {
        //period
        client.connect(nhlAPI, 80);
        //Serial.println(gameURL);
        client.print(gameURL);
        client.print(gameCode);
        client.println("/linescore");
        client.println("Host: statsapi.web.nhl.com");
        //Serial.println(client.readString());

        DeserializationError error = deserializeJson(currentGame, client);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          //return;
        }
        const int periodConst = currentGame["currentPeriod"];
        //strcpy(period, periodConst);
        Serial.println(periodConst);
        break;
      }


    case 'T': {
        //Time remaining
        /*
          possible returns:
          -# of secs in period
          -"<completed>" if game has ended
          -"<INT>" if in intermission
          -"<completed>" if there  is no current game

        */
        client.connect(nhlAPI, 80);
        //Serial.println(gameURL);
        client.print(gameURL);
        client.print(gameCode);
        client.println("/linescore");
        client.println("Host: statsapi.web.nhl.com");
        //Serial.println(client.readString());

        DeserializationError error = deserializeJson(currentGame, client);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          //return;
        }
        //need to check for game in progress before check
        const char* inInt = currentGame["intermissionInfo"]["inIntermission"];
        const int gameState = gameToday["dates"][0]["games"][0]["status"]["codedGameState"];
        const char* timeRemaining = currentGame["currentPeriodTimeRemaining"];
        
        
        if (timeRemaining == "END" || timeRemaining == "Final") {
          if (inInt == "true") {
            Serial.println("<INT>");
            break;
          }
          else {
            Serial.println("<completed>");
            gameInProg = false;
            break;
          }
        }

        Serial.println(timeRemaining);
        gameInProg = true;


        break;
      }

    case 'G': {
        //game start
        Serial.println("game start");
        gameStart();
        break;
      }

    case 'N': {
        //Get Next Game
        Serial.println("ready");
        getNextGame();
        break;
      }
  }
}





void gameStart() {
  //Creates switchboard for gameStart API
  //gets home and away but also game code
  //https://statsapi.web.nhl.com/api/v1/schedule?teamId=3

  //Connects and gets next game code and team ID

  //works sporatically
  
  client.connect(nhlAPI, 80);
  client.println("GET https://statsapi.web.nhl.com/api/v1/schedule?teamId=3");
  client.println("Host: statsapi.web.nhl.com");
  client.println();
  //Serial.println(client.readString());
  DeserializationError error = deserializeJson(currentGame, client, DeserializationOption::NestingLimit(11));

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  //must be int for path
  const int gameCodeConst = currentGame["dates"][0]["games"][0]["gamePk"];
  gameCode = gameCodeConst;

  //Serial.println(gameCodeConst);
  const int homeTeamId = currentGame["dates"][0]["games"][0]["teams"]["home"]["team"]["id"];
  //Serial.println(homeTeamId);
  homeID = homeTeamId;
  const int awayTeamId = currentGame["dates"][0]["games"][0]["teams"]["away"]["team"]["id"];
  //Serial.println(awayTeamId);
  awayID = awayTeamId;



  bool inProgress = true;
  while (inProgress) {
    //
    while (not Serial.available()) {}
    switch (Serial.read()) {
      case 'H': {
          client.connect("statsapi.web.nhl.com", 80);
          client.println("GET https://statsapi.web.nhl.com/api/v1/teams/" + String(homeTeamId));
          //client.println(homeTeamID);

          client.println("Host: statsapi.web.nhl.com");
          client.println();
          DeserializationError error = deserializeJson(team, client, DeserializationOption::NestingLimit(11));
          //get abbreviation
          const char* homeAbbr = team["teams"][0]["abbreviation"];
          //send it back to Arduino
          Serial.println(homeAbbr);
          //home team
          //Serial.println(Home);
          break;
        }

      case 'A': {
          client.connect("statsapi.web.nhl.com", 80);
          client.println("GET https://statsapi.web.nhl.com/api/v1/teams/" + String(awayTeamId));
          //client.println(homeTeamID);
          client.println("Host: statsapi.web.nhl.com");
          client.println();
          DeserializationError error = deserializeJson(team, client, DeserializationOption::NestingLimit(11));
          const char* awayAbbr = team["teams"][0]["abbreviation"];
          //away team
          Serial.println(awayAbbr);
          gameInProg = true;
          inProgress = false;
          break;
        }
    }
  }
}


//#######################################################################################
//Completed functions


void getNextGame() {
  //Nesting error resulted in helper func.
  //this one creates another switchboard for nextGame API
  //Finished and debug code cleared
  client.connect("statsapi.web.nhl.com", 80);
  client.println("GET https://statsapi.web.nhl.com/api/v1/teams/3?expand=team.schedule.next");
  client.println("Host: statsapi.web.nhl.com");
  client.println();
  DeserializationError error = deserializeJson(nextGame, client, DeserializationOption::NestingLimit(11));

  bool inProgressNext = true;
  while (inProgressNext) {
    while (not Serial.available()) {}
    switch (Serial.read()) {
      case 'H': {
          //Get Team ID
          const int homeTeamID = nextGame["teams"][0]["nextGameSchedule"]["dates"][0]["games"][0]["teams"]["home"]["team"]["id"];
          //Get Team page and write to team
          //Serial.println(String(homeTeamID));
          client.connect("statsapi.web.nhl.com", 80);
          client.println("GET https://statsapi.web.nhl.com/api/v1/teams/" + String(homeTeamID));
          //client.println(homeTeamID);
          client.println("Host: statsapi.web.nhl.com");
          client.println();
          DeserializationError error = deserializeJson(team, client, DeserializationOption::NestingLimit(11));
          //get abbreviation
          const char* homeAbbr = team["teams"][0]["abbreviation"];
          //send it back to Arduino
          Serial.println(homeAbbr);
          //Serial.println(nextGame[);
          break;
        }

      case 'A': {

          const int awayTeamID = nextGame["teams"][0]["nextGameSchedule"]["dates"][0]["games"][0]["teams"]["away"]["team"]["id"];
          //Get Team page and write to team
          //Serial.println(String(awayTeamID));
          client.connect("statsapi.web.nhl.com", 80);
          client.println("GET https://statsapi.web.nhl.com/api/v1/teams/" + String(awayTeamID));
          //client.println(homeTeamID);
          client.println("Host: statsapi.web.nhl.com");
          client.println();
          DeserializationError error = deserializeJson(team, client, DeserializationOption::NestingLimit(11));
          //get abbreviation
          const char* awayAbbr = team["teams"][0]["abbreviation"];
          //send it back to Arduino
          Serial.println(awayAbbr);


          break;
        }

      case 'D': {
          //get date and time
          char DT[20];
          const char *dateTime = nextGame["teams"][0]["nextGameSchedule"]["dates"][0]["games"][0]["gameDate"];
          //DT = dateTime;
          strcpy(DT, dateTime);
          strtok(DT, "-");
          month = strtok(NULL, "-");
          day = strtok(NULL, "T");
          hrs = strtok(NULL, ":");
          String mins = strtok(NULL, ":");
          //Serial.println(month + "/" + day + " " + hrs + ":" + mins);
          timeZoneShift(hrs, day, month);
          Serial.println(month + "/" + day + " " + hrs + ":" + mins);
          break;
        }
      case '0': {
          //reads done
          Serial.println("done");
          inProgressNext = false;
          break;
        }
    }
  }
}



void timeZoneShift(String hour, String Day, String Month) {
  //Translates time based on value given at top
  //finished and debug code cleared

  int intHrs = NULL;
  int intDay = NULL;
  int intMonth = NULL;
  //converts from GMT to specified tz
  intHrs = hour.toInt() + timeZone;
  //Serial.println(intHrs);
  if (intHrs <= 0) {
    intHrs = 12 + intHrs;
    intDay = Day.toInt() - 1;
    //Serial.println(intDay);
    if (intDay <= 0) {
      intMonth = Month.toInt() - 1;
      if ( intMonth == 2) {
        day = "28";
      }
      if ( intMonth == 4 || intMonth == 6 || intMonth == 11) {
        day = "30";
      }
      else {
        day = "31";
      }
      month = String(intMonth);
    }
    else {
      if (intDay < 10) {
        day = "0" + String(intDay);
      }
      else {
        day = String(intDay);
      }
    }
  }

  if (intHrs < 10) {
    hrs = "0" + String(intHrs);
  }
  else {
    hrs = String(intHrs);
  }
  //Serial.println(day + month + hrs);
}

String stripString( String string ) {
  string.replace("\n", "");
  string.replace("\r", "");
  string.replace("\0", "");
  string.trim();
  return string;
}
