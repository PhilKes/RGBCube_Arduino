#define RED 2
#define GREEN 1
#define BLUE 0

//------CONTROL FUNCTIONS
//Position (x,y,l) Color: 'r','g','b'
void setLed(uint8_t x, uint8_t y, uint8_t l,uint8_t color,uint16_t value)
{
	Tlc.set((x * 3) + (y * CUBE_SIZE * 3)+color,l, value);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
}
void CubeUpdate(uint8_t layer)  //Updated Layer layerno(schickt Werte in TLCs, schaltet Layer layerno ein
{
	/*
	  for(int y=0;y<CUBE_SIZE;y++)
	   {
		 for(int x=0;x<CUBE_SIZE;x++)
		 {
		   Tlc.set(LedRed[y][x],ValueLed[layerno][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
		   Tlc.set(LedGreen[y][x],ValueLed[layerno][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940
		   Tlc.set(LedBlue[y][x],ValueLed[layerno][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940
		 }
	   }
	 */
	 //CAUTION ONLY FOR CUBE_SIZE = 6
  int updateTime= micros();
	PORTC = PORTC|B00111111;
	Tlc.update(layer);
	delay(1);
	//Substitute with Port Manipulation
	//digitalWrite(layers[layer], LOW);
	PORTC = PORTC & (B00111111 & ~(1 << layer));
  Serial.print("UPDATE:");
  int mic=micros()-updateTime;
  Serial.print(mic);
  Serial.println();
	//delayMicroseconds(200);
}
void setPane(int pane, int val) 
{
	for (int l = 0; l < CUBE_SIZE; l++)
		for (int y = 0; y < CUBE_SIZE; y++)
			setLed(pane, y, l, RED, val);
			//ValueLed[l][+ y * CUBE_SIZE * 3] = val; //RED ONLY
		   // ValueLed[l][x*3+1+y*CUBE_SIZE*3]=val;
		   //ValueLed[l][x*3+2+y*CUBE_SIZE*3]=val;

}
void testLed(int pin, int layer)
{
	setLed(pin % (CUBE_SIZE * 3), pin / (CUBE_SIZE * 3), layer, BLUE, MAXBRIGHT);
}
void AllRed()
{
	for (int l = 0; l < 4; l++)
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				setLed(y, x, l, RED, brightR);
}
void AllGreen()
{
	for (int l = 0; l < 4; l++)
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				setLed(y, x, l, GREEN, brightG);
}
void AllBlue()
{
	for (int l = 0; l < 4; l++)
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				setLed(y, x, l, BLUE, brightB);
}
void AllOff()
{
	Tlc.clearAll();
}
void mappingfull()
{
	/*
		for(int i=0;i<FrameCount;i++)
		   ValueLed[i/48][i%48]=maxbright;
	   if(FrameCount/48==4)
			FrameCount=0;
	   */
}

