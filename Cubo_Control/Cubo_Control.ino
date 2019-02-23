#include "Tlc5940.h"

//DEBUG: 0 no Debug via Serial  1: BT Serial info and test Led ("onn"/"off") 2: All Debug (Frames,Layer,...)
#define DEBUG 0

#define CUBE_SIZE 6

#define TEXTSCROLL_NUM 99
#define FRAME_TIME_DEFAULT 170
#define FRAME_TIME_MIN 20
//#define MAXBRIGHT 4000
#define MAXBRIGHT 255
#define SPEED_DIV 50.0
#define FRAMECYCLES 20

#define MSGEQRESET A7
#define MSGEQSTROBE 4
#define MSGEQOUT A6

#define RED 2
#define GREEN 1
#define BLUE 0

//EXTERNAL ANIMATIONS
//#include "wallsVertical.h"
#include "font6.h"
//#include "cube_Grow.h"
#include "waterfallRGBit.h"
#include "cubeBuildRGBit.h"
#include "cubeGrowRGBit.h"
//#include "waveContinuous.h"

uint8_t layers[]={A0,A1,A2,A3,A4,A5};

//FRAME & LAYER
uint8_t curAnim;
//LayerDuration 2860 min.
uint16_t LayerDuration = 1500;
uint16_t FRAME_TIME=FRAME_TIME_DEFAULT;
//Default Frame Times for all Animations (see switch case for numbers)

unsigned long frameTime=0;
uint8_t layer = 0;
uint16_t FrameCount=0;
unsigned long oldMicros = 0;

//MSGEQ7 Music
int spectrumValue[8]={0,0,0,0,0,0,0,0};
const uint8_t MSGEQSCALE=1024/CUBE_SIZE;

//BRIGHTNESS, ANIMATION UTILITIES
uint16_t maxBright= MAXBRIGHT;
int brightR;
int brightG;
int brightB;
byte posX=0, posY=0, posZ=0;
byte posRed=0,posGreen=0,posBlue=0;
byte color;
int maxCount=400;

//GRAYSCALE VALUES
uint8_t *animation;
uint8_t ValueLed[CUBE_SIZE][CUBE_SIZE*CUBE_SIZE*3];
uint8_t charPosition;

//SERIAL 
char c=' ';
char serialData[20];
String text = "";
//Counter for char Text
uint8_t textChar = 0;

int blueScale = maxBright / 3;
int redgreenScale = maxBright / 7;
uint8_t cyclestep = maxBright / FRAMECYCLES;
#if DEBUG
  int led =5;
#endif

void setup(){
  //Set Directions for analog Pins( MSGEQOUT,Layer ANODES)
  DDRC= DDRC|B10111111;
  PORTC=PORTC|B00111111;
  Tlc.init(0);
   brightR=maxBright;
   brightG=maxBright;
   brightB=maxBright;
   color=0;
   curAnim=0;

  //Tlc.clear();
  for(int i=0;i<CUBE_SIZE;i++)  
    for(int j=0;j<CUBE_SIZE*CUBE_SIZE*3;j++)
      ValueLed[i][j]=0;
        
  #if DEBUG>0
   Serial.println("DEBUG");
  #endif  
  pinMode(MSGEQRESET,OUTPUT);
  pinMode(MSGEQSTROBE,OUTPUT);
  pinMode(MSGEQOUT,INPUT);

  Serial.begin(38400);
  //Empty input buffers
  while(Serial.available())
      Serial.read();
}

#if DEBUG>1
 unsigned long micro=0;
