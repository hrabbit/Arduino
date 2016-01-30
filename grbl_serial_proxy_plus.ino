#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

String inputString = "";         // a string to hold incoming data
String lastString = "";
boolean stringComplete = false;  // whether the string is complete

void setup() {

  // initialize both serial ports:
  Serial.begin(115200); // From PC
  Serial1.begin(115200); // To GRBL board

  pinMode(A0, OUTPUT);
  analogWrite(A0, 0);
  
  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight

  lcd.backlight(); // finish with backlight on

  lcd.setCursor(0, 0); //Start at character 4 on line 0
  lcd.print("GRBL Proxy...");

  // Reset GRBL board
  lcd.setCursor(0, 1);
  lcd.print("Resetting GRBL");

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(2, HIGH);

  // A one second pause is just to ensure the other board has completed restarting... hopefully
  delay(2000);
  
  // Show that we are ready to do things
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for GRBL");
}

void loop() {

  
  if (stringComplete) {

    String outputStatus = "";
    String outputX = "";
    String outputY = "";
    String outputZ = "";

    // Get the status first
    outputStatus = inputString.substring(1, inputString.indexOf(','));
      
    // <Idle,MPos:16.000,0.000,0.000,WPos:-84.000,0.000,-2.900>
    // Idle
    // WPos:-84.000,0.000,-2.900
    if (inputString.indexOf('<') == 0 and inputString != lastString)
    {
      
      if(outputStatus == "Alarm")
      {
        analogWrite(A0, 200);
      }
      else
      {
        analogWrite(A0, 0);
      }
      
      String WPos = inputString.substring(inputString.lastIndexOf(':') + 1);
      outputX = WPos.substring(0, WPos.indexOf(','));
      outputY = WPos.substring(WPos.indexOf(',') + 1, WPos.lastIndexOf(','));
      outputZ = WPos.substring(WPos.lastIndexOf(',') + 1, WPos.indexOf('>'));
           
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(outputStatus);

      lcd.setCursor(16 - outputZ.length(), 0);
      lcd.print(outputZ);
      
      lcd.setCursor(0, 1);
      lcd.print(outputX);

      lcd.setCursor(16 - outputY.length(), 1);
      lcd.print(outputY);

      lastString = inputString;
    }
        
    // clear the string:
    inputString = "";
    stringComplete = false;

  }
}

void serialEvent() {
  while (Serial.available())
  {
    Serial1.write(Serial.read());
  }
}

void serialEvent1() {
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
    Serial.write(inChar);
  }
}