//------ANIMATIONS
void RGBColorRoom()
{	
		for(int l=0;l<4;l++)
			for(int y=0;y<4;y++)
				for(int x=0;x<4;x++)
				{
					setLed(x,y,l,RED,100+x*1300);
					setLed(x,y,l,GREEN,100+l*800);
					setLed(x,y,l,BLUE,50+y*500);
				}
		FrameCount=0;
}
void ColorCycle()
{
	if(FrameCount==0)
	{
		MAXCOUNT=31;
		brightR=0;
		brightG=0;
		brightB=MAXBRIGHT;
	}
	else if(FrameCount<11)
	{
		brightR+=400;
		if(brightB>=400){brightB-=400;}
	}
	else if(FrameCount<21)
	{
		brightG+=400;
		brightR-=400;
	}
	else if(FrameCount<31)
	{
		brightB+=400;
		brightG-=400;
	}
	//else if(FrameCount==31){FrameCount=0;}
	AllRed();
	AllGreen();
	AllBlue();
}
void Snake()
{
  if(FrameCount==0)MAXCOUNT=383;
  if(FrameCount>=0)
  {
      int temp=FrameCount;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/(CUBE_SIZE*CUBE_SIZE);i++)
      {
        for(int j=0;j<=temp%(CUBE_SIZE*CUBE_SIZE);j++)
            setLed(j%CUBE_SIZE,j/CUBE_SIZE,i,RED,MAXBRIGHT);
        if(i>0)
          for(int k=0;k<i;k++)
            for(int x=0;x<CUBE_SIZE;x++)
              for(int y=0;y<CUBE_SIZE;y++)
                setLed(x,y,k,RED,MAXBRIGHT);
      }
  }
  if(FrameCount>63)
  {
     int temp=FrameCount-63;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
            setLed(j/4,j%4,i,GREEN,MAXBRIGHT);
        if(i>0)
          for(int k=0;k<i;k++)
            for(int x=0;x<4;x++)
              for(int y=0;y<4;y++)
                setLed(x,y,k,GREEN,MAXBRIGHT);
      }
  }
  if(FrameCount>127)
  {
    int temp=FrameCount-127;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
            setLed(j/4,j%4,i,BLUE,MAXBRIGHT);
        if(i>0)
          for(int k=0;k<i;k++)
            for(int x=0;x<4;x++)
              for(int y=0;y<4;y++)
                setLed(x,y,k,BLUE,MAXBRIGHT);
      }
  }
  if(FrameCount>191)
  {
      int temp=FrameCount-191;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
            setLed(j/4,j%4,i,RED,0);
        if(i>0)
          for(int k=0;k<i;k++)
            for(int x=0;x<4;x++)
              for(int y=0;y<4;y++)
                setLed(x,y,k,RED,0);
      }
  }
  if(FrameCount>255)
  {
     int temp=FrameCount-255;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
            setLed(j/4,j%4,i,GREEN,0);
        if(i>0)
          for(int k=0;k<i;k++)
            for(int x=0;x<4;x++)
              for(int y=0;y<4;y++)
                setLed(x,y,k,GREEN,0);
      }
  }
  if(FrameCount>319)
  {
    int temp=FrameCount-319;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
            setLed(j/4,j%4,i,BLUE,0);
        if(i>0)
          for(int k=0;k<i;k++)
            for(int x=0;x<4;x++)
              for(int y=0;y<4;y++)
                setLed(x,y,k,BLUE,0);
      }
  }
  //if(FrameCount==383)
  //  FrameCount=0;
}