#endif
void loop(){
  //Layer Refresh
  if(micros()-oldMicros>= LayerDuration){
    // Verbose Debug info Layers
   #if DEBUG==3
    Serial.print("Layer: ");
    Serial.print(layer);
    Serial.print("\t :");
    Serial.println(micros()-oldMicros);
   #endif
     CubeUpdate(layer);                                                 // sets the values for the tlc5940 Outputs and puts all MOSFET Gates HIGH (not active) except for one MOSFET Low (active) -->this layer is ON, also look under tab "function"  
     layer = (layer + 1) % CUBE_SIZE;                                   // layer counter +1
     oldMicros=micros();
   }
   
   //Frame refresh
   if(millis()-frameTime>=FRAME_TIME){
     // Verbose Debug info Frames
     #if DEBUG>=2
      Serial.print("Frame: ");
      Serial.print(FrameCount);
      Serial.print("\t: ");
      Serial.println(millis()-frameTime);
     #endif
 
   // AllOff();
    //Select current animation for next Frame
    switch(curAnim){
      case 0:AllOff();cubeGrow(FrameCount);cubeGrow((FrameCount+17)%34);//randomLeds(30,30,30);//loadAnimation();//randomLeds(30,30,30);//AllGreen();//AllBlue();AllRed();//randomLeds(20,20,20);//RGBColorRoom();//ColorCycle();//SETLED(1,0,3][1*3+0*CUBE_SIZE*3]=4000;SETLED(1,BLUE,true))
             break;
      case 1:AllRed();
             break;      
      case 2:AllGreen();//wallSenk(brightR,brightG,brightB,true);//wallWaag(brightR,brightG,brightB,0,true);
             break;       
      case 3:AllBlue();//ColorCycle();
             break;       
      case 4:rain();//cube grow littleCube();
             break;       
      case 5:randomLedsFull();
             break;        
      case 6:cubeGrow(FrameCount);
             break;  
      case 7:msgeqMusic();
             break;
      case 8:loadAnimationBit();//loadAnimation();//waterfall //RGBColorCycle();
             break;
      case 9:loadAnimationBit(); //AllRed(); AllGreen(); AllBlue();
             break;
     case 10:loadAnimationBit();
             break;
     case 99:textShow();//aniTest();
           break;
      default:randomLeds(1,0,0);
             break;   
    }
     FrameCount ++;  
     if(FrameCount>=maxCount){FrameCount=0;}  
     frameTime=millis();
   }
   
   //Bluetooth Receive Data
   if (Serial.available()){
     c = (byte)Serial.read();
     //End Command
     if (c == '\\'){
       BTEvent();
       //Reset serial buffer
       strcpy(serialData,"");
     }
     else
      //Add char to serial Command
      strncat(serialData,&c,1);
   }
}
 
