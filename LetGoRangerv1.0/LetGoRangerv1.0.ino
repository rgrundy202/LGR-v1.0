/*By Ross Grundy
   Jan 3, 2021
   Rangers Goal Horn v 1.0
   Arduino R3 Code
*/

//Libraries
#include <LiquidCrystal.h>
//#include <String.h>

// LCD Constants
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
//Board Layout Constants
const int hornRelay = 6, lightRelay = 13, espRx = 0;

//LCD Setup
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Serial Setup
const int espData = 0;

//Arduino reset function
//void(* resetFunc) (void) = 0;

void setup() {
  //Serial Initialization
  Serial.begin(115200);
  
  // Pin Initialiazation
  pinMode(hornRelay, OUTPUT);
  pinMode(lightRelay, OUTPUT);

  //Welcome Message
  welcome();
  //Network checking code
  errorCheck();
}



void loop() {

  if (isGameNow()) {
    runGame();
    //run Game();
  }
  else {
    getNextGame();
    delay(30000);
    image();
    delay(30000);
  }
}




/*Help Functions
####################################################################
*/



void welcome() {

  //Prints welcome message and connects to serial port

  lcd.begin(16, 2);
  lcd.print("LET'S GO RANGERS");
  lcd.setCursor(5, 1);
  lcd.print("v 1.0");
  delay(2000);
  image();
  delay(2000);
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("For");
  lcd.setCursor(0, 1);
  lcd.print("Connor J. Latona");
  delay(2000);
  lcd.clear();
  //Serial.readStringUntil("\n");
  //Serial.print("error?");
  //Loop holding program till esp8266 begins serial tX

}


void errorCheck() {
  //sends a pulse to check with ESP
  int initCounter = 0; // Var for detecting error
  Serial.readStringUntil('\n');
  Serial.println("check");
  while (not Serial.available()) {
    if (initCounter > 30) {
      //detecting espError
      lcd.home();
      lcd.autoscroll();
      lcd.print("Network Module Serial Not Initialized");
      lcd.clear();
      lcd.noAutoscroll();
    }

    else {
      //prints "Initializing..."
      lcd.home();
      lcd.print("Initializing");
      delay(300);
      lcd.print(".");
      delay(300);
      lcd.print(".");
      delay(300);
      lcd.print(".");
      delay(300);
      lcd.clear();
      initCounter += 1;
    }

  }
  //Serial.readStringUntil("\n");
  
    String errorCode = stripString(Serial.readString());

    if (errorCode != "None") {

      Serial.println("error detected");
      //test code^^^^

      //if error -> print on lcd
      //lcd.home();
      //lcd.autoscroll();
      lcd.print(errorCode);
      delay(600000);

    }
}

  


  void getNextGame() {
    //Gets game data from ESP then prints next game

    Serial.println('N');
    //ESP responds "ready"
    //while (not Serial.available()){}
    while( not Serial.available()){}
    Serial.readString();

    Serial.println('H');
    while (not Serial.available()) {}
    String homeTeam = stripString(Serial.readString());

    Serial.println('A');
    while (not Serial.available()) {}
    String awayTeam = stripString(Serial.readString());

    Serial.println('D');
    while (not Serial.available()) {}
    String date_time = stripString(Serial.readString());
    //date_time format = mm/dd hh:mm

    /*  Displays Standby of next game
        awayTeam     @      homeTeam
                mm/dd hh:mm
    */
    lcd.clear();
    lcd.home();
    lcd.print(awayTeam);
    lcd.setCursor(7, 0);
    lcd.print("@");
    lcd.setCursor(12, 0);
    lcd.print(homeTeam);
    lcd.setCursor(2, 1);
    lcd.print(date_time);
    Serial.println("0");
  }


  int isGameNow() {
    //checks for active game
    Serial.read();
    Serial.println('?');
    while (not Serial.available()) {}
    String isGame = stripString(Serial.readString());
    Serial.println(isGame);
    if (isGame == "true" ) {
      return true;

    }
    Serial.println("false");
    return false;
  }


  void goal() {
    lcd.clear();
    // Possibly add PP goal but KISS
    lcd.setCursor(6, 0);
    lcd.print("GOAL!");
    digitalWrite(hornRelay, HIGH);
    digitalWrite(lightRelay, HIGH);
    delay(4000);
    digitalWrite(hornRelay, LOW);
    delay(1000);
    digitalWrite(hornRelay, HIGH);
    delay(4000);
    digitalWrite(hornRelay, LOW);
    delay(1000);
    digitalWrite(hornRelay, HIGH);
    delay(4000);
    digitalWrite(hornRelay, LOW);
    digitalWrite(lightRelay, LOW);
  }


  void runGame() {
    //game setup code
    char NYR;
    char OPP;
    String OPPname = "";
    String current_period = "0";
    String clock_time = "";
    //Asks for game protocol then gets teams
    Serial.println('G');
    Serial.readString();
    Serial.println('H');
    while (not Serial.available()) {}
    String homeTeam = Serial.readString();
    homeTeam = stripString(homeTeam);
    Serial.println('A');
    while (not Serial.available()) {}
    String awayTeam = Serial.readString();
    awayTeam = stripString(awayTeam);

    //Figures out which team is rangers
    if (homeTeam == "NYR") {
      NYR = 'H';
      OPP = 'A';
      OPPname = awayTeam;
    }

    if (awayTeam == "NYR") {
      NYR = 'A';
      OPP = 'H';
      OPPname = homeTeam;
    }

    //Initiates variables for main game loop
    String NYRScore = "";
    String OPPScore = "";
    String oldNYRScore = "0";
    int scoreThrottle = 10;
    bool gameOn = true;
    String clockMin = "";
    int secNum = 0;
    int minNum = 0;
    String api_time = "";
    String timeleft = "";
    //Serial.println("Light Flash");
    digitalWrite(lightRelay, HIGH);
    delay(1000);
    digitalWrite(lightRelay, LOW);
    delay(1000);
    digitalWrite(lightRelay, HIGH);
    delay(1000);
    digitalWrite(lightRelay, LOW);
    delay(1000);
    digitalWrite(lightRelay, HIGH);
    delay(1000);
    digitalWrite(lightRelay, LOW);
    delay(1000); 

    while (gameOn) {
      //main score update loop

      //Asks for seconds
      Serial.println('T');
      while (not Serial.available()) {}
      
      String clockTime = stripString(Serial.readString());
      /*if (clockTime == api_time){
        secNum --;
        if(secNum < 0){
          secNum = 60 - secNum;
          minNum --;
        }
        
      }
      else{
        minNum = clockTime.substring(2).toInt();
        secNum = clockTime.substring(3,2).toInt();
        Serial.println(minNum, secNum);
      }
      clockTime = api_time;*/
      


          
        if (scoreThrottle == 10) {
          //calls for NYR score
          Serial.println(NYR);
          while (not Serial.available()) {}
          NYRScore = stripString(Serial.readString());

          //Calls for OPP score
          Serial.println(OPP);
          while (not Serial.available()) {}
          OPPScore = stripString(Serial.readString());

          //Runs Goal code if different from last goal
          if (NYRScore != oldNYRScore) {
            goal();
          }
          
          //Update old score
          oldNYRScore = NYRScore;

          //Checks period
          Serial.println('P');
          while (not Serial.available()) {}
          String period = Serial.readString();
          period = stripString(period);
          /*if(period != current_period){
            digitalWrite(lightRelay, HIGH);
            delay(2000);
            digitalWrite(lightRelay, LOW);
          }*/
          //Writes Score
          lcd.clear();
          lcd.print("NYR " + NYRScore);
          lcd.setCursor(10, 0);
          lcd.print(OPPScore + " " + OPPname);
          lcd.setCursor(0, 1);
          lcd.print("Period " + period);
          scoreThrottle = 0;
          if (clockTime == "Final"){
            break;
          }

        }
        scoreThrottle += 1;

        /*LCD Printout
           NYR (Score)      (Score)OPP
           Period #         MM:SS

        */
        //Print Time
        lcd.setCursor(10, 1);
        lcd.print(clockTime);
      }



    }
  


