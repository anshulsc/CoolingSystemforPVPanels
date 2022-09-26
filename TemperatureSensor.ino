

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//LCD pins
int lcdV = 8;
int lcdG = 9;

//Temperature Limit
int tempV = 13;
int tempG = 12;
float tempLimit;
int tempControllerPin = A2;
int readTempController;

//Motor Timings
int delayV = 11;
int delayG = 10;
float delayTime;
int delayTimePin = A3;
int readDelayTime;


// Relay Code
const int relay_Pin = 3;
void setup(void)
{
  pinMode(relay_Pin, OUTPUT);
   
//TempController pin mode
pinMode(tempV,OUTPUT);//define a digital pin as output
digitalWrite(tempV, HIGH);// set the above pin as HIGH so it acts as 5V

pinMode(tempG,OUTPUT);//define a digital pin as output
digitalWrite(tempG, LOW);// set the above pin as LOW so it acts as Ground  

//DelayTiming
pinMode(delayV,OUTPUT);//define a digital pin as output
digitalWrite(delayV, HIGH);// set the above pin as HIGH so it acts as 5V

pinMode(delayG,OUTPUT);//define a digital pin as output
digitalWrite(delayG, LOW);// set the above pin as LOW so it acts as Ground

//LCDPower
pinMode(lcdV,OUTPUT);//define a digital pin as output
digitalWrite(lcdV, HIGH);// set the above pin as HIGH so it acts as 5V

pinMode(lcdG,OUTPUT);//define a digital pin as output
digitalWrite(lcdG, LOW);// set the above pin as LOW so it acts as Ground
  
// start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

// Start up the library
  sensors.begin();

  
//LCD
  lcd.init();         // initialize the lcd
  lcd.backlight();    // open the backlight 
}
 
 
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature is: ");
 float temp = sensors.getTempCByIndex(0);  
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
    Serial.print("Temp is ");
    Serial.println(temp);

// //Read Temperature for Setting Limit
  readTempController = analogRead(tempControllerPin);
  Serial.print("PR value is ");
  Serial.println(readTempController);
  tempLimit = (10./1023.)*readTempController + 35;
  Serial.print("tempLimit ");
  Serial.println(tempLimit);

//  Read Time for Setting Motor Time
  readDelayTime = analogRead(delayTimePin);
  Serial.print("PRT value is ");
  Serial.println(readDelayTime);
  delayTime = ((300000.-60000.)/1023.)*readDelayTime + 60000;
  float displayTime = delayTime/1000;
  Serial.print("delayTime : ");
  Serial.println(delayTime );
  //LCD Display Code  
  lcd.clear();  
  lcd.setCursor(0, 0); // start to print at the first row
  lcd.print("CT:");
  lcd.print(temp);    // print the temperature in Celsius
  lcd.print((char)223);      // print ° character
  lcd.print("C ");
  
  
  lcd.setCursor(0, 1);// start to print at the second row
  lcd.print("L:");
  lcd.print(tempLimit); // print the temperature in Fahrenheit
  lcd.print((char)223);
  lcd.print("C ");// print ° character

  lcd.print("M");
  lcd.print("T:");
  lcd.print((int)displayTime);
  lcd.print("s");// print the temperature in Fahrenheit



  
  
    if(temp > tempLimit)
    {
      Serial.println("Temperature above 38");
      digitalWrite(relay_Pin, HIGH); // turn on pump 5 minutes
      delay(delayTime);
      digitalWrite(relay_Pin, LOW);  // turn off pump 5 minuutes
      delay(5000);
    }
   
    delay(100);
}