void wallSenk(int brR,int brG,int brB,boolean last)
{
  switch(FrameCount)
  {        
    case 0:
            for(int y=0;y<4;y++)
              for(int x=0;x<4;x++)
              {
                setLed(x,y,3,RED,brR);
				setLed(x,y,3,GREEN,brG);
				setLed(x,y,3,BLUE,brB);
              }
         break;
    case 1:
            for(int y=0;y<4;y++)
              for(int x=0;x<4;x++)
              {
                setLed(x,y,2,RED,brR);
				setLed(x,y,2,GREEN,brG);
				setLed(x,y,2,BLUE,brB);
				setLed(x,y,3,RED,brR/4);
				setLed(x,y,3,GREEN,brG/4);
				setLed(x,y,3,BLUE,brB/4);
              }
         break;
    case 2:  
            for(int y=0;y<4;y++)
              for(int x=0;x<4;x++)
              {
                setLed(x,y,1,RED,brR);
				setLed(x,y,1,GREEN,brG);
				setLed(x,y,1,BLUE,brB);
				setLed(x,y,2,RED,brR/4);
				setLed(x,y,2,GREEN,brG/4);
				setLed(x,y,2,BLUE,brB/4);
				setLed(x,y,3,RED,brR/8);
				setLed(x,y,3,GREEN,brG/8);
				setLed(x,y,3,BLUE,brB/8);
              }
              break;
    case 3:
           for(int y=0;y<4;y++)
              for(int x=0;x<4;x++)
              {
                setLed(x,y,0,RED,brR);
				setLed(x,y,0,GREEN,brG);
				setLed(x,y,0,BLUE,brB);
				setLed(x,y,1,RED,brR/4);
				setLed(x,y,1,GREEN,brG/4);
				setLed(x,y,1,BLUE,brB/4);
				setLed(x,y,2,RED,brR/8);
				setLed(x,y,2,GREEN,brG/8);
				setLed(x,y,2,BLUE,brB/8);
				setLed(x,y,3,RED,brR/16);
				setLed(x,y,3,GREEN,brG/16);
				setLed(x,y,3,BLUE,brB/16);
              }
           break;
    case 4:
            for(int y=0;y<4;y++)
              for(int x=0;x<4;x++)
              {
                setLed(x,y,1,RED,brR);
				setLed(x,y,1,GREEN,brG);
				setLed(x,y,1,BLUE,brB);
				setLed(x,y,0,RED,brR/4);
				setLed(x,y,0,GREEN,brG/4);
				setLed(x,y,0,BLUE,brB/4);
              }
            break;
    case 5:
            for(int y=0;y<4;y++)
              for(int x=0;x<4;x++)
              {
                setLed(x,y,2,RED,brR);
				setLed(x,y,2,GREEN,brG);
				setLed(x,y,2,BLUE,brB);
				setLed(x,y,1,RED,brR/4);
				setLed(x,y,1,GREEN,brG/4);
				setLed(x,y,1,BLUE,brB/4);
				setLed(x,y,0,RED,brR/8);
				setLed(x,y,0,GREEN,brG/8);
				setLed(x,y,0,BLUE,brB/8);
              }
			 if(last)
				FrameCount=0;
              break; 
    default:
                break;                
  }
}
void wallWaag(int brR,int brG,int brB,int side,boolean last)
{
  if(side==0)
  {
    //AllOff();
    switch(FrameCount)
    {        
      case 0:
          for(int l=0;l<4;l++)
              for(int y=0;y<4;y++)
              {
                  setLed(y,0,l,RED,brR);
  				setLed(y,0,l,GREEN,brG);
  				setLed(y,0,l,BLUE,brB);
              }
           break;
      case 1:  
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
                  setLed(y,1,l,RED,brR);
  				  setLed(y,1,l,GREEN,brG);
  				  setLed(y,1,l,BLUE,brB);
  				  setLed(y,0,l,RED,brR/4);
  				  setLed(y,0,l,GREEN,brG/4);
  				  setLed(y,0,l,BLUE,brB/4);
                }
                break;
      case 2:
             for(int l=0;l<4;l++)
               for(int y=0;y<4;y++)
               {
				setLed(y,2,l,RED,brR);
  				setLed(y,2,l,GREEN,brG);
  				setLed(y,2,l,BLUE,brB);
  				setLed(y,1,l,RED,brR/4);
  				setLed(y,1,l,GREEN,brG/4);
  				setLed(y,1,l,BLUE,brB/4);
  				setLed(y,0,l,RED,brR/8);
  				setLed(y,0,l,GREEN,brG/8);
  				setLed(y,0,l,BLUE,brB/8);
               }
             break;
      case 3:
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
				setLed(y,3,l,RED,brR);
  				  setLed(y,3,l,GREEN,brG);
  				  setLed(y,3,l,BLUE,brB);
  				  setLed(y,2,l,RED,brR/4);
  			   setLed(y,2,l,GREEN,brG/4);
  			   setLed(y,2,l,BLUE,brB/4);
  			   setLed(y,1,l,RED,brR/8);
  				setLed(y,1,l,GREEN,brG/8);
  				 setLed(y,1,l,BLUE,brB/8);
  				setLed(y,0,l,RED,brR/16);
  				setLed(y,0,l,GREEN,brG/16);
  				setLed(y,0,l,BLUE,brB/16);
                }
              break;
      case 4:
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
                    setLed(y,2,l,RED,brR);
  				  setLed(y,2,l,GREEN,brG);
  				  setLed(y,2,l,BLUE,brB);
  				  setLed(y,3,l,RED,brR/4);
  				  setLed(y,3,l,GREEN,brG/4);
  				  setLed(y,3,l,BLUE,brB/4);
                }
              break;
      case 5:
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
                    setLed(y,1,l,RED,brR);
  				  setLed(y,1,l,GREEN,brG);
  				  setLed(y,1,l,BLUE,brB);
  				  setLed(y,2,l,RED,brR/4);
  				  setLed(y,2,l,GREEN,brG/4);
  				  setLed(y,2,l,BLUE,brB/4);
  				  setLed(y,3,l,RED,brR/8);
  				  setLed(y,3,l,GREEN,brG/8);
  				  setLed(y,3,l,BLUE,brB/8);
                }
               if(last==true)
                  FrameCount=0;
                break;
      default:
                break;                
    }
  }
  else if(side==1)
  {
    //AllOff();
    switch(FrameCount)
    {        
      case 0:
          for(int l=0;l<4;l++)
              for(int y=0;y<4;y++)
              {
                  setLed(y,3,l,RED,brR);
  				  setLed(y,3,l,GREEN,brG);
  				  setLed(y,3,l,BLUE,brB);
              }
           break;
      case 1:  
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
                    setLed(y,2,l,RED,brR);
  					setLed(y,2,l,GREEN,brG);
  					setLed(y,2,l,BLUE,brB);
  					setLed(y,3,l,RED,brR/4);
  					setLed(y,3,l,GREEN,brG/4);
  					setLed(y,3,l,BLUE,brB/4);
                }
                break;
      case 2:
             for(int l=0;l<4;l++)
               for(int y=0;y<4;y++)
               {
                setLed(y,2,l,RED,brR/4);
  				setLed(y,2,l,GREEN,brG/4);
  				setLed(y,2,l,BLUE,brB/4);
  				setLed(y,1,l,RED,brR);
  				setLed(y,1,l,GREEN,brG);
  				setLed(y,1,l,BLUE,brB);
  				setLed(y,3,l,RED,brR/8);
  				setLed(y,3,l,GREEN,brG/8);
  				setLed(y,3,l,BLUE,brB/8);
               }
             break;
      case 3:
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
                  setLed(y,0,l,RED,brR);
  				  setLed(y,0,l,GREEN,brG);
  				  setLed(y,0,l,BLUE,brB);
  				  setLed(y,1,l,RED,brR/4);
  				  setLed(y,1,l,GREEN,brG/4);
  				  setLed(y,1,l,BLUE,brB/4);	
                }
              break;
      case 4:
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
                  setLed(y,1,l,RED,brR);
  				  setLed(y,1,l,GREEN,brG);
  				  setLed(y,1,l,BLUE,brB);
  				  setLed(y,0,l,RED,brR/4);
  				  setLed(y,0,l,GREEN,brG/4);
  				  setLed(y,0,l,BLUE,brB/4);
                }
              break;
      case 5:
              for(int l=0;l<4;l++)
                for(int y=0;y<4;y++)
                {
                  setLed(y,2,l,RED,brR);
  				  setLed(y,2,l,GREEN,brG);
  				  setLed(y,2,l,BLUE,brB);
  				  setLed(y,1,l,RED,brR/4);
  				  setLed(y,1,l,GREEN,brG/4);
  				  setLed(y,1,l,BLUE,brB/4);
  				  setLed(y,0,l,RED,brR/8);
  				  setLed(y,0,l,GREEN,brG/8);
  				  setLed(y,0,l,BLUE,brB/8);
                }
                FrameCount=0;
                break;
      default:
                  break;                
    }
  }
}

