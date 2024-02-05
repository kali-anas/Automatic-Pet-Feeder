#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "index.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define LED 14

const long utcOffsetInSeconds = 10800;
Servo myservo;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


const char* ssid = "tan";
String timer;
String sensor_value;
const char* password = "ukulele123";
unsigned long myTime;
int pos;

ESP8266WebServer server(80);


void handleRoot()

{
  String s = webpage;
  server.send(200, "text/html", s);
}


void sensor_data() {
  int weightReading = analogRead(A0);
 sensor_value = String(weightReading);
  server.send(200, "text/plane", sensor_value);
}
void serve(){
    for (pos = 90; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
  {                                   // in steps of 1 degree
    myservo.write(pos);               // tell servo to go to position in variable 'pos'
    delay(5);                        // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 90; pos -= 1)  // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(55);           // waits 15ms for the servo to reach the position
  }
}

void led_control() {

  timeClient.update();

  String state = server.arg("state");
  Serial.print(state);
  if (state == "1") {
    digitalWrite(LED, HIGH);  //LED ON
    timer = String(timeClient.getFormattedTime());
    serve();
    server.send(200, "text/plane", timer);
  } 
  else {
    digitalWrite(LED, LOW);  //LED OFF
  }
  Serial.print(state);
}


void setup(void)

{
  myservo.attach(5);
   myservo.write(0);

 
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.println("");

  pinMode(LED, OUTPUT);

  timeClient.begin();

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting...");
  }

  Serial.println("");

  Serial.print("Connected to ");

  Serial.println(ssid);

  Serial.print("IP address: ");

  Serial.println(WiFi.localIP());



  server.on("/", handleRoot);

  server.on("/led_set", led_control);

  server.on("/adcread", sensor_data);

  server.begin();
}


void loop(void)
{
  server.handleClient();
}
