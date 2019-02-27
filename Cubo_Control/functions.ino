#include <FastLED.h>

#define SETLED(X,Y,L,COLOR,VAL)  ValueLed[(L)][(X) * 3 + (Y)* CUBE_SIZE * 3 + (COLOR)] = (VAL);
//Sets new Values for next layer and sends to TLCs, switches Anodes
 void CubeUpdate(int layerno){
  Tlc.clear();
  //unsigned long micro=micros();
  for (int y = 0; y < CUBE_SIZE; y++)
    for (int x = 0; x < CUBE_SIZE; x++){
      /*Tlc.set(LedRed[y][x],ValueLed[layerno][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
      Tlc.set(LedGreen[y][x],ValueLed[layerno][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940
      Tlc.set(LedBlue[y][x],ValueLed[layerno][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940
      */
      Tlc.set(x * 3 + y * CUBE_SIZE * 3, ValueLed[layerno][x * 3 + y * CUBE_SIZE * 3]*16);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
      Tlc.set(x * 3 + 1 + y * CUBE_SIZE * 3, ValueLed[layerno][x * 3 + 1 + y * CUBE_SIZE * 3]*16);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
      Tlc.set(x * 3 + 2 + y * CUBE_SIZE * 3, ValueLed[layerno][x * 3 + 2 + y * CUBE_SIZE * 3]*16);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
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

void snake(char action){
  AllOff();
  //Move along direction
  if(action!='-'){
    switch(action){
      case 'u':
        snakeDir=4;
        break;
      case 'd':
        snakeDir=5;
        break;
      case 'l':
        snakeDir=2;
        break;
      case 'r':
        snakeDir=3;
        break;
    }
  }
  
    switch(snakeDir){
      case 0:
          snakeX=(snakeX+1)%CUBE_SIZE;
          break;
      case 1:
          if(--snakeX<0)
              snakeX=CUBE_SIZE-1;
          break;
      case 2:
          snakeY=(snakeY+1)%CUBE_SIZE;
          break;
      case 3:
           if(--snakeY<0)
              snakeY=CUBE_SIZE-1;
          break;
      case 4:
          snakeL=(snakeL+1)%CUBE_SIZE;
          break;
      case 5:
          if(--snakeL<0)
              snakeL=CUBE_SIZE-1;
          break;
      
      //Update Tail
      for(int i=1; i<snakeList.size();i++){
          Point node=snakeList.get(i);
          Point before=snakeList.get(i-1);
          node.x=before.x; node.y=before.y; node.l=before.l;
      }
      //Update Head
      Point head=snakeList.get(0);
      head.x=snakeX;
      head.y=snakeY;
      head.l=snakeL;
      //Draw Snake 
      for(int i=0; i<snakeList.size();i++){
          Point node=snakeList.get(i);
          SETLED(node.x ,node.y ,node.l,GREEN,maxBright)
      }
  }
}
void testAll(){
  FRAME_TIME=800;
  AllOff();
  maxCount=649;
  ValueLed[FrameCount/(CUBE_SIZE*CUBE_SIZE*3)][FrameCount%(CUBE_SIZE*CUBE_SIZE*3)]=maxBright;
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
       setXPillar(1,1,2,2, RED);
       setYPillar(3,3,1,4, GREEN);
       setZPillar(4,0,3,3,BLUE);
    }
}
void cubeRotate(){
	if(FrameCount==0)
		maxCount=16*4;
  AllOff();
	drawCube(5-(FrameCount/(4*4)),(FrameCount%16)/4,FrameCount%4,3,RED);
}

/* Cubes growing/shrinking */
void cubeGrow(uint8_t frame){
  if(frame==0){
    maxCount=34;
    frame=1;
  }
  //AllOff();
  if(frame<7)                        //RED GROW (5 Frames)
    drawCubeHollowUp(0,0,0,frame,RED);
  else if(frame<12)                  //RED SHRINK to width 1 (6 Frames)
    drawCubeHollowUp(0,(frame+1)%7,(frame+1)%7,CUBE_SIZE-((frame+1)%7),RED);
  else if(frame<18)                  //GREEN GROW
    drawCubeHollowUp(0,CUBE_SIZE-1-(frame+2)%7,CUBE_SIZE-1-(frame+2)%7,(frame+3)%7,GREEN);
  else if(frame<23)                  //GREEN SHRINK to width 1
    drawCubeHollowUp(0,(frame+4)%7,0,CUBE_SIZE-(frame+4)%7,GREEN);
  else if(frame<29)                  //BLUE GROW
    drawCubeHollowUp(0,CUBE_SIZE-1-(frame+5)%7,0,(frame+6)%7,BLUE);
  else if(frame<34)                  //BLUE SHRINK to width 1
    drawCubeHollowUp(0,0,0,CUBE_SIZE-(frame+7)%7,BLUE);
}

void drawCubeHollowUp(uint8_t positionZ,uint8_t posY,uint8_t posX,uint8_t width,uint8_t color){
  if(positionZ+width>CUBE_SIZE || posY+width>CUBE_SIZE || posX+width>CUBE_SIZE || width<1)
    return;
   setXPillar(posY,positionZ,posX,width,color);
   setXPillar(posY+width-1,positionZ,posX,width,color);
   setXPillar(posY,positionZ+width-1,posX,width,color);
   setXPillar(posY+width-1,positionZ+width-1,posX,width,color);

   setYPillar(posX,positionZ,posY,width,color);
   setYPillar(posX+width-1,positionZ,posY,width,color);
   setYPillar(posX,positionZ+width-1,posY,width,color);
   setYPillar(posX+width-1,positionZ+width-1,posY,width,color);

   setZPillar(posX,posY,positionZ,width,color);
   setZPillar(posX+width-1,posY,positionZ,width,color);
   setZPillar(posX,posY+width-1,positionZ,width,color);
   setZPillar(posX+width-1,posY+width-1,positionZ,width,color);
}

/* Random Leds rain from top layer down*/
void rain(){
  for(int l=0;l<CUBE_SIZE-1;l++){
    for(int x=0; x<CUBE_SIZE;x++){
      for(int y=0;y<CUBE_SIZE;y++){
        ValueLed[l][x*3+y*CUBE_SIZE*3+0]=ValueLed[l+1][x*3+y*CUBE_SIZE*3+0];
        ValueLed[l][x*3+y*CUBE_SIZE*3+1]=ValueLed[l+1][x*3+y*CUBE_SIZE*3+1];
        ValueLed[l][x*3+y*CUBE_SIZE*3+2]=ValueLed[l+1][x*3+y*CUBE_SIZE*3+2];
        }
      } 
  }
  clearLayer(CUBE_SIZE-1);
  
  for(int y=0; y<random8(22);y++){
      uint8_t rd = random8(3);
      SETLED(random8(6), random8(6), CUBE_SIZE-1, rd, maxBright)
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
void clearLayer(uint8_t layer){
  for(int y=0;y<CUBE_SIZE;y++)
    for(int x=0;x<CUBE_SIZE;x++){
      SETLED(x,y,layer,RED,0)
      SETLED(x,y,layer,GREEN,0) 
      SETLED(x,y,layer,BLUE,0) 
    }
}
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
inline void setXPillar(uint8_t y,uint8_t z,uint8_t xstart,uint8_t width, uint8_t color){
  if(xstart+width-1>CUBE_SIZE-1)
    return;
  for (int x = xstart; x < xstart+width; x++)
      SETLED(x, y, z, color, maxBright)
}
inline void setYPillar(uint8_t x, uint8_t z,uint8_t ystart,uint8_t width, uint8_t color){
  if(ystart+width-1>CUBE_SIZE-1)
    return;
  for (int y = ystart; y < ystart+width; y++)
    SETLED(x, y, z, color, maxBright)
}
inline void setZPillar(uint8_t x, uint8_t y,uint8_t zstart,uint8_t width, uint8_t color){
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
