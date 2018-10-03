#include "SparkFun_Tlc5940.h"
#define DEBUG 0
#define TlcMax 4095
uint8_t layers[]={A3,A4,A2,A5};
uint16_t LayerDuration = 3500;
uint16_t FRAME_TIME=300;
uint8_t frameStep=25;
int MAXBRIGHT;
int MAXCOUNT = 400;
int BRIGHTSTEP = 250;
byte stretchStep;
unsigned long oldMicros = 0;
int RedCal = 2048;
int GreenCal = -512;
int BlueCal = 768;
unsigned long frameTime = 0;
uint8_t layer = 0;
uint16_t FrameCount = 0;
byte curAnim;
int brightR;
int brightG;
int brightB;
byte posX;
byte posY;
byte posL;
byte color;
uint16_t *animation;
#include <SoftwareSerial.h>
SoftwareSerial BTserial(8,12); // RX | TX
char c=' ';
String string="";
void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
	DDRC=B00111111;
	PORTC=B00111111;
	Tlc.init(0);
	MAXBRIGHT=4090;
	brightR=MAXBRIGHT;
	brightG=MAXBRIGHT;
	brightB=MAXBRIGHT;
	color=0;
	curAnim=0;
	Tlc.clearAll();
	BTserial.begin(9600);  
#if DEBUG
  Serial.begin(9600);
  Serial.println("DEBUG");
#endif
 // Serial.begin(115200);
  //attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_SONY12>, CHANGE);
}
#if DEBUG
 uint32_t micro=0;
#endif
void loop()
{
  if(micros()-oldMicros>= LayerDuration)// waits until LayerDuration is reached and than goes through cycle
  { 
    //Serial.print("new: ");
    //Serial.println(micros()-micro);              
   #if DEBUG
    Serial.print("Layer: ");
    Serial.print(layer);
    Serial.print("  ");
    Serial.println(micros()-oldMicros);
   #endif
     oldMicros=micros();
    if(millis()-frameTime>=FRAME_TIME)
     {
   #if DEBUG
      Serial.print("Frame: ");
      Serial.print(FrameCount);
      Serial.print(" ------- ");
      Serial.println(millis()-frameTime);
   #endif
      AllOff();
      switch(curAnim)
      {
        case 0:Snake();//randomLeds(20,20,20);//RGBColorRoom();//ColorCycle();//ValueLed[3][LedBlue[0][1]]=4000;ValueLed[3][LedGreen[0][1]]=4000;ValueLed[3][LedRed[0][1]]=4000;;randomLeds(1,0,0);wallWaag(maxbright,0,0,0,false);wallWaag(0,maxbright,0,1,false);wallSenk(0,0,maxbright,true);
               break;
        case 1:randomLeds(30,30,30);
               break;      
        case 2:wallSenk(brightR,brightG,brightB,true);//wallWaag(brightR,brightG,brightB,0,true);
               break;       
        case 3:ColorCycle();
               break;       
        case 4:littleCube();
               break;       
        case 5:randomLedsFull();
               break;        
        case 6:aniTest();
               break;
        case 7:aniTest();
               break;
        case 8:AllGreen();AllBlue();AllRed();
               break;
        case 9: AllRed();
               break;
        default:randomLeds(1,0,0);
               break;   
      }
       frameTime=millis();
       FrameCount ++;  
       if(FrameCount>=MAXCOUNT){FrameCount=0;}  
     }
     CubeUpdate(layer);                                                 // sets the values for the tlc5940 Outputs and puts all MOSFET Gates HIGH (not active) except for one MOSFET Low (active) -->this layer is ON, also look under tab "function"  
     layer = (layer + 1) % CUBE_SIZE;                                   // layer counter +1
   }
   if(BTserial.available())
      BTEvent();
 }
void BTEvent() {
  // called when directly received a valid IR signal.
  // do not use Serial inside, it can crash your program!
    string="";
    while (BTserial.available())
    {
      c = ((byte)BTserial.read());
       if(c == ':')
         break;
       else
        string += c;
    }
    int a=string.toInt();
	/*
     if(a>0)
     {
        curAnim=a;
        FrameCount=0;
        brightR=maxbright;
        brightG=maxbright;
        brightB=maxbright;
        if(curAnim==6)
        {
          animation=&ani_font4[0][0];
          FRAME_TIME=ANI_FONT4_FRAMETIME;
          maxCount=ANI_FONT4_FRAMES;
        } 
        else if(curAnim==7)
        {
          animation=&ani_test[0][0];
          FRAME_TIME=ANI_TEST_FRAMETIME;
          maxCount=ANI_TEST_FRAMES;
        } 
     }
	 */
}
