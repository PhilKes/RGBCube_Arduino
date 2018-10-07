#include "SparkFun_Tlc5940.h"
#include <SoftwareSerial.h>
#define DEBUG 1
#define TlcMax 4095

uint8_t layers[CUBE_SIZE] = { A0,A1,A2,A3,A4,A5 };
uint16_t LayerDuration = 3500;
uint16_t FRAME_TIME=300;
uint8_t frameStep=25;

int MAXBRIGHT;
int MAXCOUNT = 400;
int BRIGHTSTEP = 250;
int msgeqReset = A7;
int msgeqStrobe = 4;
int msgeqOut = A6;

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
SoftwareSerial BTserial(8,12); // RX | TX
char c=' ';
String string="";
//#include "font6.h"
void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
	DDRC=B00111111;
	PORTC=B00111111;
	Tlc.init(0);
	MAXBRIGHT=4000;
	brightR=MAXBRIGHT;
	brightG=MAXBRIGHT;
	brightB=MAXBRIGHT;
	color=0;
	curAnim=0;
	Tlc.clearAll();
	BTserial.begin(9600);  
#if DEBUG
  Serial.begin(115200);
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
   #if !DEBUG
      Serial.print("Frame: ");
      Serial.print(FrameCount);
      Serial.print(" ------- ");
      Serial.println(millis()-frameTime);
   #endif
      //AllOff();
      switch(curAnim)
      {
        case 0:randomLeds(30,30,30);//randomLeds(20,20,20);//RGBColorRoom();//ColorCycle();//ValueLed[3][LedBlue[0][1]]=4000;ValueLed[3][LedGreen[0][1]]=4000;ValueLed[3][LedRed[0][1]]=4000;;randomLeds(1,0,0);wallWaag(maxbright,0,0,0,false);wallWaag(0,maxbright,0,1,false);wallSenk(0,0,maxbright,true);
               break;
        case 1:Snake();
               break;      
        case 2:randomLedsFull();//wallSenk(brightR,brightG,brightB,true);//wallWaag(brightR,brightG,brightB,0,true);
               break;       
        case 3:AllRed();//ColorCycle();
               break;       
        case 4:AllBlue();//littleCube();
               break;       
        case 5:AllGreen();//randomLedsFull();
               break;        
        case 6:randomLeds(1,0,0);//aniTest();
               break;
        case 7:randomLeds(20,20,20);
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
   //if(BTserial.available())
   //   BTEvent();
 }
void BTEvent() 
{
  // called when directly received a valid IR signal.
  // do not use Serial inside, it can crash your program!
    string="";
    while (BTserial.available())
    {
      c = ((byte)BTserial.read());
       if(c == 'A')
       {
          while (BTserial.available())
          {
            string += BTserial.read();
          }
          curAnim=string.toInt();
          if(curAnim==6)
          {
           // animation=&ani_font6[0][0];
          //  FRAME_TIME=ANI_FONT6_FRAMETIME;
         //   MAXCOUNT=ANI_FONT6_FRAMES;
          }
       }
         break;
        
    }
    //int a=string.toInt();
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
void msgeqMusic() 
{
	int spectrumValue[8];
	digitalWrite(msgeqReset, HIGH);
	digitalWrite(msgeqReset, LOW);
	for (int i = 0; i < 6; i++)
	{
		digitalWrite(msgeqStrobe, LOW);
		delayMicroseconds(30);
		spectrumValue[i] = analogRead(msgeqOut);
		//  spectrumValue[i]=constrain(spectrumValue[i], filter, 1023);
		digitalWrite(msgeqStrobe, HIGH);
		// spectrumValue[i]=map(spectrumValue[i], filter,1023,0,255);
		//Serial.print(spectrumValue[i]);
	   // Serial.print("\t");
		byte val = 255;
		val = val << (spectrumValue[i] / 127);
		val = ~val;
		/* for (int j=0;j<4;j++)
		  {
			lc.setColumn(i/2,j+(i%2)*4,val);
			 delayMicroseconds(30);
		  }*/
		setPane(i, val);

		/* Serial.print("\t<");
		 Serial.print(val);
		 Serial.print(">\t");
		 */
	}
	//MAXCOUNT = 0;
	FrameCount = 0;
}
