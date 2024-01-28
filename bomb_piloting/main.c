#include <Servo.h>
#include <Wire.h>
#include <math.h>

const int SCC433TK03_ADDRESS = 0x68;  // Replace with the correct I2C address of SCC433T-K03
const float GYRO_SENSITIVITY; 

//Defining servomotors
Servo servoA;
Servo servoB;
Servo servoC;
Servo servoD;

void setup() 
{
  Wire.begin();
}

void setup() 
{
  // Attach servos to corresponding pins
  servoA.attach(9);
  servoB.attach(10);
  servoC.attach(11);
  servoD.attach(12);

  // Set initial positions (adjust as needed)
  servoA.write(0);
  servoB.write(0);
  servoC.write(0);
  servoD.write(0);
}

void loop() 
{
  // Request data from SCC433T-K03
  Wire.beginTransmission(SCC433TK03_ADDRESS);
  Wire.write(0x22);  // Replace with the register address to read from
  Wire.endTransmission();

  // Read data from SCC433T-K03
  Wire.requestFrom(SCC433TK03_ADDRESS, 6);  // Adjust the number based on the expected data size
  while (Wire.available() < 6);

  // Read sensor data
  int16_t gyroZ = Wire.read() << 8 | Wire.read();

  // Convert raw gyro values to degrees per second (adjust scaling based on datasheet)
  // Replace GYRO_SENSITIVITY with the correct scaling factor
  float gyroZ_deg = gyroZ / GYRO_SENSITIVITY;
  float gyroZ_rad = gyroZ_deg * (M_PI / 180.0);


  delay(1000);  // Adjust delay based on your required update rate

  if (gyroZ_rad > 0)
  {
    x = x*cos(gyroZ_rad) - y*sin(gyroZ_rad); //x and y are obtained from the camera tracking
  }
   
  if (gyroZ_rad < 0)
  {
    y = -x*sin(gyroZ_rad) + y*cos(gyroZ_rad);
  }

  // Move servos to desired positions (adjust as needed)
  moveServo(servoA, 0);
  moveServo(servoB, 90);
  moveServo(servoC, 180);
  moveServo(servoD, 270);

  delay(2000); // Wait for 2 seconds

  // Move servos back to initial positions
  moveServo(servoA, 90);
  moveServo(servoB, 90);
  moveServo(servoC, 90);
  moveServo(servoD, 90);

  delay(2000); // Wait for 2 seconds

  
}
