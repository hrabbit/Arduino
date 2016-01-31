void setup() 
{
  // initialize both serial ports:
  Serial.begin(115200); // USB port
  Serial1.begin(115200); // To GRBL board
}

void loop() {}

void serialEvent() {
  while (Serial.available())
  {
    Serial1.write(Serial.read());
  }
}

void serialEvent1() {
  while (Serial1.available())
  {
    Serial.write(Serial1.read());
  }
}
