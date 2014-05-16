// Source: http://bildr.org/2011/02/mlx90614-arduino/

// Further reference to the code is given at the Bibliography of the .tex file of the finalReport.

#include <i2cmaster.h>
#include <EEPROM.h>
#include <Servo.h>

Servo servoHOR;
Servo servoVER;

void setup(){
  delay(3000);
  Serial.begin(9600);

  i2c_init(); //Initialise the i2c bus
  PORTC = (1 << PORTC4) | (1 << PORTC5);//enable pullups

  servoVER.attach(6);
  servoHOR.attach(7);
  
}

void loop(){
  int counter = 0;     //Stores index of each point of data
  float tempdata;      //Stores temperature data

  for (int currentX = 30; currentX < 80; currentX++) {    //Sweeps through x values b/w 30 and 80
    servoHOR.write(currentX);      //Sets servo to x position
    for (int currentY = 40; currentY < 90; currentY++) {  //Sweeps through y values b/w 40 and 90
      servoVER.write(currentY);    //Sets servo to y position
      delay(10);
      tempdata = get_temp();       //Returns the temperature in Celsius
      Serial.println(tempdata);    //Prints temperature to serial monitor
      counter++;
    }  
    delay(10);
  }
  Serial.end();
  
  while(1) {}                     //Infinite loop that runs after everything is done
}

float get_temp(){
  //INITIALIZATION
  int dev = 0x5A<<1;  //This is the device address of the sensor
  int data_low = 0;
  int data_high = 0;
  int pec = 0;

  //WRITE MODE
  i2c_start_wait(dev+I2C_WRITE); //Sets device address and write mode
  i2c_write(0x07);               //Write value 0x07 to MLX sensor

  //READ MODE
  i2c_rep_start(dev+I2C_READ);   //Sets device address and read mode
  data_low = i2c_readAck(); //Read 1 byte and then send ack
  data_high = i2c_readAck(); //Read 1 byte and then send ack
  pec = i2c_readNak();
  i2c_stop();  

  //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
  double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
  double tempData = 0x0000; // zero out the data
  int frac; // data past the decimal point

  // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor)-0.01;


  float celsius = tempData - 273.15;
  float fahrenheit = (celsius*1.8) + 32;

  delay(50); // wait a second before printing again

  return celsius;