void randomLedsFull()
{
  for(int j=0;j<CUBE_SIZE;j++)
  {
    for(int x=0;x<CUBE_SIZE;x++)
			for (int y = 0; y < CUBE_SIZE; y++)
			{
				int rand = random(0, 3);
				if (rand == 0)
					setLed(x, y, j, RED, MAXBRIGHT);
				else if (rand == 1)
					setLed(x, y, j, GREEN, MAXBRIGHT);
				else if (rand == 2)
					setLed(x, y, j, BLUE, MAXBRIGHT);
			}
      FrameCount=0;
    }   
}
void randomLeds(int anzR,int anzG,int anzB)
{
      for (int p=anzR;p>0;p--)
        setLed(random(0,CUBE_SIZE),random(0,CUBE_SIZE),random(0,CUBE_SIZE),RED,MAXBRIGHT);
      for (int p=anzG;p>0;p--)
        setLed(random(0,CUBE_SIZE),random(0,CUBE_SIZE),random(0,CUBE_SIZE),GREEN,MAXBRIGHT);
      for (int p=anzB;p>0;p--)
        setLed(random(0,CUBE_SIZE),random(0,CUBE_SIZE),random(0,CUBE_SIZE),BLUE,MAXBRIGHT);
      FrameCount=0;
}

