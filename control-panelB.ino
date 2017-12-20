
#include <SparkFunMAX31855k.h> // Using the max31855k driver
#include <SPI.h>  // Included here too due Arduino IDE; Used in above header
#include <SoftwareSerial.h>
#include <math.h>

// Define SPI Arduino pin numbers (Arduino Pro Mini)
const uint8_t CHIP_SELECT_PIN = 10; // Using standard CS line (SS)
// SCK & MISO are defined by Arduiino
const uint8_t VCC = 14; 
// Powering board straight from Arduino Pro Mini
const uint8_t GND = 15;
int targetValue;
const int sensorPin = A0; 
const int relayPin = 4;
char currStr[6], targStr[6];
float temperature;
int roundCurr;

// Attach the serial display's RX line to digital pin 2
SoftwareSerial mySerial(3,2); // pin 2 = TX, pin 3 = RX (unused)

// Instantiate an instance of the SparkFunMAX31855k class
SparkFunMAX31855k probe(CHIP_SELECT_PIN, VCC, GND);

void setup() {
  pinMode(sensorPin, INPUT);
  mySerial.begin(9600);
//  Serial.begin(9600);
  delay(500);  // Let IC stabilize or first readings will be garbage
  pinMode(relayPin, OUTPUT);
}

void loop() {

//  // Read the temperature in Fahrenheit
  temperature = probe.readTempF();
  
  if (!isnan(temperature)) {
    mySerial.write(254);
    mySerial.write(128);
    mySerial.write("Curr Temp=");
	  roundCurr = round(temperature);
    sprintf(currStr, "%6d", roundCurr);
    mySerial.write(currStr);
  }
  
  mySerial.write(254); 
  mySerial.write(192);
  
  mySerial.write("Targ Temp=");
  targetValue = map(analogRead(A0), 1023, 0, 100, 400);
  Serial.print("Target before if ");
  Serial.println(targetValue);
  sprintf(targStr, "%6d", targetValue);//?????????
  
  
  mySerial.write(targStr);
  
  targetValue = map(analogRead(A0), 1023, 0, 100, 400);
  temperature = probe.readTempF();
  roundCurr = round(temperature);
  if(!isnan(temperature)){
    if(roundCurr > targetValue){
      digitalWrite(relayPin, HIGH);
    }
    else{
      digitalWrite(relayPin, LOW);
    }
  }

//  targetValue = map(analogRead(A0), 1023, 0, 100, 400);
//    if(s2 > targetValue){
//      digitalWrite(relayPin, HIGH);
//      Serial.print("HIGH, TV: ");
//      Serial.print(targetValue);
//      Serial.print("input: ");
//      Serial.println(s2);
//    }
//    else{
//      digitalWrite(relayPin, LOW);
////      Serial.println("HIGH, TV: " + targetValue + "Input: " + s2);
//    }
  
  delay(750);
}

