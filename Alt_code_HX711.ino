#include "HX711.h"

#define DOUT  A1
#define CLK  A0

int c=0;
float a[50];

HX711 scale(DOUT, CLK,128);


void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale(1);
  scale.tare();
  float c = scale.get_units(10);
  Serial.println(c);

  delay(2000);

  /*long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);*/
}

void loop() {

  
  //float sum=0,mean;
  
   //Adjust to this calibration factor
  
  /*for(int i=0;i<50;i++)
  a[i] = scale.get_units();

  for(int i=0;i<50;i++)
  { sum = sum+a[i]; }  */

  //mean = sum/50;
  
  
  //float z = scale.get_units()+(mean/31.25);

  Serial.print("Reading: ");
  Serial.print(scale.get_units(10), 3);
  Serial.println(" ");
   
  /* Serial.print("MEAN: ");
   Serial.print(mean);
   Serial.println(" ");  */

   //Serial.print("Weight : ");
   //Serial.println(z);
   //Serial.println(" ");
}


