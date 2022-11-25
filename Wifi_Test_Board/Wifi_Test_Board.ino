//Wifi Test code

#include <ESP8266WiFi.h>
#include <base64.h>


void setup()
{
  Serial.begin(115200);
  Serial.println("Sketch Start");

  WiFi.begin("Miniwanca", "t3rminus");

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  WiFiClient client;
  Serial.println("\nStarting connection...");
    // if you get a connection, report back via serial:
    if (client.connect("api.mysportsfeeds.com", 80)) {
      Serial.println("connected");
      // Make a HTTP request:
      //base authString ->4bb7ac65-3a28-4e30-8274-e8f421:MYSPORTSFEEDS
      //encoded auth string -> NGJiN2FjNjUtM2EyOC00ZTMwLTgyNzQtZThmNDIxOk1ZU1BPUlRTRkVFRFM=
      client.println("GET https://api.mysportsfeeds.com/v2.1/pull/nhl/current/games.csv?team=nyr&status=unplayed&limit=1 HTTP/1.1");
      client.println("Host: api.mysportsfeeds.com");
      client.println("Authorization:Basic NGJiN2FjNjUtM2EyOC00ZTMwLTgyNzQtZThmNDIxOk1ZU1BPUlRTRkVFRFM=");
      client.println("Connection: close");
      client.println();
      client.readStringUntil('#');
      client.readStringUntil('\n');
      String response = client.readString();
      Serial.println("response:");
      Serial.println( response);
    } 
}

void loop() {}
