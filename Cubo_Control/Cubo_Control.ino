#include "Tlc5940.h"

//DEBUG: 0 no Debug via Serial  1: BT Serial info and test Led ("onn"/"off") 2: All Debug (Frames,Layer,...)
#define DEBUG 0

#define CUBE_SIZE 6
#define TEXTSCROLL_NUM 6
#define FRAME_TIME_DEFAULT 300
uint8_t layers[]={A0,A1,A2,A3,A4,A5};

//FRAME & LAYER
byte curAnim;
//LayerDuration 2860 min.
uint16_t LayerDuration = 2200;
uint16_t FRAME_TIME=FRAME_TIME_DEFAULT;
unsigned long frameTime=0;
uint8_t layer = 0;
uint16_t FrameCount=0;
unsigned long oldMicros = 0;

//MSGEQ7 Music
int spectrumValue[8]={};
int msgeqReset=A7;
int msgeqStrobe=4;
int msgeqOut=A6;
const uint8_t MSGEQSCALE=1024/CUBE_SIZE;

//BRIGHTNESS, ANIMATION UTILITIES
uint16_t maxbright=4000;
int brightR;
int brightG;
int brightB;
byte posX, posY, posZ;
byte color;
int maxCount=400;

//GRAYSCALE VALUES
uint8_t *animation;
uint16_t ValueLed[CUBE_SIZE][CUBE_SIZE*CUBE_SIZE*3];
uint8_t charPosition;

//SERIAL 
char c=' ';
char serialData[20];
String text = "";
//Counter for char Text
uint8_t textChar = 0;

#if DEBUG
  int led =5;
#endif

//EXTERNAL ANIMATIONS
#include "wallsVertical.h"
#include "font6.h"
#include "cube_Grow.h"
#include "waterfall.h"
void setup()
{
  //Set Directions for analog Pins( MSGEQOUT,Layer ANODES)
  DDRC= DDRC|B10111111;
  PORTC=PORTC|B00111111;
  Tlc.init(0);
   brightR=maxbright;
   brightG=maxbright;
   brightB=maxbright;
   color=0;
   curAnim=0;

  //Tlc.clear();
  for(int i=0;i<CUBE_SIZE;i++)  
  	for(int j=0;j<CUBE_SIZE*CUBE_SIZE*3;j++)
  		ValueLed[i][j]=0;
  
	#if DEBUG>0
	 Serial.println("DEBUG");
	#endif  
	pinMode(msgeqReset,OUTPUT);
	pinMode(msgeqStrobe,OUTPUT);
	pinMode(msgeqOut,INPUT);

  Serial.begin(38400);
  //Empty input buffers
  while(Serial.available())
      Serial.read();
}
#if DEBUG>1
 unsigned long micro=0;
#endif
void loop()
{
  //Layer Refresh
  if(micros()-oldMicros>= LayerDuration)// waits until LayerDuration is reached and than goes through cycle
  {
    // micro=micros();     
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
   if(millis()-frameTime>=FRAME_TIME)
   {
     // micro=micros();
  	 #if DEBUG>=2
  		Serial.print("Frame: ");
  		Serial.print(FrameCount);
  		Serial.print("\t: ");
  		Serial.println(millis()-frameTime);
  	 #endif
 
    //AllOff();
    //Serial.println(micros()-micro);
    switch(curAnim)
    {
      case 0:randomLeds(30,30,30);//loadAnimation();//randomLeds(30,30,30);//AllGreen();//AllBlue();AllRed();//randomLeds(20,20,20);//RGBColorRoom();//ColorCycle();//SETLED(1,0,3][1*3+0*CUBE_SIZE*3]=4000;SETLED(1,BLUE,true))
             break;
      case 1:AllRed();
             break;      
      case 2:AllGreen();//wallSenk(brightR,brightG,brightB,true);//wallWaag(brightR,brightG,brightB,0,true);
             break;       
      case 3:AllBlue();//ColorCycle();
             break;       
      case 4:loadAnimation();//littleCube();
             break;       
      case 5:randomLedsFull();
             break;        
      case 6:textScroll();//aniTest();
             break;
      case 7:msgeqMusic();
             break;
	    case 8:RGBColorCycle();
             break;
	    case 9:AllRed(); AllGreen(); AllBlue();
             break;
     case 10:AllOff();
             break;
      default:randomLeds(1,0,0);
             break;   
    }
     FrameCount ++;  
     if(FrameCount>=maxCount){FrameCount=0;}  
     frameTime=millis();
     //Serial.println(micros()-micro);
   }
   //Bluetooth Receive Data
   if (Serial.available())
   {
	   c = (byte)Serial.read();
     //End Command
	   if (c == '\\')
	   {
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
void BTEvent()
{
	/*
        while(Serial.available())
        {
          c=(byte)Serial.read();
          serialData+=c;
        }*/
      #if DEBUG
        Serial.println(String(serialData));
        while(Serial.available())
        {Serial.read();}
      #endif
        //Change Animation
        if(serialData[0]=='A')
        {
            curAnim=String(serialData).substring(1).toInt();
      			brightR = maxbright;
      			brightG = maxbright;
      			brightB = maxbright;
            FRAME_TIME=FRAME_TIME_DEFAULT;
			      FrameCount = 0;
            #if DEBUG
            Serial.print("Switched Anim: ");
			      Serial.println(curAnim);
            #endif
			//WALLS VERTICAL
            if(curAnim==4)
            {
               animation=&ani_wallsvertical[0][0];
               FRAME_TIME=ANI_WALLSVERTICAL_FRAMETIME;
               maxCount=ANI_WALLSVERTICAL_FRAMES;
            }
			//MSGEQ7
            else if(curAnim==7)
            {
              FRAME_TIME=20;
              maxCount=0;
            }
        }
		//TEXT SCROLL
		else if (serialData[0] == 'T') 
		{
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
		}
    #if DEBUG
      if(serialData=="off")
        ledOff();
      else if(serialData=="on")
         ledOn();
    #endif

}
#if DEBUG
  void ledOn()
  {
        analogWrite(led, 100);
        delay(10);
  }
   
  void ledOff()
  {
        analogWrite(led, 0);
        delay(10);
  }
#endif
//MSGEQ7 Spectrum Analyzer
void msgeqMusic(){
  AllOff();
  digitalWrite(msgeqReset, HIGH);
  digitalWrite(msgeqReset, LOW);
  for (int i=0;i<6;i++)
  {
    digitalWrite(msgeqStrobe, LOW);
    delayMicroseconds(30);
    //Read i-th Audio Band value
    spectrumValue[i]=analogRead(msgeqOut);
    digitalWrite(msgeqStrobe, HIGH);
    //
    //Serial.print(spectrumValue[i]/MSGEQSCALE);
    //Serial.print(" ");
  }
  for (int i = 0; i < 6;i++)
	  setPaneVal(i, spectrumValue[i] / MSGEQSCALE);
  //Serial.println();
  // maxCount=0;
}
