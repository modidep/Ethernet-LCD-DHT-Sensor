// Trigger Version
// LCD Screen connection:
// Green GND on Arduino
// Red 5V on Arduino
// Blue SDA on Arduino
// Green SDC on Arduino
// DHT22 Sensor connection:
// Brown GND on Arduino, 4th pin from left from DHT22
// Red 5V on Arduino, 1st pin from left from DHT22
// Blue SDA on Arduino
// Green SDC on Arduino
// Pot connection:
// 5V Red
// GND Gray
// A1 Green 
// A0 Blue

//load libraries
#include <Wire.h>                 // LCD
#include <LCD.h>                  // LCD
#include <LiquidCrystal_I2C.h>    // LCD
#include <SPI.h>                  // Ethernet
#include <Ethernet.h>             // Ethernet
#include "DHT.h"                  //Temperature & Humidity Sensor
#include <ArduinoJson.h>          //JSON

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char ipAddr[]={' ',' ',' ',' ',' '};
IPAddress fallbackip (192,168,1,177);
EthernetServer server(80);
const size_t bufferSize = JSON_OBJECT_SIZE(2);
const int RLYPIN = 4; // what digital pin relay is connected to
const int BUZZER =9;  // Set Buzzer pin
const int TEMPPOTPIN = A0;
const int HUMDPOTPIN = A1;
bool acon;
int potValue;
#define DHTPIN 2     // what digital pin DHT22 is connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 (Sensor Type)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to DHTPIN
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor (for DHT 11)

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);


//Define variables for LCD Screen 

#define I2C_ADDR          0x27        //Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN      3
#define En_pin             2
#define Rw_pin             1
#define Rs_pin             0
#define D4_pin             4
#define D5_pin             5
#define D6_pin             6
#define D7_pin             7

//Initialise the LCD
LiquidCrystal_I2C      lcd(I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


void setup()
 {
     Serial.begin(9600);  //begin serial
     Serial.println("DHT22 started!");
    
    //Define the LCD as 16 column by 2 rows 
    lcd.begin (16,2);
    Serial.println("LCD Initialized!");
    //Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    dht.begin();
    Serial.println("DHT Initialized!");
    // Initialize relay
    pinMode (RLYPIN,OUTPUT);
    digitalWrite(RLYPIN, HIGH);
    Serial.println(F("Relay initialized."));
    pinMode(BUZZER, OUTPUT); // Set buzzer - pin 9 as an output
    tone(BUZZER, 1000);
    delay(1000);
    noTone(BUZZER);
    Serial.println(F("Buzzer initialized."));
    acon = false;
    acon = false;
    lcd.clear();                           //Clear the LCD
    lcd.setCursor(0,0);                    // Set the cursor on the LCD to Col 1 Row 1
    lcd.print("#NAME# Sensor 1");         // Print on text on the LCD 
    lcd.setCursor(0,1);                    // Set the cursor on the LCD to Col 1 Row 2
    lcd.print("  Sensor  Init");           // Print on text on the LCD
    delay(2000) ;
    /////Ethernet///////////////////////
    lcd.clear();                           //Clear the LCD
    lcd.setCursor(0,0);                    // Set the cursor on the LCD to Col 1 Row 1
    lcd.print("#NAME# Sensor 1");         // Print on text on the LCD 
    lcd.setCursor(0,1);                    // Set the cursor on the LCD to Col 1 Row 2
    lcd.print("  Obtaining IP  ");           // Print IP address on the LCD  
    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      lcd.clear();                           //Clear the LCD
      lcd.setCursor(0,0);                    // Set the cursor on the LCD to Col 1 Row 1
      lcd.print("#NAME# Sensor 1");         // Print on text on the LCD 
      lcd.setCursor(0,1);                    // Set the cursor on the LCD to Col 1 Row 2
      lcd.print("Network problem!");           // Print IP address on the LCD
      delay (2000);
      ///////////////Fixed ip/////////////////////////
      Ethernet.begin(mac, fallbackip);  // initialize Ethernet device
      server.begin();           // start to listen for clients
      Serial.println(F("Server initialized."));
      Serial.print(F("Please connect to http://"));
      Serial.println(Ethernet.localIP());
      // print your local IP address:
      lcd.clear();                           //Clear the LCD
      lcd.setCursor(0,0);                    // Set the cursor on the LCD to Col 1 Row 1
      lcd.print("Fallback Address:");         // Print on text on the LCD
      lcd.setCursor(0,1);                    // Set the cursor on the LCD to Col 1 Row 2
      lcd.print(fallbackip);                         // Print IP address on the LCD
      ////////////////////Fixed ip///////////////////////  
    }
    else {
      Serial.print("IP address: ");    
      for (byte thisByte = 0; thisByte < 4; thisByte++) {
        // print the value of each byte of the IP address:
        ipAddr[thisByte] = Ethernet.localIP()[thisByte];
        Serial.print(Ethernet.localIP()[thisByte], DEC);
        Serial.print("."); 
        }
      IPAddress ip (ipAddr[0],ipAddr[1],ipAddr[2],ipAddr[3]);
      Ethernet.begin(mac, ip);  // initialize Ethernet device
      server.begin();           // start to listen for clients
      Serial.println(F("Server initialized."));
      Serial.print(F("Please connect to http://"));
      Serial.println(Ethernet.localIP());
      // print your local IP address:
      lcd.clear();                           //Clear the LCD
      lcd.setCursor(0,0);                    // Set the cursor on the LCD to Col 1 Row 1
      lcd.print("My IP Address is:");         // Print on text on the LCD
      lcd.setCursor(0,1);                    // Set the cursor on the LCD to Col 1 Row 2
      lcd.print(ip);                         // Print IP address on the LCD  
    }
    delay(3000);
    /////Ethernet/////////////////////////
}
    
