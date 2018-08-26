#include "Tlc5940.h"
#include "IRLremote.h"

#define IRL_BLOCKING true
#define pinIR 2
uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;
uint8_t LedRed[4][4]=
{{42,38,33,29},
{46,37,26,30},
{5,40,21,22},
{2,9,12,19}};

uint8_t LedGreen[4][4]=
{{47,39,32,31},
{43,41,34,25},
{0,7,11,17},
{1,6,13,15}};

uint8_t LedBlue[4][4]=
{{45,35,27,28},
{44,36,23,24},
{3,8,18,20},
{4,10,14,16}};

#define CUBE_SIZE 4
#define DEBUG 0
uint8_t layers[]={A3,A4,A2,A5};
float CubeBrig = 0.10;
float CubeSat = 1.00;
int CubeRes = 2047;

int RedCal = 2048;
int GreenCal = -512;
int BlueCal = 768;
#define TlcMax 4095
byte curAnim;
uint16_t LayerDuration = 3500;
uint16_t FRAME_TIME=1000;
uint8_t frameStep=25;
unsigned long frameTime=0;
uint8_t layer = 0;
uint16_t FrameCount=0;
unsigned long oldMicros = 0;

int maxbright;
int brightR;
int brightG;
int brightB;
byte posX;
byte posY;
byte posL;
byte color;
int maxCount=400;
int brightStep=250;
byte stretchStep;
int *animation;
uint16_t ValueLed[CUBE_SIZE][CUBE_SIZE*CUBE_SIZE*3];
#include "test.h"
#include "font4.h"

void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  DDRC=B00111100;
  PORTC=B00111100;
  Tlc.init(0);
  maxbright=4090;
   brightR=maxbright;
   brightG=maxbright;
   brightB=maxbright;
   color=0;
   curAnim=0;

  //Tlc.clear();
  for(int i=0;i<CUBE_SIZE;i++)  
  	for(int j=0;j<CUBE_SIZE*CUBE_SIZE*3;j++)
  		ValueLed[i][j]=0;
     
  checkBright();
#if DEBUG
  Serial.begin(115200);
  Serial.println("DEBUG");
#endif
 // Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_SONY12>, CHANGE);
}
 uint32_t micro=0;
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
       if(FrameCount>=maxCount){FrameCount=0;}  
       
     }
     CubeUpdate(layer);                                                 // sets the values for the tlc5940 Outputs and puts all MOSFET Gates HIGH (not active) except for one MOSFET Low (active) -->this layer is ON, also look under tab "function"  
     layer = (layer + 1) % CUBE_SIZE;                                   // layer counter +1
   }
 }


void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
  // called when directly received a valid IR signal.
  // do not use Serial inside, it can crash your program!
  uint8_t oldSREG = SREG;
  // dont update value if blocking is enabled
  if (IRL_BLOCKING && !IRProtocol) 
  {

    switch(command)
    {
      case 0x0:curAnim=1;              //NUM1
                  FrameCount=0;
                  break;
      case 0x1:curAnim=2;            //NUM2
                  FrameCount=0;
                  break;            
      case 0x2:curAnim=3;             //NUM3
                  brightB=maxbright;
                  FrameCount=0; 
                  break;
      case 0x3:curAnim=4;              //NUM4
                  FrameCount=0;
                  break;
      case 0x4:curAnim=5;              //NUM5
                  FrameCount=0;
                  break;            
      case 0x5:curAnim=6;              //NUM6
                  FrameCount=0;
                  animation=&ani_font4[0][0];
                  FRAME_TIME=ANI_FONT4_FRAMETIME;
                  maxCount=ANI_FONT4_FRAMES;
                  break;            
      case 0x6:curAnim=7;              //NUM7#
                  animation=&ani_test[0][0];
                  FRAME_TIME=ANI_TEST_FRAMETIME;
                  maxCount=ANI_TEST_FRAMES;
                  FrameCount=0;
                  break;            
      case 0x7:curAnim=8;              //NUM8
                  brightR=maxbright;
                  brightG=maxbright;
                  brightB=maxbright;
                  FrameCount=0;
                  break;
      case 0x8:curAnim=9;             //NUM9
                  FrameCount=0;
                  break;
      case 0x9:curAnim=0;             //NUM0
                  brightR=maxbright;
                  FrameCount=0;
                  break;
      case 0x12:
                   switch(color)    //LAUT+
                   {
                     case 0:if((brightR+brightStep)<=maxbright){brightR+=brightStep;} //Rot 
                                                 
                            break;
                     case 1:if((brightG+brightStep)<=maxbright){brightG+=brightStep;} //Grün                      
                            break;
                     case 2:if((brightB+brightStep)<=maxbright){brightB+=brightStep;} //Blau                      
                            break; 
                     default:
                             break;            
                   }
                   break;
      case 0x13:
                    switch(color)  //LAUT-
                   {
                     case 0:if((brightR-brightStep)>=0){brightR-=brightStep;} //Rot                      
                            break;
                     case 1:if((brightG-brightStep)>=0){brightG-=brightStep;} //Grün                      
                            break;
                     case 2:if((brightB-brightStep)>=0){brightB-=brightStep;} //Blau                      
                            break; 
                     default:
                             break;      
                   }  
                  break;
      case 0x10:  if(FRAME_TIME+frameStep<=2000){FRAME_TIME+=frameStep;}            //P+
                  break;
      case 0x11:     if(FRAME_TIME-frameStep>65){FRAME_TIME-=frameStep;}         //P-
                  break;
      case 0x4C:color=0;      //Rot
                  break;
      case 0x4D:color=1;      //Grün
                  break;
      case 0x4F:color=2;      //Blau
                  break;            
      case 0x15:              //POWER
                  break;
      case 0x718E:              //FWD
                 // if(stretch+stretchStep<=200){stretch+=stretchStep;}
                  break;
      case 0x708F:              //BACKWD
                 // if(stretch-stretchStep>0){stretch-=stretchStep;}
                  break;             
      default:
                  break;      
    }
    IRProtocol = 0;
  }
  SREG = oldSREG;
}







