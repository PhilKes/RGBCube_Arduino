#include "SparkFun_Tlc5940.h"

#define CUBE_SIZE 6
#define DEBUG 0


uint8_t layer = 0;

uint8_t layers[]={A0,A1,A2,A3,A5,A4};
#include <SoftwareSerial.h>
SoftwareSerial BTserial(8,12); // RX | TX
char c='r';
String string="";

void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  //Serial.begin(9600);
 // BTserial.begin(9600);
   for(int i=0;i<6;i++){
    pinMode(layers[i],OUTPUT);
    digitalWrite(layers[i],HIGH);
    }
 //   digitalWrite(layers[0],LOW);
  Tlc.init(0);
  delay(1);
    
   for(int y=0;y<6;y++)
   {
      for(int x=0;x<CUBE_SIZE;x++)
      {
        //if(y==4 && x==3)
         // break;
        //Tlc.set((x*3)+(y*6*3),2045);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
        //Tlc.set((x*3)+(y*6*3)+1,4095);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
        //Tlc.set((x*3)+(y*6*3)+2,4095);
        //Tlc.set((x*3)+(y*6*3)+2,ValueLed[layer][(x*3+y*CUBE_SIZE*3)+2]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
      }
     
    }
    //Tlc.set(30,4095);
   Tlc.update();
    delay(1);
    digitalWrite(layers[2],LOW);

}
void loop()
{
  
    for(int i=0*16;i<(7)*16;i++)
    {

      Tlc.set(i,2047);
      Tlc.update();
      delay(1);
      delay(333);
    }
    delay(2000);
    Tlc.clear();
    
    /*
    digitalWrite(layers[0],LOW);
    delay(4);
    digitalWrite(layers[0],HIGH);
    digitalWrite(layers[1],LOW);
    delay(4);
    digitalWrite(layers[1],HIGH);
    digitalWrite(layers[2],LOW);
    delay(4);
    digitalWrite(layers[2],HIGH);
    digitalWrite(layers[3],LOW);
    delay(4);
    digitalWrite(layers[3],HIGH);
    digitalWrite(layers[4],LOW);
    delay(4);
    digitalWrite(layers[4],HIGH);
    digitalWrite(layers[5],LOW);
    delay(4);
    digitalWrite(layers[5],HIGH);*/
  /*
  for(int i=0;i<6;i++){
    digitalWrite(layers[i],LOW);
    }
    delay(1000);
    for(int i=0;i<6;i++){
    digitalWrite(layers[i],HIGH);
    }*/

   /*
    switch(c){
    case 'r':AllRed();
            //Serial.println("Red");
            break;
    case 'g':AllGreen();
            break;
    case 'b':AllBlue();
            break;
    default:
            AllGreen();
            AllBlue();
            AllRed();
            break;
    }*/
    //CubeUpdate(layer);
  
    //layer=(layer+1)%6;
   // if(BTserial.available())
   //   BTEvent();
}

void BTEvent() {
  // called when directly received a valid IR signal.
  // do not use Serial inside, it can crash your program!
    string="";
    while (BTserial.available())
    {
      c = ((byte)BTserial.read());
     // Serial.println(c);
       /*if(c == ':')
         break;
       else
        string += c;*/
    }
    /*
    int a=string.toInt();

     if(a>0)
     {
        curAnim=a;
        FrameCount=0;
        brightR=maxbright;
        brightG=maxbright;
        brightB=maxbright;
        if(curAnim==6)
        {
         // animation=&ani_font4[0][0];
          //FRAME_TIME=ANI_FONT4_FRAMETIME;
          //maxCount=ANI_FONT4_FRAMES;
        } 
        else if(curAnim==7)
        {
          FRAME_TIME=100;
          maxCount=0;
        } 
     }*/
}




