
//Arduino code used to control a RELAY based off the temperature of a THERMAL PROBE

#include <SparkFunMAX31855k.h> // Using the max31855k driver
#include <SPI.h>  // Included here too due Arduino IDE; Used in above header
#include <SoftwareSerial.h>
#include <math.h>

// Define SPI Arduino pin numbers (Arduino Pro Mini)
const uint8_t CHIP_SELECT_PIN = 10; // Using standard CS line (SS)
// SCK & MISO are defined by Arduiino
const uint8_t VCC = 14; 
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

  //Read the temperature in Fahrenheit
  temperature = probe.readTempF();

  //Write temperature
  if (!isnan(temperature)) { //Check num
    mySerial.write(254);
    mySerial.write(128);
    mySerial.write("Curr Temp=");
	  roundCurr = round(temperature);
    sprintf(currStr, "%6d", roundCurr);
    mySerial.write(currStr);
  }
  
  mySerial.write(254); 
  mySerial.write(192);

  //Set and display target temperature
  mySerial.write("Targ Temp=");
  targetValue = map(analogRead(A0), 1023, 0, 100, 400); //Use potentiometer to set this through mapping
  Serial.print("Target before if ");
  Serial.println(targetValue);
  sprintf(targStr, "%6d", targetValue);
  
  mySerial.write(targStr);

  //Reset values after sprintf
  targetValue = map(analogRead(A0), 1023, 0, 100, 400);
  temperature = probe.readTempF();
  roundCurr = round(temperature);

  //Check if temperature is too low or high, turn relay on/off depending on this
  if(!isnan(temperature)){
    if(roundCurr > targetValue){
      digitalWrite(relayPin, HIGH);
    }
    else{
      digitalWrite(relayPin, LOW);
    }
  }

  delay(750);
}

