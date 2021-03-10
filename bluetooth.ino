// Load libraries
#include <Servo.h>
#include "BluetoothSerial.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

Servo myservo;  
int pos = 0;

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// Handle received and sent messages
char incomingChar;
float tempC;

unsigned long previousMillis = 0;    // Stores last time temperature was published
const long interval = 10000;         // interval at which to publish sensor readings


void setup() {
   lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  Serial.begin(115200);
  // Bluetooth device name
  SerialBT.begin("ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");
   myservo.attach(13);
   Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
 lcd.setCursor(0,0);
lcd.print(" COVID-19 ");
delay(2000);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
     previousMillis = currentMillis;
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");
    tempC = sensors.getTempCByIndex(0);
    Serial.println(tempC);  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp:  ");
    lcd.setCursor(0,1);
    lcd.print(tempC);
  }
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    Serial.println(incomingChar);
    
    if (incomingChar == '0' or tempC > 40)
    {
      Serial.println("gate close");
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gate close");
    }
    else
    {
    Serial.println("gate open");  
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gate open");
    myservo.write(90);              // tell servo to go to position in variable 'pos'
    delay(1000);
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    lcd.clear();
    //delay(1000);
    }
  //delay(20);
}
}