//Execute Command
void BTEvent(){
      //Verbose Debug Info Bluetooth Command
      #if DEBUG
        Serial.println(String(serialData));
        while(Serial.available()){Serial.read();}
      #endif
        //Change Animation
        if(serialData[0]=='A'){
            curAnim=String(serialData).substring(1).toInt();
            brightR = maxBright;
            brightG = maxBright;
            brightB = maxBright;
            FRAME_TIME=FRAME_TIME_DEFAULT;
            FrameCount = 0;
           //IMPORTANT!
            AllOff();
            #if DEBUG
            Serial.print("Switched Anim: ");
            Serial.println(curAnim);
            #endif
      //CUBE GROW
            if(curAnim==8){
               animation=&ani_cubegrowrgbit[0][0];
               FRAME_TIME=ANI_CUBEGROWRGBIT_FRAMETIME;
               maxCount=ANI_CUBEGROWRGBIT_FRAMES;
            }
      //WAVE SINGLE
			else if(curAnim==9){
               animation=&ani_waterfallrgbit[0][0];
               FRAME_TIME=ANI_WATERFALLRGBIT_FRAMETIME;
               maxCount=ANI_WATERFALLRGBIT_FRAMES;
            }
			else if(curAnim==10){
                animation=&ani_cubebuildrgbit[0][0];
               FRAME_TIME=ANI_CUBEBUILDRGBIT_FRAMETIME;
               maxCount=ANI_CUBEBUILDRGBIT_FRAMES;
            }
      //MSGEQ7
            else if(curAnim==7){
              FRAME_TIME=50;
              maxCount=0;
            }
        }
        
    //TEXT SHOW
    else if (serialData[0] == 'T'){
      AllOff();
      curAnim=TEXTSCROLL_NUM;
      FrameCount = 0;
      text = String(serialData).substring(1);
      textChar = 0;
      charPosition=0;
      #if DEBUG
      Serial.print("Text: ");
      Serial.println(text);
      #endif
      animation = &ani_font6[0][0];
      FRAME_TIME = ANI_FONT6_FRAMETIME;
      //maxCount = ANI_FONT6_FRAMES+1;
      maxCount=(text.length()-1);
    }
   
   //TEXT SCROLL
   /*
    else if (serialData[0] == 'T'){
      curAnim=TEXTSCROLL_NUM;
      FrameCount = 0;
      text = String(serialData).substring(1);
      textChar = 1;
      charPosition=0;
      #if DEBUG
      Serial.print("Text: ");
      Serial.println(text);
      #endif
      animation = &ani_font6[0][0];
      FRAME_TIME = ANI_FONT6_FRAMETIME;
      //maxCount = ANI_FONT6_FRAMES+1;
      maxCount=(text.length()-1)*6;
    }*/
    //BRIGHTNESS
    else if (serialData[0] == 'B'){
      maxBright = String(serialData).substring(1).toInt();
      if (maxBright > MAXBRIGHT || maxBright < 0)
        maxBright = MAXBRIGHT;
      blueScale = maxBright / 3;
      redgreenScale = maxBright / 7;
      cyclestep=maxBright / FRAMECYCLES;
    }
    //SPEED
    else if (serialData[0] == 'F'){
      //Break if curAnim is Music Spectrum
      if (curAnim != 7){
        int div = String(serialData).substring(1).toInt();
        //FRAME_TIME = ANIM_TIMES[curAnim] * (SPEED_DIV / div);
        FRAME_TIME = FRAME_TIME_DEFAULT* (SPEED_DIV / div);
        if (FRAME_TIME < FRAME_TIME_MIN)
          FRAME_TIME = FRAME_TIME_MIN;
      }
    }
    //Debug Bluetooth Testing
    #if DEBUG
      if(serialData=="off")
        ledOff();
      else if(serialData=="on")
         ledOn();
    #endif

}
//Debug methods for testing Bluetooth
#if DEBUG
  void ledOn(){
        analogWrite(led, 100);
        delay(10);
  }
   
  void ledOff(){
        analogWrite(led, 0);
        delay(10);
  }
#endif

//MSGEQ7 Spectrum Analyzer
void msgeqMusic(){
  //AllOff();
  digitalWrite(MSGEQRESET, HIGH);
  digitalWrite(MSGEQRESET, LOW);
  for (int i=0;i<5;i++){
    digitalWrite(MSGEQSTROBE, LOW);
    delayMicroseconds(30);
    //Read i-th Audio Band value
    spectrumValue[i]=analogRead(MSGEQOUT);
    spectrumValue[i] = constrain(spectrumValue[i], 0, 1023);
    //spectrumValue[i] = map(spectrumValue[i], 0, 1023, 0, 4095);
    digitalWrite(MSGEQSTROBE, HIGH);
    Serial.print(spectrumValue[i]);
    spectrumValue[i] = map(spectrumValue[i], 0, 1023,0,6);
    Serial.print(" ");
  }
  Serial.println();
  spectrumValue[5]=spectrumValue[4];
  
  //for (int i = 0; i < 6;i++)
    setPaneVal(0, spectrumValue[0],RED);
    setPaneVal(1, spectrumValue[1],RED);
    setPaneVal(2, spectrumValue[2],GREEN);
    setPaneVal(3, spectrumValue[3],GREEN);
    setPaneVal(4, spectrumValue[4],BLUE);
    setPaneVal(5, spectrumValue[5],BLUE);
    
}
