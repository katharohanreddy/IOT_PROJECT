
#include "WiFi.h"
#include "HTTPClient.h"
#include "time.h"
#include <ArduinoJson.h>
char* ssid = "JioFi3_5C3299";
char* password = "93mud7vw7d";
String cse_ip = "192.168.225.76"; // YOUR IP from ipconfig/ifconfig
String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";
String ae = "AKRV";
String cnt = "SONAR";
const int trigPin = 26;
const int echoPin = 25;
WiFiServer server1(80);
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

int distanceFeet;
float remainInch;
// your network password

WiFiClient client;
void createCI(String& val)
{
     HTTPClient http;
     http.begin(server + ae + "/" + cnt + "/");
     http.addHeader("X-M2M-Origin", "admin:admin");
     http.addHeader("Content-Type", "application/json;ty=4");
     int code = http.POST("{\"m2m:cin\": {\"cnf\" : \"application/json\",\"con\": " + String(val) + "}}");
     Serial.println(code);
     if (code == -1) {
     Serial.println("UNABLE TO CONNECT TO THE SERVER");
    }
    http.end();

}


// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

const int pirPin = 34;


const int buzzerPin = 32;

bool x;

// define sound speed in cm/uS

float buzzer_frequency;



void tone(byte pin, int freq) {
  ledcSetup(0, 2000, 8); // setup beeper
  ledcAttachPin(pin, 0); // attach beeper
  ledcWriteTone(0, freq); // play tone
}

void setup()
{
    Serial.begin(115200);     // Starts the serial communication
    pinMode(pirPin, INPUT);   // Sets the pirPin as an Input
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
    pinMode(buzzerPin, OUTPUT); // Sets the buzzerPin as an Input
    pinMode (35, OUTPUT);
    //WiFi.mode(WIFI_STA);
    //ThingSpeak.begin(client); // Initialize ThingSpeak
    Serial.println("Hello");

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop()
{
    tone(buzzerPin, 0);
    
    if ((millis() - lastTime) > timerDelay)
    {
      x = digitalRead(pirPin);
        if (x)
        {
            Serial.print("Motion detected ");
            // Clears the trigPin
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            // Sets the trigPin on HIGH state for 10 micro seconds
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            // Reads the echoPin, returns the sound wave travel time in microseconds
            duration = pulseIn(echoPin, HIGH);

            // Calculate the distance
            distanceCm = duration * SOUND_SPEED / 2;

            // Convert to inches
            distanceInch = distanceCm * CM_TO_INCH;

            // Prints the distance in the Serial Monitor
            Serial.print("at a distance of ");
            Serial.print(distanceCm);
            Serial.println(" cm.");

            if (distanceCm > 250)
            {
              buzzer_frequency = 1;
              tone(buzzerPin, 1);
            }
            else if (distanceCm > 150 and distanceCm <= 250)
            {
              buzzer_frequency = 3;
              tone(buzzerPin, 3);
            }
            else if (distanceCm > 100 and distanceCm <= 150)
            {
              buzzer_frequency = 5;
              tone(buzzerPin, 5);
            }
            else if (distanceCm > 50 and distanceCm <= 100)
            {
              buzzer_frequency = 7;
              tone(buzzerPin, 7);
            }
            else if (distanceCm > 20 and distanceCm <= 50)
            {
              buzzer_frequency = 9;
              tone(buzzerPin, 9);
            }
            else if (distanceCm < 20)
            {
              buzzer_frequency = 11;
              tone(buzzerPin, 11);
            }

            
            Serial.println(buzzer_frequency);


            if (WiFi.status() == WL_CONNECTED)
            {                                  // Check to make sure wifi is still connected
                 String val = String(distanceCm);
                createCI(val);
             // Call the sendData function defined below
            }
            else
            {
                Serial.println("WiFi Disconnected");
            }
            if(distanceCm < 10)
            {
               digitalWrite(35, HIGH);
            }
        }
        else
        {
           Serial.println("Pir No motion detected ");
        }
         
    }

    delay(1000);
}
