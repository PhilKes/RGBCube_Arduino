#include <FastLED.h>

#define SETLED(X,Y,L,COLOR,VAL)  ValueLed[(L)][(X) * 3 + (Y)* CUBE_SIZE * 3 + (COLOR)] = (VAL);
//Sets new Values for next layer and sends to TLCs, switches Anodes
 void CubeUpdate(int layerno){
  //unsigned long micro=micros();
  for (int y = 0; y < CUBE_SIZE; y++)
    for (int x = 0; x < CUBE_SIZE; x++){
      /*Tlc.set(LedRed[y][x],ValueLed[layerno][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
      Tlc.set(LedGreen[y][x],ValueLed[layerno][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940
      Tlc.set(LedBlue[y][x],ValueLed[layerno][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940
      */
      Tlc.set(x * 3 + y * CUBE_SIZE * 3, ValueLed[layerno][x * 3 + y * CUBE_SIZE * 3]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
      Tlc.set(x * 3 + 1 + y * CUBE_SIZE * 3, ValueLed[layerno][x * 3 + 1 + y * CUBE_SIZE * 3]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
      Tlc.set(x * 3 + 2 + y * CUBE_SIZE * 3, ValueLed[layerno][x * 3 + 2 + y * CUBE_SIZE * 3]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
    }
  PORTC = PORTC | B00111111;
  Tlc.update();
  delay(1);
  PORTC = PORTC & (B00111111 & ~(1 << layer));
}

//Loads Frame from extern. Animations from PROGMEM into ValueLed Buffer
void loadAnimation(){
  for (int l = 0; l < CUBE_SIZE; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
      for (int x = 0; x < CUBE_SIZE; x++){
            //uint8_t color = pgm_read_word_near(animation + FrameCount * (NUM_TLCS * 16 * CUBE_SIZE) + l * NUM_TLCS * 16 + (x * 3 + y * CUBE_SIZE * 3));
        uint8_t color = pgm_read_byte_near(animation + FrameCount * (CUBE_SIZE*CUBE_SIZE*CUBE_SIZE) + l * (CUBE_SIZE*CUBE_SIZE) + (x + y * CUBE_SIZE));
        uint16_t red = (color >> 5)*redgreenScale;
        uint16_t green = ((color& B00011100) >> 2)*redgreenScale;
        uint16_t blue = (color& B00000011)*blueScale;
        SETLED(x , y ,l,RED, red)
        SETLED(x ,y ,l,GREEN,green)
        SETLED(x ,y ,l,BLUE,blue)
      }
  //Serial.println(micros()-micro);
}

//Load Animation with RGBit coding (each color has 1 bit)
void loadAnimationBit(){
      uint8_t color=0;
      uint8_t x=0;
      uint8_t y=0;
      uint8_t l=0;
      for (int i = 0; i < 81; i++){
       uint8_t curByte = pgm_read_byte_near(animation + FrameCount*81 + i);
       //uint8_t mask=B10000000;
       for (int j = 0; j < 8; j++){
        switch(color){
          case 0:
              SETLED(x , y ,l,RED, ((curByte>>(7-j)&1)*maxBright))
              break;
          case 1:
              SETLED(x ,y ,l,GREEN,((curByte>>(7-j)&1)*maxBright))
              break;
          case 2:
              SETLED(x ,y ,l,BLUE,((curByte>>(7-j)&1)*maxBright))
              break;
          }
       if(++color==3){
          color=0;
          if(++x==CUBE_SIZE){
            x=0;
            if(++y==CUBE_SIZE){
              y=0;
              if(++l==CUBE_SIZE){
                l=0;
              }
            }
          }
        }
       }
      }
}

void testLed(int pin, int layer){/*
   Tlc.set(pin,maxbright);
   Tlc.update();*/
  ValueLed[layer][pin] = 4000;
}
void cubeUp(){
  if(FrameCount==0){
    posX=0;
    posY=0;
    posZ=2;
    maxCount=500;
   }
   else{
    /*
      AllOff();
      drawCube(posZ,posY,posX,3,RED);
      if(posZ+1>CUBE_SIZE-1)
        posZ=2;
      else
         posZ++;*/
       // setXPillar(uint8_t y,uint8_t z,uint8_t xstart,uint8_t width, uint8_t color)
       setXPillar(1,1,2,2, RED);
       setYPillar(3,3,1,4, GREEN);
       setZPillar(4,0,3,3,BLUE);
    }
}
void drawCube(uint8_t positionZ,uint8_t posY,uint8_t posX,uint8_t width,uint8_t color){
  if(positionZ-width+1<0 || posY+width>CUBE_SIZE || posX+width>CUBE_SIZE)
    return;
 for(uint8_t l=positionZ;l>positionZ-width;l--){
   for(uint8_t y=posY;y<posY+width;y++){
    for(uint8_t x=posX;x<posX+width;x++){
      SETLED(x,y,l,color,maxBright)
      //SETLED(x,y,l,GREEN,brightG)
      //SETLED(x,y,l,BLUE,brightB)
    }    
  }
 }
}
//----UTILITIES FOR ANIMATIONS
void AllOff(){
  //micro=micros();
  for (int l = 0; l < CUBE_SIZE; l++)
    for (int i = 0; i < CUBE_SIZE*CUBE_SIZE * 3; i++)
      ValueLed[l][i] = 0;
    // uint16_t *p=ValueLed[0];
     //for (; p< ValueLed+CUBE_SIZE*CUBE_SIZE*CUBE_SIZE*3; p++)
     //     p=0;
 // memset(ValueLed[0],0,sizeof(ValueLed[0][0])*CUBE_SIZE*CUBE_SIZE*CUBE_SIZE*3);
  //Serial.println(micros()-micro);
}
void setXPillar(uint8_t y,uint8_t z,uint8_t xstart,uint8_t width, uint8_t color){
  if(xstart+width-1>CUBE_SIZE-1)
    return;
  for (int x = xstart; x < xstart+width; x++)
      SETLED(x, y, z, color, maxBright)
}
 void setYPillar(uint8_t x, uint8_t z,uint8_t ystart,uint8_t width, uint8_t color){
  if(ystart+width-1>CUBE_SIZE-1)
    return;
  for (int y = ystart; y < ystart+width; y++)
    SETLED(x, y, z, color, maxBright)
}
 void setZPillar(uint8_t x, uint8_t y,uint8_t zstart,uint8_t width, uint8_t color){
  if(zstart+width-1>CUBE_SIZE-1)
    return;
  for (int z = zstart; z < zstart+width;z++)
    SETLED(x, y, z, color, maxBright)
}
void AllInit(){
  for (int i = 0; i < NUM_TLCS*16; i++){
    Tlc.set(i, ValueLed[0][i]);
  }
  Tlc.update();
  digitalWrite(layers[0], LOW);  //layer 1
}
void AllRed(){
  for (int l = 0; l < CUBE_SIZE; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
      for (int x = 0; x < CUBE_SIZE; x++)
        SETLED(x, y ,l,RED, maxBright)
}
void AllGreen(){
  for (int l = 0; l < CUBE_SIZE; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
      for (int x = 0; x < CUBE_SIZE; x++)
        SETLED(x,y ,l,GREEN,maxBright)
}
void AllBlue(){
  for (int l = 0; l < CUBE_SIZE; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
      for (int x = 0; x < CUBE_SIZE; x++)
        SETLED(x, y ,l,BLUE, maxBright)
}
void setXRow(uint8_t y,uint8_t z, uint8_t color){
  for (int x = 0; x < CUBE_SIZE; x++)
      SETLED(x, y, z, color, maxBright)
}
 void setYRow(uint8_t x, uint8_t z, uint8_t color){
  for (int y = 0; y < CUBE_SIZE; y++)
    SETLED(x, y, z, color, maxBright)
}
 void setZRow(uint8_t x, uint8_t y, uint8_t color){
  for (int z = 0; z < CUBE_SIZE;z++)
    SETLED(x, y, z, color, maxBright)
}
void setPaneVal(int pane, int val){
  for (int l = 0; l < val; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
      SETLED(pane, y, l, RED, maxBright)
  for (int l = val; l < CUBE_SIZE; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
     SETLED(pane, y, l, RED, 0)

  //ValueLed[l][+ y * CUBE_SIZE * 3] = val; //RED ONLY
   // ValueLed[l][x*3+1+y*CUBE_SIZE*3]=val;
   //ValueLed[l][x*3+2+y*CUBE_SIZE*3]=val;
}

void setPaneVal(int pane, int val,uint8_t color){
  for (int l = 0; l < val; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
      SETLED(pane, y, l, color, maxBright)
  for (int l = val; l < CUBE_SIZE; l++)
    for (int y = 0; y < CUBE_SIZE; y++)
     SETLED(pane, y, l, color, 0)

  //ValueLed[l][+ y * CUBE_SIZE * 3] = val; //RED ONLY
   // ValueLed[l][x*3+1+y*CUBE_SIZE*3]=val;
   //ValueLed[l][x*3+2+y*CUBE_SIZE*3]=val;
}
void setXYPane(uint8_t z,uint8_t color){
  for (int y = 0; y < CUBE_SIZE;y++)
    for (int x = 0; x< CUBE_SIZE; x++)
      SETLED(x, y, z, color, maxBright)
}
void setXZPane(uint8_t y, uint8_t color){
  for (int z = 0; z < CUBE_SIZE; z++)
    for (int x = 0; x< CUBE_SIZE; x++)
      SETLED(x, y, z, color, maxBright)
}
void setYZPane(uint8_t x, uint8_t color){
  for (int z = 0; z < CUBE_SIZE; z++)
    for (int y = 0; y < CUBE_SIZE; y++)
      SETLED(x, y, z, color, maxBright)
}
//-----ANIMATIONS
void randomLedsFull(){
  AllOff();
  for (uint8_t j = 0; j < CUBE_SIZE; j++){
    for (uint8_t x = 0; x < CUBE_SIZE; x++){
      for (uint8_t y = 0; y < CUBE_SIZE; y++){
        uint8_t rand = random8(3);
        SETLED(x,y,j,rand,maxBright);
        /*
        if (rand == BLUE)
          //Blue
          SETLED(x ,y ,j,BLUE,maxbright)
        else if (rand == 1)
          //Green
          SETLED(x ,y ,j,GREEN,maxbright)
        else if (rand == 2)
          //Red
          SETLED(x ,y ,j,RED,maxbright)
         */
      }
    }
    FrameCount = 0;
  }
}
void randomLeds(int red, int green, int blue){
  AllOff();
  for (int p = red; p > 0; p--)
    SETLED(random8(CUBE_SIZE) , random8(CUBE_SIZE),random8(CUBE_SIZE),RED,maxBright)
  for (int p = green; p > 0; p--)
    SETLED(random8(CUBE_SIZE) , random8(CUBE_SIZE),random8(CUBE_SIZE),GREEN,maxBright)
  for (int p = blue; p > 0; p--)
    SETLED(random8(CUBE_SIZE) , random8(CUBE_SIZE),random8(CUBE_SIZE),BLUE,maxBright)
  FrameCount = 0;
}
void RGBColorRoom(){  
    for(int l=0;l< CUBE_SIZE;l++)
      for(int y=0;y< CUBE_SIZE;y++)
        for(int x=0;x< CUBE_SIZE;x++){
          SETLED(x,y,l,RED,100+x*1300)
          SETLED(x,y,l,GREEN,100+l*800)
          SETLED(x,y,l,BLUE,50+y*500)
        }
    FrameCount=0;
}
//Shows Text,no scrolling
void textShow(){
  //WHITE SPACE
  if(text.charAt(textChar)==32){
    AllOff();
   }
   else{
    FrameCount = text.charAt(textChar) - 65;
    #if DEBUG==2
    Serial.print("Char: ");
    Serial.print(text.charAt(textChar));
    Serial.print(" | ");
    Serial.println(FrameCount);
    #endif
    loadAnimation();
   }
  charPosition++;
  FrameCount= charPosition;
  textChar = (textChar + 1) % text.length();
}
//Scrolls through text sent via BT
void textScroll(){
  if(FrameCount>0){
    int y=0;
    for(int l=0;l<CUBE_SIZE;l++)
      for(int x=1;x<CUBE_SIZE;x++){
        //COLOR IMPORTANT
        SETLED(x-1,y,l,ValueLed[l][x*3+y*3*CUBE_SIZE],BLUE);  
      }
      uint8_t ch = text.charAt(textChar) - 65;
      for(int l=0;l< CUBE_SIZE;l++){
          //load First Pane of Character
          //loadAnimation();
          uint8_t color = pgm_read_byte_near(animation + ch * (CUBE_SIZE*CUBE_SIZE*CUBE_SIZE) + l * (CUBE_SIZE*CUBE_SIZE) + (charPosition + 0 * CUBE_SIZE));
          uint16_t red = (color >> 5)*redgreenScale;
          //uint16_t green = ((color& B00011100) >> 2)*redgreenScale;
         // uint16_t blue = (color& B00000011)*blueScale;
          SETLED(CUBE_SIZE-1 ,0 ,l,RED,red)
          //SETLED(CUBE_SIZE-1 ,0 ,l,BLUE,blue)
          //SETLED(CUBE_SIZE-1 ,0 ,l,BLUE,blue)
        }
       charPosition++;
      if(FrameCount%6==0){
      //textChar= actual character
        textChar = (textChar + 1) % text.length();
        charPosition=0;
      }
    }
  else{
    FrameCount = text.charAt(0) - 65;
    loadAnimation();
   // FrameCount = 1;
  }
}
void ColorCycle(){
  if(FrameCount==0){
    maxCount=31;
    brightR=0;
    brightG=0;
    brightB=maxBright;
  }
  else if(FrameCount<11){
    brightR+=400;
    if(brightB>=400){brightB-=400;}
  }
  else if(FrameCount<21){
    brightG+=400;
    brightR-=400;
  }
  else if(FrameCount<31){
    brightB+=400;
    brightG-=400;
  }
  //else if(FrameCount==31){FrameCount=0;}
  AllRed();
  AllGreen();
  AllBlue();
}
void RGBColorCycle(){
  if (FrameCount == 0){
    brightR = maxBright;
    brightG = 0;
    brightB = 0;
  }
   else if (FrameCount< FRAMECYCLES)
    brightG += cyclestep;
   else if (FrameCount< 2*FRAMECYCLES)
    brightR -= cyclestep;
  else if (FrameCount< 3*FRAMECYCLES)
    brightB += cyclestep;
  else if (FrameCount< 4*FRAMECYCLES)
    brightG -= cyclestep;
  else if (FrameCount< 5*FRAMECYCLES)
    brightR += cyclestep;
  else if (FrameCount< 6*FRAMECYCLES)
    brightB -= cyclestep;
  else
    FrameCount = 0;
  //else if(FrameCount==31){FrameCount=0;}
  AllRed();
  AllGreen();
  AllBlue();
}
void Snake(){
  if (FrameCount == 0)maxCount = 383;
  if (FrameCount >= 0){
    if (FrameCount == 0)maxCount = 383;
    if (FrameCount >= 0){
      int temp = FrameCount;
      if (temp > 63)temp = 63;
      for (int i = 0; i <= temp / (CUBE_SIZE*CUBE_SIZE); i++){
        for (int j = 0; j <= temp % (CUBE_SIZE*CUBE_SIZE); j++)
          SETLED(j%CUBE_SIZE , j / CUBE_SIZE ,i,RED, maxBright)
        if (i > 0)
          for (int k = 0; k < i; k++)
            for (int x = 0; x < CUBE_SIZE; x++)
              for (int y = 0; y < CUBE_SIZE; y++)
                SETLED(x , y ,k,RED, maxBright)
      }
    }
  }
  if (FrameCount > 63){
    int temp = FrameCount - 63;
    if (temp > 63)temp = 63;
    for (int i = 0; i <= temp / 16; i++){
      for (int j = 0; j <= temp % 16; j++)
        SETLED(j % 4 ,j / 4 ,i,GREEN,maxBright)
      if (i > 0)
        for (int k = 0; k < i; k++)
          for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
              SETLED(x ,y ,k,GREEN,maxBright)
    }
  }
  if (FrameCount > 127){
    int temp = FrameCount - 127;
    if (temp > 63)temp = 63;
    for (int i = 0; i <= temp / 16; i++){
      for (int j = 0; j <= temp % 16; j++)
        SETLED(j % 4 ,j / 4 ,i,BLUE,maxBright)
      if (i > 0)
        for (int k = 0; k < i; k++)
          for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
              SETLED(x ,y ,k,BLUE,maxBright)
    }
  }
  if (FrameCount > 191){
    int temp = FrameCount - 191;
    if (temp > 63)temp = 63;
    for (int i = 0; i <= temp / 16; i++){
      for (int j = 0; j <= temp % 16; j++)
        SETLED(j % 4 , j / 4 ,i,RED, 0)
      if (i > 0)
        for (int k = 0; k < i; k++)
          for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
              SETLED(x , y ,k,RED, 0)
    }
  }
  if (FrameCount > 255){
    int temp = FrameCount - 255;
    if (temp > 63)temp = 63;
    for (int i = 0; i <= temp / 16; i++){
      for (int j = 0; j <= temp % 16; j++)
        SETLED(j % 4 ,j / 4 ,i,GREEN,0)
      if (i > 0)
        for (int k = 0; k < i; k++)
          for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
              SETLED(x ,y ,k,GREEN,0)
    }
  }
  if (FrameCount > 319){
    int temp = FrameCount - 319;
    if (temp > 63)temp = 63;
    for (int i = 0; i <= temp / 16; i++){
      for (int j = 0; j <= temp % 16; j++)
        SETLED(j % 4 ,j / 4 ,i,BLUE,0)
      if (i > 0)
        for (int k = 0; k < i; k++)
          for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
              SETLED(x ,y ,k,BLUE,0)
    }
  }
    //if(FrameCount==383)
    //  FrameCount=0;
}
void littleCube(){
  AllOff();
  switch(FrameCount){
    case 0:
  posX=1;
  posY=1;
  posZ=2;
         break;
    case 1:  
    posZ--;
              break;
    case 2:  
    posX++;
              break;
    case 3:
    posY--;
           break;
    case 4:
    posX--;
            break;
    case 5:
    posX--;
            break;
    case 6:
    posY++;
              break;
   case 7:
    posY++;
              break;
    case 8:
    posX++;
              break;
   case 9:
    posX++;
              break;
   case 10:
    posZ++;
              break;
   case 11:
    posY--;
              break;
  case 12:
    posY--;
              break;
  case 13:
    posX--;
              break;
  case 14:
    posX--;
              break;
   case 15:
    posY++;
              break;
  case 16:
    posY++;
              break;
  case 17:
    posX++;
              break;
case 18:
    posX++;
              break;
case 19:
    posZ++;
              break;
case 20:
    posY--;
              break;
case 21:
    posY--;
              break; 
case 22:
    posX--;
              break; 
case 23:
    posX--;
              break;  
 case 24:
    posY++;
              break;  
case 25:
    posY++;
              break;  
case 26: 
    posX++;
              break; 
case 27:
    posY--;
            FrameCount=0;
              break;        
    default:
                break;                
  }
  littleCube(posZ,posY,posX);
}
void Cubes4(){
  if(FrameCount==0){  
    brightR=maxBright;
    brightG=0;
    brightB=0;
    littleCube(1,0,2);    //Rot
    brightR=0;
    brightG=maxBright;
    brightB=0;
    littleCube(3,2,2);    //Grün
    brightR=0;
    brightG=0;
    brightB=maxBright;
    littleCube(1,2,0);    //Blau
    brightR=maxBright/2;
    brightG=0;
    brightB=maxBright/2;
    littleCube(1,0,0);    //Rot+Blau
    brightR=maxBright/2;
    brightG=maxBright/2;
    brightB=0;
    littleCube(3,0,2);    //Rot+Grün
    brightR=0;
    brightG=maxBright/2;
    brightB=maxBright/2;
    littleCube(3,2,0);    //Blau+Grün
    brightR=maxBright;
    brightG=maxBright/2;
    brightB=maxBright/4;
    littleCube(3,0,0);    //Rot+Grün+(blau+rot)
    brightR=maxBright/2;
    brightG=maxBright;
    brightB=maxBright/4;
    littleCube(1,2,2);    //Grün+Blau+(grün+rot)
  }
      else if(FrameCount==1){ 
    brightR=maxBright;
    brightG=0;
    brightB=0;
    littleCube(3,2,2);    //Rot
    brightR=0;
    brightG=maxBright;
    brightB=0;
    littleCube(1,0,2);    //Grün
    brightR=0;
    brightG=0;
    brightB=maxBright;
    littleCube(1,0,0);    //Blau
    brightR=maxBright/2;
    brightG=0;
    brightB=maxBright/2;
    littleCube(1,2,0);    //Rot+Blau
    brightR=maxBright/2;
    brightG=maxBright/2;
    brightB=0;
    littleCube(3,2,0);    //Rot+Grün
    brightR=0;
    brightG=maxBright/2;
    brightB=maxBright/2;
    littleCube(3,0,2);    //Blau+Grün
    brightR=maxBright;
    brightG=maxBright/2;
    brightB=maxBright/4;
    littleCube(1,2,2);    //Rot+Grün+(blau+rot)
    brightR=maxBright/2;
    brightG=maxBright;
    brightB=maxBright/4;
    littleCube(3,0,0);    //Grün+Blau+(grün+rot)
  }
else if(FrameCount==2){ 
    brightR=maxBright;
    brightG=0;
    brightB=0;
    littleCube(1,2,0);    //Rot
    brightR=0;
    brightG=maxBright;
    brightB=0;
    littleCube(1,0,0);    //Grün
    brightR=0;
    brightG=0;
    brightB=maxBright;
    littleCube(1,0,2);    //Blau
    brightR=maxBright/2;
    brightG=0;
    brightB=maxBright/2;
    littleCube(3,2,2);    //Rot+Blau
    brightR=maxBright/2;
    brightG=maxBright/2;
    brightB=0;
    littleCube(3,0,0);    //Rot+Grün
    brightR=0;
    brightG=maxBright/2;
    brightB=maxBright/2;
    littleCube(1,2,0);    //Blau+Grün
    brightR=maxBright;
    brightG=maxBright/2;
    brightB=maxBright/4;
    littleCube(3,0,2);    //Rot+Grün+(blau+rot)
    brightR=maxBright/2;
    brightG=maxBright;
    brightB=maxBright/4;
    littleCube(3,2,0);    //Grün+Blau+(grün+rot)
    FrameCount=0;
  }
}

void littleCube(int positionZ,int posY,int posxX){
  for(int l=positionZ;l>posZ-2;l--){
   for(int y=posY;y<posY+2;y++){
    for(int x=posX;x<posX+2;x++){
      SETLED(x,y,l,RED,brightR)
      SETLED(x,y,l,GREEN,brightG)
      SETLED(x,y,l,BLUE,brightB)
    }    
  }
 }
}  