void loop()
  {
  // Wait a few seconds between measurements.
  delay(5000);   
  // start the Ethernet connection and the server:

  static float t;
  static float h;
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float rawH = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float rawT = dht.readTemperature();

  //Read potentiometer values for value adjustment
  // int tempOffsetTen = analogRead(TEMPPOTPIN)*10;
  // int humdOffsetTen = analogRead(HUMDPOTPIN)*10;
  int tempOffsetTen = analogRead(TEMPPOTPIN)*40/1024;
  int humdOffsetTen = analogRead(HUMDPOTPIN)*40/1024;
  float tempOffset = 2.0 - tempOffsetTen/10.0;
  float humdOffset = 2.0 - humdOffsetTen/10.0;

  
  Serial.println(tempOffsetTen);
  Serial.println(humdOffsetTen);
    
  t = rawT - tempOffset;
  h = rawH - humdOffset;
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0,0);                    // Set the cursor on the LCD to Col 1 Row 1
    lcd.print("#NAME# Sensor 1");         // Print on text on the LCD 
    lcd.setCursor(0,1);                    // Set the cursor on the LCD to Col 1 Row 2
    lcd.print("!!!DHT  Fail!!!");           // Print on text on the LCD 
    return;
  }
  // Turn relay on and off
  if ((h > 55) && (acon == false)) {
    acon = true;
    RelayOn ();
    delay(500);
    RelayOff ();
  }
  else if ((t > 28) && (acon == false)) {
    acon = true;
    RelayOn ();
    delay(500);
    RelayOff ();
  }
  else if (((t < 28) && (h < 55)) && acon == true) {
    acon = false;
    RelayOn ();
    delay(500);
    RelayOff ();
  }
  // Beep buzzer short if AC is on
  if (acon) {
    tone(BUZZER, 1000);
    delay(200);
    noTone(BUZZER);
  }

  //Serial print Temperature and humidity
  printTempHumAC(t,h,rawT,rawH,tempOffset,humdOffset);
  
  ///////// JSON SERVER ////////////////////
 EthernetClient client = server.available();

  // Do we have a client?
  if (!client) return;

  Serial.println(F("New client"));

  // Read the request (we ignore the content in this example)
  while (client.available()) client.read();

  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<73> jsonBuffer;

  // Create the root object
  JsonObject& root = jsonBuffer.createObject();

  root["temperature"] = t;
  root["humidity"] = h;
  root["AC"] = acon;

  Serial.print(F("Sending: "));
  root.printTo(Serial);
  Serial.println();

  // Write response headers
  client.println("HTTP/1.0 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  // Write JSON document
  root.prettyPrintTo(client);

  // Disconnect
  client.stop();
}

void printTempHumAC(float t, float h, float rawT, float rawH, float tempOffset, float humdOffset) {
    String strt = "";
    String strh = "";
    String stracon = "";
    if (t < 10 && t > -10) {
      strt = String (" " + String(t));
    }
    else {
      strt = String (t);
    }

        if (h < 10 && h > -10) {
      strh = String (" " + String(h));
    }
    else {
      strh = String (h);
    }
    
    if (acon) {
      stracon = "ON";
    }
    else {
      stracon = "OFF";
    }
    
    Serial.print("Humidity: ");
    Serial.print("Raw " + String (rawH) + " + " + "Offset " + String(humdOffset) + " = " + strh);
    Serial.print(" %\t");
    Serial.print("Temp    : ");
    Serial.print("Raw " + String(rawT) + " + " + "Offset " + String(tempOffset) + " = " + strt);
    Serial.print(" °C \n");
    Serial.print("A/C: ");
    Serial.print(stracon);
    Serial.print( "\n");

    
    lcd.clear();                           //Clear the LCD
    lcd.setCursor(0,0);                    // Set the cursor on the LCD to Col 1 Row 1
    lcd.print("Temp    : ");                // Print on text on the LCD
    lcd.setCursor(10,0);                   // Set the cursor on the LCD to Col 11 Row 1
    lcd.print(strt);                       // Print on temperature on the LCD
    lcd.setCursor(14,0);                   // Set the cursor on the LCD to Col 15 Row 1
    lcd.print("\xDF" "C");                 // Print on text on the LCD
    lcd.setCursor(0,1);                    //set the cursor on the LCD to Col 0 Row 2
    lcd.print("Humidity: ");               // Print on text on the LCD
    lcd.setCursor(10,1);                   // Set the cursor on the LCD to Col 11 Row 2
    lcd.print(strh);                       // Print on humidity on the LCD
    lcd.setCursor(14,1);                   // Set the cursor on the LCD to Col 15 Row 2
    lcd.print(" %");                        // Print on text on the LCD
}

void RelayOn () {
    digitalWrite(RLYPIN, LOW);
    Serial.println(F("Relay On"));
}

void RelayOff () {
    digitalWrite(RLYPIN, HIGH);
    Serial.println(F("Relay Off"));
}


