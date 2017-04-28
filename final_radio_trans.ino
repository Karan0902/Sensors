#include <RH_ASK.h>
#include <SPI.h> 
#include <Wire.h>
#include <BMP085.h>
//#include <Adxl345_Lib.h>

//ADXL345 adxl;
BMP085 bmp;
//AccelerometerScaled ax;

const int SSIZE = 5;

float hi, hi_p, hi_o, hi1;
//float ac, ac_p, ac_o;
//float vel;

float hi_pa, hi_sa;
//float ac_pa, ac_sa;

float hi_kal, hi_var, hi_gin, hi_per;
float ac_kal, ac_var, ac_gin, ac_per;
float kal_pnoise;

//float cum_var_hi[SSIZE], cum_var_ac[SSIZE];

//time keeping
unsigned long t = 0, dt1;
unsigned long iter_no;
double dt = 0;

RH_ASK driver;
int loop_count=0;
char trans_data_parts[10];
char final_string[50];

void setup() 
{
  

  digitalWrite(11, HIGH);
  pinMode(11, OUTPUT);
Serial.begin(250000);
 
 if (!driver.init())
 {
//{    Serial.println("init failed");
  while(1);
}


  if (!bmp.begin(0)) 
  {
//  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  digitalWrite(11, LOW);
  while(1);
  }
  else
  {
//  delay(100);
  bmp.calibrate();
//  Serial.println("BMP085 initialized and ready");
  delay(500);
  }

   float baro_var[50];

   for(int i=0; i<50; i++ )
   {
     baro_var[i] = bmp.calcpressureheight();
   }

   hi_var = variance(baro_var, 50);
   hi_sa  = pa_map(hi_var);

//  adxl.ADXL345_set();
//  adxl.accel_constant(2048,2112,2048,2048,1792,2240);
////   adxl.accel_constant(2048*(1/9.8),2112*9.8,2048*9.8,2048*9.8,1856*9.8,2176*9.8);
//
//   float accel_var[50];
//
//   for(int i=0; i<50; i++)
//   {
//     ax = adxl.scalevalue();
//     accel_var[i] = ax.ZAxis;
//   }
//
//   ac_var = variance(accel_var, 50);
//   ac_sa = pa_map(ac_var);

  iter_no = 0;

//  hi = hi_p = hi_o = hi1 = 0;
//  hi_pa = 1;
//  for(int i=0; i<SSIZE; i++)
//  cum_var_hi[i] = 1;

//  ac = ac_p = ac_o = 0;
//  ac_pa = 1;
//  for(int i=0; i<SSIZE; i++)
//  cum_var_ac[i] = 1;

  hi_kal = ac_kal = 0;
  hi_per = ac_per = 0.5;
  hi_gin = ac_gin = 0.5;
  kal_pnoise = 0.2;
  
//  vel = 0;
  t = millis();
  delay(60);

  strcpy(final_string,"");
  Serial.print("data  :   "); Serial.println(final_string);
}







 
void loop() 
{

 dt= (millis()-t)/1000.0;
 dt1=millis()-t;
 t =  millis();
 //Serial.println(dt1);
//  ax = adxl.scalevalue();
 
  hi_o = bmp.calcpressureheight();
//  ac_o = (ax.ZAxis) + 0.9714;

//  ac_o  = iter_no%5; 
//  hi_o += vel*dt + 0.5*ac*pow(dt,2);

//  Serial.println();
//  Serial.println();
//  Serial.println();
//  Serial.println();
//  Serial.println();
//  Serial.print("dt: ");
//  Serial.println(dt,10);
//  Serial.print("hi_o, ac_o:  ");
//  Serial.println(hi_o, 10);
//  Serial.print(", ");
//  Serial.println(ac_o, 10);

  filt_estimate();
  filt_update();

//  upd_mean(hi_o, hi);
//  upd_variance(hi_o, hi);  



//radiohead starts

// Serial.print("hi_kal: ");
// Serial.println(hi_kal);
 int32_t data_for_kal=hi_kal*100;
// Serial.println(data_for_kal);
 sprintf(trans_data_parts, "b%d",data_for_kal);
// Serial.println(trans_data_parts);
 
 
 strcat(final_string, trans_data_parts);

 sprintf(trans_data_parts, "t%d", dt1);
 strcat(final_string,trans_data_parts);

  sprintf(trans_data_parts, "u%de", hi_o);
 strcat(final_string,trans_data_parts);
 Serial.print("lol data  :   "); Serial.println(final_string);
// Serial.println("\n");
// Serial.print("data sent:  ");
// Serial.println(final_string);
// Serial.println("\n");
if(loop_count==4)
{
  Serial.print("data  :   "); Serial.println(final_string);
 driver.send((uint8_t *)final_string, strlen(final_string));
 driver.waitPacketSent();
 strcpy(final_string,"");
 
 loop_count=0;
}
loop_count++;
// radiohead  ends
 }



 