String stripString( String string ) {
    string.replace("\n", "");
    string.replace("\r", "");
    string.trim();
    return string;
  }
  
void image() {
lcd.clear();

byte image08[8] = {B10111, B11111, B11111, B11111, B11111, B11111, B11110, B11111};
byte image09[8] = {B11101, B11111, B11111, B11111, B11111, B11111, B00000, B00000};
byte image10[8] = {B11000, B10000, B10000, B10000, B10000, B10000, B10000, B10000};
byte image07[8] = {B00011, B00001, B00001, B00001, B00001, B00001, B00001, B00001};
byte image26[8] = {B10000, B10000, B10000, B10000, B00000, B00000, B00000, B00000};
byte image25[8] = {B10000, B11000, B11100, B11110, B11111, B11110, B01100, B01000};
byte image23[8] = {B00001, B00001, B00001, B00001, B00000, B00000, B00000, B00000};
byte image24[8] = {B01111, B00111, B00011, B00001, B10000, B01000, B00100, B00010};

lcd.createChar(0, image08);
lcd.createChar(1, image09);
lcd.createChar(2, image10);
lcd.createChar(3, image07);
lcd.createChar(4, image26);
lcd.createChar(5, image25);
lcd.createChar(6, image23);
lcd.createChar(7, image24);

lcd.setCursor(7, 0);
lcd.write(byte(0));
lcd.setCursor(8, 0);
lcd.write(byte(1));
lcd.setCursor(9, 0);
lcd.write(byte(2));
lcd.setCursor(6, 0);
lcd.write(byte(3));
lcd.setCursor(9, 1);
lcd.write(byte(4));
lcd.setCursor(8, 1);
lcd.write(byte(5));
lcd.setCursor(6, 1);
lcd.write(byte(6));
lcd.setCursor(7, 1);
lcd.write(byte(7));
}
