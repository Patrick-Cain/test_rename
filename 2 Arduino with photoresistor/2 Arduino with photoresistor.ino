//tinkercad: https://www.tinkercad.com/things/7OKXD2HTSMT-fabulous-fyyran/editel?sharecode=orELFcbmalizg9PHzLDCOpqHDFUe2J5a3xZtL-o51Pk
//Master
#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  Wire.requestFrom(8, 1);    // request 1 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    byte c = Wire.read();
    Serial.print("The ldr voltage is: ");   //Print the value of ldr voltage
    Serial.println(c);
  }
  delay(1000);
}


//Slave
#include <Wire.h>
# define ldrpin A0

byte x=0;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  pinMode(ldrpin,INPUT);
}

void loop() {

}

void requestEvent() {
   x=analogRead(ldrpin);
   Wire.write(x);
}
