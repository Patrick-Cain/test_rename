//https://www.tinkercad.com/things/h4NZdlheCqb-brilliant-stantia/editel?sharecode=Wv8IlPph568swdejD-Q7rNn361zt5UhsJZbEr2PW_h8
//Master
#include <Wire.h>

byte a,b,c,d;
long int Angle_value;
void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  first_slave();
}
void first_slave()
{
  Wire.requestFrom(8, 4);    // request 4 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
  a = Wire.read();
  b = Wire.read();
  c = Wire.read();
  d = Wire.read();

  //Left shift the 32-bits data to rearrange it to ascending order
  Angle_value = a;
  Angle_value = (Angle_value << 8) | b;
  Angle_value = (Angle_value << 8) | c;
  Angle_value = (Angle_value << 8) | d;

    }
  Serial.print("The yaw_angle  is: ");   //Print the value of ldr voltage
  Serial.println(Angle_value);

  delay(1000);
}


//Slave
#include <Wire.h>

unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
long int raw_yaw_angle; //short changed to long int to contain more bits
long int yaw_angle;

// suuss and fail is to check how many packet losses based on checksum
float suss = 0;
float fail = 0;


void serialEvent1() {
  /* Called at the end of loop whenever there is Data in at RX
     Serial1 is TX1 RX1 at Arduino Mega
     TX1 Pin 18
     RX1 Pin 19
     Noticed it is SerialEvent1
  */
  while (Serial1.available()) {
    Re_buf[counter] = (unsigned char)Serial1.read();
    // The fist byte is not the packet header,
    // skip to wait till the packet header to arrive
    if (counter == 0 && Re_buf[0] != 0x55) return;
    counter++;
    if (counter == 11) // All packet received, bool sign = true
    {
      counter = 0;
      sign = 1;
    }
  }
}

void calYawAngle(){
  if (sign){
    sign = 0;
    if (Re_buf[0] == 0x55)   //检查帧头
    {
      int sum = 0;

      for (int x = 0; x < 10; x++)
      {
        sum += Re_buf[x];
      }

      while (sum > 256)
      {
        sum -= 256;
      }
      if ( sum == Re_buf[10] && Re_buf[1] == 83 ) {                       //0x53
        raw_yaw_angle = (Re_buf [7] << 8 | Re_buf [6]);
        yaw_angle = ((raw_yaw_angle<<10)+(raw_yaw_angle<<6)+(raw_yaw_angle<<5)+(raw_yaw_angle<<2)+raw_yaw_angle)>>11;
        Serial.print("Current Angle:");
        Serial.println(yaw_angle);
        suss++;
        //        Serial.print("Failed: ");
        //        Serial.println(fail / suss * 100);
        return;
      }
      else if (sum != Re_buf[10])
      {
        suss++;
        fail++;
        //        Serial.print("Failed:  ");
        //        Serial.println(fail / suss * 100);
        return;
      }
    }
  }
}


void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

  Serial1.begin(115200);
  /*
  Serial1.write(0xFF);
  Serial1.write(0xAA);
  Serial1.write(0x64); //0xFF 0xAA 0x64 changes to JY-61 to 9600 baud rate
  Serial1.end();
  Serial1.begin(9600);
  */
  Serial1.write(0xFF);
  Serial1.write(0xAA);
  Serial1.write(0x52); //0xFF 0xAA 0x52 initialize angle in z Direction = 0

  Serial1.write(0xFF);
  Serial1.write(0xAA);
  Serial1.write(0x66);//vertical installation

  Serial.begin(9600);
}

void loop() {

}

void requestEvent() {
   calYawAngle();
   byte myArray[4];

   //Right Shift operation is done to arrange the 32-bits data in descending order
   //divide the data(32 bits) into 4 pieces(each array store 8 bits) and send to master one by one
   myArray[0] = (yaw_angle >> 24) & 0xFF;
   myArray[1] = (yaw_angle >> 16) & 0xFF;
   myArray[2] = (yaw_angle >> 8) & 0xFF;
   myArray[3] = yaw_angle & 0xFF;

   Wire.write(myArray,4);
}
