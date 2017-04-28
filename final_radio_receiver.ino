#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver(8000,11,12,10, false);
int loop_count=0;
int ovf_const = 0;

void setup()
{
    Serial.begin(250000); // Debugging only
    if (!driver.init())
         {
          Serial.println("init failed");
         
         while(1)
         {
          Serial.println("x");
          Serial.println(0);
          Serial.println("y");
          Serial.println(0);
          Serial.println("time");
          Serial.println(0);
          
         }
         }
    else{
      Serial.println("i am working");
    }
}
String data_rec; 

void loop()
{  
    int index;
    uint8_t rec_buffer[30];
    uint8_t buflen = sizeof(rec_buffer);
    char data[30];
    int value;
    //int x = atoi(buf);
  
     if (driver.recv(rec_buffer, &buflen)) // Non-blocking
    {
      for(index=0;index<buflen;index++)
        data[index]=(char)rec_buffer[index];
      data[index]='\0';
     Serial.print("data :   "); Serial.println(data);


      // get_data function takes second arg as start delimiter and third arg
      // as ending delimiter to extract the content from the string 
      //value=get_data(data,'r','p')/10000.00;
      //Serial.print("roll: "); Serial.println(value);
      
      //value=get_data(data,'b','t');
      //Serial.println("y"); Serial.println(value);  

      //value=get_data(data,'t','u');      
      //Serial.println("time"); Serial.println(value);

      //value=get_data(data,'u','e');      
      //Serial.println("x"); Serial.println(value);

      //Serial.print("Time at recevier: "); Serial.println((float)millis()/1000);      
    }
    
   
}



 int get_data(char message[],char start_delimiter,char end_delimiter)
 {
  int buflen,index,start_pos,end_pos,new_index;
  buflen=strlen(message);
  char message_data[20];
  
  for(index=0;index<buflen;index++)
    if(message[index]==start_delimiter)
      { 
        start_pos=index; break;
      }
  
  for(index=0;index<buflen;index++)
    if(message[index]==end_delimiter)
      {
        end_pos=index; break;
      }
  start_pos=start_pos+1;
  
  for(index=start_pos,new_index=0;index<end_pos;index++,new_index++)
    message_data[new_index]=(char)message[index];
  
  message_data[new_index]='\0';
  int value =atoi(message_data);
  
  return value;
 }  