void littleCube()
{
  //AllOff();
  switch(FrameCount)
  {
    case 0:
	posX=1;
	posY=1;
	posL=2;
         break;
    case 1:  
		posL--;
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
		posL++;
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
		posL++;
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
  zeichnelittleCube(posL,posY,posX);
}
void Cubes4()
{
	if(FrameCount==0)
	{	
		brightR=MAXBRIGHT;
		brightG=0;
		brightB=0;
		zeichnelittleCube(1,0,2);		//Rot
		brightR=0;
		brightG=MAXBRIGHT;
		brightB=0;
		zeichnelittleCube(3,2,2);		//Grün
		brightR=0;
		brightG=0;
		brightB=MAXBRIGHT;
		zeichnelittleCube(1,2,0);		//Blau
		brightR=MAXBRIGHT/2;
		brightG=0;
		brightB=MAXBRIGHT/2;
		zeichnelittleCube(1,0,0);		//Rot+Blau
		brightR=MAXBRIGHT/2;
		brightG=MAXBRIGHT/2;
		brightB=0;
		zeichnelittleCube(3,0,2);		//Rot+Grün
		brightR=0;
		brightG=MAXBRIGHT/2;
		brightB=MAXBRIGHT/2;
		zeichnelittleCube(3,2,0);		//Blau+Grün
                brightR=MAXBRIGHT;
		brightG=MAXBRIGHT/2;
		brightB=MAXBRIGHT/4;
		zeichnelittleCube(3,0,0);		//Rot+Grün+(blau+rot)
		brightR=MAXBRIGHT/2;
		brightG=MAXBRIGHT;
		brightB=MAXBRIGHT/4;
		zeichnelittleCube(1,2,2);		//Grün+Blau+(grün+rot)
	}
      else if(FrameCount==1)
	{	
		brightR=MAXBRIGHT;
		brightG=0;
		brightB=0;
		zeichnelittleCube(3,2,2);		//Rot
		brightR=0;
		brightG=MAXBRIGHT;
		brightB=0;
		zeichnelittleCube(1,0,2);		//Grün
		brightR=0;
		brightG=0;
		brightB=MAXBRIGHT;
		zeichnelittleCube(1,0,0);		//Blau
		brightR=MAXBRIGHT/2;
		brightG=0;
		brightB=MAXBRIGHT/2;
		zeichnelittleCube(1,2,0);		//Rot+Blau
		brightR=MAXBRIGHT/2;
		brightG=MAXBRIGHT/2;
		brightB=0;
		zeichnelittleCube(3,2,0);		//Rot+Grün
		brightR=0;
		brightG=MAXBRIGHT/2;
		brightB=MAXBRIGHT/2;
		zeichnelittleCube(3,0,2);		//Blau+Grün
                brightR=MAXBRIGHT;
		brightG=MAXBRIGHT/2;
		brightB=MAXBRIGHT/4;
		zeichnelittleCube(1,2,2);		//Rot+Grün+(blau+rot)
		brightR=MAXBRIGHT/2;
		brightG=MAXBRIGHT;
		brightB=MAXBRIGHT/4;
		zeichnelittleCube(3,0,0);		//Grün+Blau+(grün+rot)
	}
else if(FrameCount==2)
	{	
		brightR=MAXBRIGHT;
		brightG=0;
		brightB=0;
		zeichnelittleCube(1,2,0);		//Rot
		brightR=0;
		brightG=MAXBRIGHT;
		brightB=0;
		zeichnelittleCube(1,0,0);		//Grün
		brightR=0;
		brightG=0;
		brightB=MAXBRIGHT;
		zeichnelittleCube(1,0,2);		//Blau
		brightR=MAXBRIGHT/2;
		brightG=0;
		brightB=MAXBRIGHT/2;
		zeichnelittleCube(3,2,2);		//Rot+Blau
		brightR=MAXBRIGHT/2;
		brightG=MAXBRIGHT/2;
		brightB=0;
		zeichnelittleCube(3,0,0);		//Rot+Grün
		brightR=0;
		brightG=MAXBRIGHT/2;
		brightB=MAXBRIGHT/2;
		zeichnelittleCube(1,2,0);		//Blau+Grün
                brightR=MAXBRIGHT;
		brightG=MAXBRIGHT/2;
		brightB=MAXBRIGHT/4;
		zeichnelittleCube(3,0,2);		//Rot+Grün+(blau+rot)
		brightR=MAXBRIGHT/2;
		brightG=MAXBRIGHT;
		brightB=MAXBRIGHT/4;
		zeichnelittleCube(3,2,0);		//Grün+Blau+(grün+rot)
		FrameCount=0;
	}
}
void zeichnelittleCube(int posl,int posy,int posx)
{
  for(int l=posl;l>posl-2;l--)
   for(int y=posy;y<posy+2;y++)
    for(int x=posx;x<posx+2;x++)
	{
      setLed(x,y,l,RED,brightR);
      setLed(x,y,l,GREEN,brightG);
      setLed(x,y,l,BLUE,brightB);
    }
}  
void aniTest()
{
  //Serial.println("---------FRAME----------");
  //micro=micros();
    for(int l=0;l<CUBE_SIZE;l++)
      for(int y=0;y<CUBE_SIZE;y++)
        for(int x=0;x<CUBE_SIZE;x++)
        {
          //animation                        : 1st LED Red 1st Frame
          //animation+1                      : 1st LED Green 1st Frame
          //animation+2                      : 1st LED Blue 1st Frame
          //animation+FrameCount*(NUM_TLC*16): 1st LED Red 2nd Frame
          //animation+(x*3+y*CUBE_SIZE*3)    : (x,y) LED Red 
          setLed(x,y,l,RED, pgm_read_word_near(animation+FrameCount*(NUM_TLCS *16*CUBE_SIZE)+l*NUM_TLCS*16+(x*3+y*CUBE_SIZE*3)));
          setLed(x,y,l,GREEN, pgm_read_word_near(animation+1+FrameCount*(NUM_TLCS *16*CUBE_SIZE)+l*NUM_TLCS*16+(x*3+y*CUBE_SIZE*3)));
          setLed(x,y,l,BLUE, pgm_read_word_near(animation+2+FrameCount*(NUM_TLCS *16*CUBE_SIZE)+l*NUM_TLCS*16+(x*3+y*CUBE_SIZE*3)));
        }
    //Serial.println(micros()-micro);
}
