#include "Tlc5940.h"

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

#define CUBE_SIZE 6
#define DEBUG 0
int spectrumValue[8];
uint8_t layers[]={A0,A1,A2,A3,A4,A5};
float CubeBrig = 0.10;
float CubeSat = 1.00;
int CubeRes = 2047;

int RedCal = 2048;
int GreenCal = -512;
int BlueCal = 768;
#define TlcMax 4095
byte curAnim;
uint16_t LayerDuration = 800;
uint16_t FRAME_TIME=300;
uint8_t frameStep=25;
unsigned long frameTime=0;
uint8_t layer = 0;
uint16_t FrameCount=0;
unsigned long oldMicros = 0;

int msgeqReset=A7;
int msgeqStrobe=4;
int msgeqOut=A6;

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
   //BTserial.begin(9600);  
#if DEBUG
  //Serial.begin(9600);
 // Serial.println("DEBUG");
#endif
 // Serial.begin(115200);
  //attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_SONY12>, CHANGE);
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
        case 0:mappingfull();//AllGreen();//AllBlue();AllRed();//randomLeds(20,20,20);//RGBColorRoom();//ColorCycle();//ValueLed[3][LedBlue[0][1]]=4000;ValueLed[3][LedGreen[0][1]]=4000;ValueLed[3][LedRed[0][1]]=4000;;randomLeds(1,0,0);wallWaag(maxbright,0,0,0,false);wallWaag(0,maxbright,0,1,false);wallSenk(0,0,maxbright,true);
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
        case 7:msgeqMusic();
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
  // if(BTserial.available())
 //     BTEvent();
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
     }
       /*
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
  */
}
void msgeqMusic(){
  digitalWrite(msgeqReset, HIGH);
  digitalWrite(msgeqReset, LOW);
   for (int i=0;i<6;i++)
  {
    digitalWrite(msgeqStrobe, LOW);
    delayMicroseconds(30);
    spectrumValue[i]=analogRead(msgeqOut);
  //  spectrumValue[i]=constrain(spectrumValue[i], filter, 1023);
    digitalWrite(msgeqStrobe, HIGH);
    // spectrumValue[i]=map(spectrumValue[i], filter,1023,0,255);
    //Serial.print(spectrumValue[i]);
   // Serial.print("\t");
    byte val=255;
    val=val << (spectrumValue[i]/127);
    val=~val;
  /* for (int j=0;j<4;j++)
    {
      lc.setColumn(i/2,j+(i%2)*4,val);
       delayMicroseconds(30);
    }*/
    setPane(i,val);

   /* Serial.print("\t<");
    Serial.print(val);
    Serial.print(">\t");
    */
  }
   maxCount=0;
}

void setPane(int pane, int val){


  for(int l=0;l<CUBE_SIZE;l++)
  {
     for(int y=0;y<CUBE_SIZE;y++)
    {
    
        ValueLed[l][pane*3+y*CUBE_SIZE*3]=val; //RED ONLY
       // ValueLed[l][x*3+1+y*CUBE_SIZE*3]=val;
       //ValueLed[l][x*3+2+y*CUBE_SIZE*3]=val;
      
   }
  }
  
}