void filt_estimate()
{
// hi_p = hi + vel*dt + (0.5*ac_o*pow(dt, 2));
//  ac_p = (2.0*(hi_o - hi - vel*dt))/pow(dt, 2);

  hi_per = hi_per + kal_pnoise;
  ac_per = ac_per + kal_pnoise;
  
//  Serial.print("hi_p, ac_p: ");
//  Serial.print(hi_p, 10);
//  Serial.print(", ");
//  Serial.println(ac_p, 10);
}


void filt_update()
{
//  float hi_err;
//  float ac_err;
//
//  hi1 = hi;
//
//  //MyFilter 1 dependent on sensor accuracy
//  hi = ((hi_sa)*hi_o + (2.0-hi_sa)*hi_p) / 2.0;
//  ac = ((ac_sa)*ac_o + (2.0-ac_sa)*ac_p) / 2.0;
//
//  //MyFilter 2 dependent on prediction accuracy
//  //hi = ((2.0-hi_pa)*hi_o + (hi_pa)*hi_p) / 2.0;
//  //ac = ((2.0-ac_pa)*ac_o + (ac_pa)*ac_p) / 2.0;

  //Kalman Filter Stuff
  
  hi_gin = hi_per / (hi_per + hi_var);
//  ac_gin = ac_per / (ac_per + ac_var);

  hi_kal = hi_kal + hi_gin*(hi_o - hi_kal);
//  ac_kal = ac_kal + ac_gin*(ac_o - ac_kal);

  hi_per = (1 - hi_gin) * hi_per;
//  ac_per = (1 - ac_gin) * ac_per;

//  vel = (hi - hi1 - 0.5*ac*dt*dt) / dt;
//
//  Serial.print("hi, ac, vel:  ");
//  Serial.print(hi, 10);
//  Serial.print(", ");
//  Serial.print(ac, 10);
//  Serial.print(", ");
//  Serial.println(vel, 10);
//
//  Serial.print("hi_kal, ac_kal,: ");
//  Serial.println(hi_kal, 10);
//  Serial.print("Time: ");
//  Serial.println(millis());
//  Serial.print(", ");
//  Serial.println(ac_kal, 10);
//
//  hi_err = abs(hi - hi_p);
//  ac_err = abs(ac - ac_p);
//
//  hi_pa = pa_update(hi_err, &cum_var_hi[0]);
//  ac_pa = pa_update(ac_err, &cum_var_ac[0]);
//
//  Serial.print("hi_pa, ac_pa: ");
//  Serial.print(hi_pa, 10);
//  Serial.print(", ");
//  Serial.println(ac_pa, 10);
//
//  Serial.print("iter: ");
//  Serial.println(iter_no);
//  iter_no++;
}

//float pa_update(float pe, float * cum_var)
//{
//  // *cum_var = ((*cum_var * iter_no) + pow(pe, 2)) / (iter_no + 1.0);
//
//  *(cum_var + (iter_no%SSIZE)) = pow(pe, 2);
//
//  float var = 0;
//
//  for(int i = 0; i<SSIZE; i++)
//  {
//    var+= *(cum_var+i);
//    Serial.println(*(cum_var+i));
//  }
//
//  Serial.println();
//  Serial.println();
//
//  var /= SSIZE;
//
//  // Serial.print("cum_var: ");
//  // Serial.println(var);
//
//  return pa_map(var);
//
//}

float pa_map(float cum_var)
{
  float sd = sqrt(cum_var);

  return (sd <= 1) ? (2 - sd) : (1 / sd);
}

float variance(float a[], int n)
{
  float sum = 0, m, var = 0;
  m = mean(a, n);
  for(int i=0; i<n; i++)
  var += (a[i] - m) * (a[i] - m);
  var /= (float)n;
  return var;
}

float mean(float a[], int n)
{
  float sum = 0;
  for(int i=0; i<n; i++)
  sum+=a[i];
  return sum/(float)n;
}

