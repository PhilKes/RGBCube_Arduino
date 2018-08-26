/* FUNCTIONS;


    
    
    MINE:
    mapping();            //jede Led einzeln, 500ms Delay
    mappingSerial();      //mapping mit Serial Monitor(int i beachten!)
    testLed(int pin);     //testet Led auf Pin pin
    CubeUpdate(int layer);//aktualisiert EbeneNr layer(schickt werte an TLCs, schaltet Ebene layer ein
    
    AllRed();            //Schaltet alle Roten/Blauen/Grünen Leds auf allen Ebenen ein
    AllBlue();
    AllGreen();
    ColorCycle();
    Snake()
    randomLeds(int anzR,int anzG,int anzB);//Schaltet jeden 2.Frame zufällig Rote/Grüne/Blaue Leds an (anzR,...)
    wallWaag(int brightR,int brightG,int brightB); // Waagrechte Wand (hin und her)
    wallWaagSenk(int brightR,int brightG,int brightB);//"       "       "      , Senkrechte Wand(hoch, runter)
    littleCube(int brightR,int brightG,int brightB)    //2x2 Cube Schlangenbahn von Mitte aus
    RGBColorRoom();//Zeigt komplettes RGB - Farbspektrum
    AllOff();            //Schaltet alle Leds auf allen Ebenen aus
*/

/*void mappingSerial()
{  
  Tlc.set(i,3000);
  Tlc.update();
  do{}
  while(Serial.read()!=110);
  Tlc.set(i,0);
  i++; 
}
*/

void RGBColorRoom()
{

		
		for(int l=0;l<4;l++)
		{
			for(int y=0;y<4;y++)
			{
				for(int x=0;x<4;x++)
				{
					ValueLed[l][LedRed[y][x]]=100+x*1300;
					ValueLed[l][LedGreen[y][x]]=100+l*800;
					ValueLed[l][LedBlue[y][x]]=50+y*500;
				}
			}
		}
		FrameCount=0;
}

void ColorCycle()
{
	if(FrameCount==0)
	{
		maxCount=31;
		brightR=0;
		brightG=0;
		brightB=maxbright;
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
void mappingfull()
{
      
      
    for(int i=0;i<FrameCount;i++)
     {
       ValueLed[i/48][i%48]=maxbright;
     }
   if(FrameCount/48==4)
        FrameCount=0;
   
}
void Snake()
{
  if(FrameCount==0)maxCount=383;
  if(FrameCount>=0)
  {
      int temp=FrameCount;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
        {
            ValueLed[i][LedRed[j/CUBE_SIZE][j%CUBE_SIZE]]=maxbright;
        }
        if(i>0)
        {
          for(int k=0;k<i;k++)
          {
            for(int x=0;x<CUBE_SIZE;x++)
            {
              for(int y=0;y<CUBE_SIZE;y++)
              {
                ValueLed[k][LedRed[y][x]]=maxbright;
              }
            }
          }
        }
      }
  }
  if(FrameCount>63)
  {
     int temp=FrameCount-63;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
        {
            ValueLed[i][LedGreen[j/4][j%4]]=maxbright;
        }
        if(i>0)
        {
          for(int k=0;k<i;k++)
          {
            for(int x=0;x<4;x++)
            {
              for(int y=0;y<4;y++)
              {
                ValueLed[k][LedGreen[y][x]]=maxbright;
              }
            }
          }
        }
      }
  }
  if(FrameCount>127)
  {
    int temp=FrameCount-127;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
        {
            ValueLed[i][LedBlue[j/4][j%4]]=maxbright;
        }
        if(i>0)
        {
          for(int k=0;k<i;k++)
          {
            for(int x=0;x<4;x++)
            {
              for(int y=0;y<4;y++)
              {
                ValueLed[k][LedBlue[y][x]]=maxbright;
              }
            }
          }
        }
      }
  }
  if(FrameCount>191)
  {
      int temp=FrameCount-191;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
        {
            ValueLed[i][LedRed[j/4][j%4]]=0;
        }
        if(i>0)
        {
          for(int k=0;k<i;k++)
          {
            for(int x=0;x<4;x++)
            {
              for(int y=0;y<4;y++)
              {
                ValueLed[k][LedRed[y][x]]=0;
              }
            }
          }
        }
      }
  }
  if(FrameCount>255)
  {
     int temp=FrameCount-255;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
        {
            ValueLed[i][LedGreen[j/4][j%4]]=0;
        }
        if(i>0)
        {
          for(int k=0;k<i;k++)
          {
            for(int x=0;x<4;x++)
            {
              for(int y=0;y<4;y++)
              {
                ValueLed[k][LedGreen[y][x]]=0;
              }
            }
          }
        }
      }
  }
  if(FrameCount>319)
  {
    int temp=FrameCount-319;
      if(temp>63)temp=63;
      for(int i=0;i<=temp/16;i++)
      {
        for(int j=0;j<=temp%16;j++)
        {
            ValueLed[i][LedBlue[j/4][j%4]]=0;
        }
        if(i>0)
        {
          for(int k=0;k<i;k++)
          {
            for(int x=0;x<4;x++)
            {
              for(int y=0;y<4;y++)
              {
                ValueLed[k][LedBlue[y][x]]=0;
              }
            }
          }
        }
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
            {
              for(int x=0;x<4;x++)
              {
                ValueLed[3][LedRed[y][x]]=brR;
		ValueLed[3][LedGreen[y][x]]=brG;
		ValueLed[3][LedBlue[y][x]]=brB;
              }
            }
         break;
    case 1:
            for(int y=0;y<4;y++)
            {
              for(int x=0;x<4;x++)
              {
                ValueLed[2][LedRed[y][x]]=brR;
				ValueLed[2][LedGreen[y][x]]=brG;
				ValueLed[2][LedBlue[y][x]]=brB;
				
				ValueLed[3][LedRed[y][x]]=brR/4;
				ValueLed[3][LedGreen[y][x]]=brG/4;
				ValueLed[3][LedBlue[y][x]]=brB/4;
              }
            }
         break;
    case 2:  
            for(int y=0;y<4;y++)
            {
              for(int x=0;x<4;x++)
              {
                ValueLed[1][LedRed[y][x]]=brR;
			ValueLed[1][LedGreen[y][x]]=brG;
			ValueLed[1][LedBlue[y][x]]=brB;
			
			ValueLed[2][LedRed[y][x]]=brR/4;
			ValueLed[2][LedGreen[y][x]]=brG/4;
			ValueLed[2][LedBlue[y][x]]=brB/4;
			
			ValueLed[3][LedRed[y][x]]=brR/8;
				ValueLed[3][LedGreen[y][x]]=brG/8;
				ValueLed[3][LedBlue[y][x]]=brB/8;
              }
            }
              break;
    case 3:
           for(int y=0;y<4;y++)
            {
              for(int x=0;x<4;x++)
              {
                ValueLed[0][LedRed[y][x]]=brR;
		ValueLed[0][LedGreen[y][x]]=brG;
		ValueLed[0][LedBlue[y][x]]=brB;
		
		     ValueLed[1][LedRed[y][x]]=brR/4;
			ValueLed[1][LedGreen[y][x]]=brG/4;
			ValueLed[1][LedBlue[y][x]]=brB/4;
			
			ValueLed[2][LedRed[y][x]]=brR/8;
			ValueLed[2][LedGreen[y][x]]=brG/8;
			ValueLed[2][LedBlue[y][x]]=brB/8;
			
			ValueLed[3][LedRed[y][x]]=brR/16;
				ValueLed[3][LedGreen[y][x]]=brG/16;
				ValueLed[3][LedBlue[y][x]]=brB/16;
              }
            }
           break;
    case 4:
            for(int y=0;y<4;y++)
            {
              for(int x=0;x<4;x++)
              {
                ValueLed[1][LedRed[y][x]]=brR;
		ValueLed[1][LedGreen[y][x]]=brG;
		ValueLed[1][LedBlue[y][x]]=brB;
		
		 ValueLed[0][LedRed[y][x]]=brR/4;
		ValueLed[0][LedGreen[y][x]]=brG/4;
		ValueLed[0][LedBlue[y][x]]=brB/4;
              }
            }
            break;
    case 5:
            for(int y=0;y<4;y++)
            {
              for(int x=0;x<4;x++)
              {
                ValueLed[2][LedRed[y][x]]=brR;
		ValueLed[2][LedGreen[y][x]]=brG;
		ValueLed[2][LedBlue[y][x]]=brB;
		
		 ValueLed[1][LedRed[y][x]]=brR/4;
		ValueLed[1][LedGreen[y][x]]=brG/4;
		ValueLed[1][LedBlue[y][x]]=brB/4;
		
		 ValueLed[0][LedRed[y][x]]=brR/8;
		ValueLed[0][LedGreen[y][x]]=brG/8;
		ValueLed[0][LedBlue[y][x]]=brB/8;
              }
            }
			 if(last)
			 {
                           FrameCount=0;
			 }
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
          {
              for(int y=0;y<4;y++)
              {
                  ValueLed[l][LedRed[y][0]]=brR;
  				ValueLed[l][LedGreen[y][0]]=brG;
  				ValueLed[l][LedBlue[y][0]]=brB;
              }
           }
           break;
      case 1:  
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                    ValueLed[l][LedRed[y][1]]=brR;
  				  ValueLed[l][LedGreen[y][1]]=brG;
  				  ValueLed[l][LedBlue[y][1]]=brB;
  				  
  				  ValueLed[l][LedRed[y][0]]=brR/4;
  				  ValueLed[l][LedGreen[y][0]]=brG/4;
  				  ValueLed[l][LedBlue[y][0]]=brB/4;
                }
               }
                break;
      case 2:
             for(int l=0;l<4;l++)
             {
               for(int y=0;y<4;y++)
               {
                 ValueLed[l][LedRed[y][2]]=brR;
  			   ValueLed[l][LedGreen[y][2]]=brG;
  			   ValueLed[l][LedBlue[y][2]]=brB;
  			   
  			   ValueLed[l][LedRed[y][1]]=brR/4;
  				ValueLed[l][LedGreen[y][1]]=brG/4;
  				  ValueLed[l][LedBlue[y][1]]=brB/4;
  				  
  				ValueLed[l][LedRed[y][0]]=brR/8;
  				ValueLed[l][LedGreen[y][0]]=brG/8;
  				ValueLed[l][LedBlue[y][0]]=brB/8;
  				  
               }
             }
             break;
      case 3:
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                    ValueLed[l][LedRed[y][3]]=brR;
  				  ValueLed[l][LedGreen[y][3]]=brG;
  				  ValueLed[l][LedBlue[y][3]]=brB;
  				  
  				  ValueLed[l][LedRed[y][2]]=brR/4;
  			   ValueLed[l][LedGreen[y][2]]=brG/4;
  			   ValueLed[l][LedBlue[y][2]]=brB/4;
  			   
  			   ValueLed[l][LedRed[y][1]]=brR/8;
  				ValueLed[l][LedGreen[y][1]]=brG/8;
  				 ValueLed[l][LedBlue[y][1]]=brB/8;
  				  
  				ValueLed[l][LedRed[y][0]]=brR/16;
  				ValueLed[l][LedGreen[y][0]]=brG/16;
  				ValueLed[l][LedBlue[y][0]]=brB/16;
  				
                }
               }
              break;
      case 4:
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                    ValueLed[l][LedRed[y][2]]=brR;
  				  ValueLed[l][LedGreen[y][2]]=brG;
  				  ValueLed[l][LedBlue[y][2]]=brB;
  				  
  				  ValueLed[l][LedRed[y][3]]=brR/4;
  				  ValueLed[l][LedGreen[y][3]]=brG/4;
  				  ValueLed[l][LedBlue[y][3]]=brB/4;
                }
               }
              break;
      case 5:
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                    ValueLed[l][LedRed[y][1]]=brR;
  				  ValueLed[l][LedGreen[y][1]]=brG;
  				  ValueLed[l][LedBlue[y][1]]=brB;
  				  
  				  ValueLed[l][LedRed[y][2]]=brR/4;
  				  ValueLed[l][LedGreen[y][2]]=brG/4;
  				  ValueLed[l][LedBlue[y][2]]=brB/4;
  				  
  				  ValueLed[l][LedRed[y][3]]=brR/8;
  				  ValueLed[l][LedGreen[y][3]]=brG/8;
  				  ValueLed[l][LedBlue[y][3]]=brB/8;
                }
               }
               if(last==true)
               {
                  FrameCount=0;
                }
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
          {
              for(int y=0;y<4;y++)
              {
                  ValueLed[l][LedRed[y][3]]=brR;
  				  ValueLed[l][LedGreen[y][3]]=brG;
  				  ValueLed[l][LedBlue[y][3]]=brB;
              }
           }
           break;
      case 1:  
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                    ValueLed[l][LedRed[y][2]]=brR;
  			   ValueLed[l][LedGreen[y][2]]=brG;
  			   ValueLed[l][LedBlue[y][2]]=brB;
  				  
  				  ValueLed[l][LedRed[y][3]]=brR/4;
  				  ValueLed[l][LedGreen[y][3]]=brG/4;
  				  ValueLed[l][LedBlue[y][3]]=brB/4;
                }
               }
                break;
      case 2:
             for(int l=0;l<4;l++)
             {
               for(int y=0;y<4;y++)
               {
                 ValueLed[l][LedRed[y][2]]=brR/4;
  			   ValueLed[l][LedGreen[y][2]]=brG/4;
  			   ValueLed[l][LedBlue[y][2]]=brB/4;
  			   
  			   ValueLed[l][LedRed[y][1]]=brR;
  				ValueLed[l][LedGreen[y][1]]=brG;
  				  ValueLed[l][LedBlue[y][1]]=brB;
  				  
  				ValueLed[l][LedRed[y][3]]=brR/8;
  				ValueLed[l][LedGreen[y][3]]=brG/8;
  				ValueLed[l][LedBlue[y][3]]=brB/8;
  				  
               }
             }
             break;
      case 3:
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                    ValueLed[l][LedRed[y][0]]=brR;
  				  ValueLed[l][LedGreen[y][0]]=brG;
  				  ValueLed[l][LedBlue[y][0]]=brB;
  				  
  				  ValueLed[l][LedRed[y][1]]=brR/4;
  			   ValueLed[l][LedGreen[y][1]]=brG/4;
  			   ValueLed[l][LedBlue[y][1]]=brB/4;	
                }
               }
              break;
      case 4:
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                    ValueLed[l][LedRed[y][1]]=brR;
  				  ValueLed[l][LedGreen[y][1]]=brG;
  				  ValueLed[l][LedBlue[y][1]]=brB;
  				  
  				  ValueLed[l][LedRed[y][0]]=brR/4;
  				  ValueLed[l][LedGreen[y][0]]=brG/4;
  				  ValueLed[l][LedBlue[y][0]]=brB/4;
                }
               }
              break;
      case 5:
              for(int l=0;l<4;l++)
              {
                for(int y=0;y<4;y++)
                {
                                  ValueLed[l][LedRed[y][2]]=brR;
  				  ValueLed[l][LedGreen[y][2]]=brG;
  				  ValueLed[l][LedBlue[y][2]]=brB;
  				  
  				  ValueLed[l][LedRed[y][1]]=brR/4;
  				  ValueLed[l][LedGreen[y][1]]=brG/4;
  				  ValueLed[l][LedBlue[y][1]]=brB/4;
  				  
  				  ValueLed[l][LedRed[y][0]]=brR/8;
  				  ValueLed[l][LedGreen[y][0]]=brG/8;
  				  ValueLed[l][LedBlue[y][0]]=brB/8;
                }
               }
                FrameCount=0;
                break;
     
      default:
                  break;                
    }
  }
}

void testLed(int pin,int layer)
{
 /* Tlc.set(pin,maxbright);
  Tlc.update();*/
  ValueLed[layer][pin]=4000;
 
}
void AllRed()
{

    for(int l=0;l<4;l++)
    {
      for(int y=0;y<4;y++)
      {
        for(int x=0;x<4;x++)
        {
  	       ValueLed[l][LedRed[y][x]]=brightR;
        }
      }
    }

}


 
void AllGreen()
{

    for(int l=0;l<4;l++)
    {
      for(int y=0;y<4;y++)
      {
        for(int x=0;x<4;x++)
        {
  	ValueLed[l][LedGreen[y][x]]=brightG;
        }
      }
    }
}
void AllBlue()
{
    for(int l=0;l<4;l++)
    {
      for(int y=0;y<4;y++)
      {
        for(int x=0;x<4;x++)
        {
  	ValueLed[l][LedBlue[y][x]]=brightB;
        }
      }
    }
}

void randomLedsFull()
{
    
 
	  
      for(int j=0;j<4;j++)
      {
    		for(int x=0;x<4;x++)
    		{
          for(int y=0;y<4;y++)
  		    {
  			  int rand=random(0,3);
          if(rand==0)
            ValueLed[j][LedRed[y][x]]=maxbright;
          else if(rand==1)
            ValueLed[j][LedGreen[y][x]]=maxbright;
          else if(rand==2)
            ValueLed[j][LedBlue[y][x]]=maxbright;
  		    }
        }
      
      
      FrameCount=0;
    }   
}
void randomLeds(int anzR,int anzG,int anzB)
{
    
      for (int p=anzR;p>0;p--)
      {
        ValueLed[random(0,4)][LedRed[random(0,4)][random(0,4)]]=maxbright;
      }
      for (int p=anzG;p>0;p--)
      {
        ValueLed[random(0,4)][LedGreen[random(0,4)][random(0,4)]]=maxbright;
      }
      for (int p=anzB;p>0;p--)
      {
        ValueLed[random(0,4)][LedBlue[random(0,4)][random(0,4)]]=maxbright;
      }
      FrameCount=0;
    
}


void AllOff()
{
  for(int l=0;l<4;l++)
  {
    for(int i=0;i<48;i++)
    {
      ValueLed[l][i]=0;
    }
  }
}
void AllInit()
{
  
    for(int i=0;i<48;i++)
    {
      Tlc.set(i,ValueLed[0][i]);
    }
    Tlc.update();
    digitalWrite(layers[0],LOW);  //layer 1
  
}
/*void mapping()
{
  for(int j = 0; j <4; j++)
  {
    switch(j)
    {
      case 0: digitalWrite(LayerFour, HIGH);
              digitalWrite(LayerOne, LOW);
              digitalWrite(LayerTwo, HIGH);
              digitalWrite(LayerThree, HIGH);
              break;
      case 1: digitalWrite(LayerFour, HIGH);
              digitalWrite(LayerOne, HIGH);
              digitalWrite(LayerTwo, LOW);
              digitalWrite(LayerThree, HIGH);
              break;
      case 2: digitalWrite(LayerFour, HIGH);
              digitalWrite(LayerOne, HIGH);
              digitalWrite(LayerTwo, HIGH);
              digitalWrite(LayerThree, LOW);
              break;
      case 3: digitalWrite(LayerFour, LOW);
              digitalWrite(LayerOne, HIGH);
              digitalWrite(LayerTwo, HIGH);
              digitalWrite(LayerThree, HIGH);
              break;
    }
    for(int i = 0; i <=47; i++)
    {
      Tlc.set(i, 4000);
      if(i==0) 
      {
        Tlc.set(47,0);
         Tlc.update();
      }
      else 
      //if(i>=1)
      {  
        Tlc.set(i0, 0);
      }
      Tlc.update();
      delay(1500);
      
      
  }
  

}
}

*/
  
 void CubeUpdate(int layerno)  //Updated Layer layerno(schickt Werte in TLCs, schaltet Layer layerno ein
 {
 
   for(int y=0;y<CUBE_SIZE;y++)
    {
      for(int x=0;x<CUBE_SIZE;x++)
      {
        Tlc.set(LedRed[y][x],ValueLed[layerno][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
        Tlc.set(LedGreen[y][x],ValueLed[layerno][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
        Tlc.set(LedBlue[y][x],ValueLed[layerno][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
      }
    }
  
    PORTC=B00111100;
    Tlc.update();
    delay(1);
    digitalWrite(layers[layerno],LOW);
    //delayMicroseconds(200);
    /*
  if(layerno == 1)
  {                 
    for(int y=0;y<4;y++)
    {
  		for(int x=0;x<4;x++)
  		{
  			Tlc.set(LedRed[y][x],ValueLed[0][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
  			Tlc.set(LedGreen[y][x],ValueLed[0][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
  			Tlc.set(LedBlue[y][x],ValueLed[0][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
  		}
    }
    
    digitalWrite(LayerOne, HIGH);   // sets LayerOne (pin A0) High (not active)--> Layer one OFF
    digitalWrite(LayerTwo, HIGH);   // sets LayerTwo (pin A1) High (not active)--> Layer two OFF
    digitalWrite(LayerThree, HIGH); // sets LayerThree (pin A2) High (not active)--> Layer three OFF
    digitalWrite(LayerFour, HIGH);  // sets LayerFour (pin A3) High (not active)--> Layer four OFF
    
    Tlc.update();                   // writes the values for the LEDs brightness to the tlc
      delay(1);                              // give the tlc some time to let the value settle
    digitalWrite(LayerOne, LOW);    // sets LayerOne (pin A0) Low (active)--> Layer one ON
    
  }
  
    if(layerno == 2){
    
    for(int y=0;y<4;y++)
    {
		for(int x=0;x<4;x++)
		{
			Tlc.set(LedRed[y][x],ValueLed[1][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
			Tlc.set(LedGreen[y][x],ValueLed[1][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
			Tlc.set(LedBlue[y][x],ValueLed[1][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
		}
    }
    
    digitalWrite(LayerOne, HIGH);  // same as if(layerno==1)
    digitalWrite(LayerTwo, HIGH);
    digitalWrite(LayerThree, HIGH);
    digitalWrite(LayerFour, HIGH);
   
    Tlc.update();
    delay(1);
    digitalWrite(LayerTwo, LOW);  // sets LayerTwo (pin A1) Low (active)--> Layer two ON
      
  }
  
  
  if(layerno == 3){
   
    for(int y=0;y<4;y++)
    {
		for(int x=0;x<4;x++)
		{
			Tlc.set(LedRed[y][x],ValueLed[2][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
			Tlc.set(LedGreen[y][x],ValueLed[2][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
			Tlc.set(LedBlue[y][x],ValueLed[2][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
		}
    }
    
    digitalWrite(LayerOne, HIGH);   // same as if(layerno==1)
    digitalWrite(LayerTwo, HIGH);
    digitalWrite(LayerThree, HIGH);
    digitalWrite(LayerFour, HIGH);
    
    Tlc.update();
    delay(1);
    digitalWrite(LayerThree, LOW);   // sets LayerThree (pin A2) Low (active)--> Layer three ON 
    
  }
  
  
  if(layerno == 4){
   
    for(int y=0;y<4;y++)
    {
		for(int x=0;x<4;x++)
		{
			Tlc.set(LedRed[y][x],ValueLed[3][LedRed[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); first TLC5940
			Tlc.set(LedGreen[y][x],ValueLed[3][LedGreen[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); second TLC5940 
			Tlc.set(LedBlue[y][x],ValueLed[3][LedBlue[y][x]]);             // set AaR brightness to AchR OUTPUT(OUTPUT 0); third TLC5940 
		}
    }
   
    digitalWrite(LayerOne, HIGH);  // same as if(layerno==1)
    digitalWrite(LayerTwo, HIGH);
    digitalWrite(LayerThree, HIGH);
    digitalWrite(LayerFour, HIGH);
     
    Tlc.update();
    delay(1);
    digitalWrite(LayerFour, LOW);  // sets LayerFour (pin A3) Low (active)--> Layer four ON
  }*/
	
}

  
 
  void checkBright()
  {
    if(maxbright>4000)
    {  
       maxbright=4000;
    }
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
		brightR=maxbright;
		brightG=0;
		brightB=0;
		zeichnelittleCube(1,0,2);		//Rot
		
		brightR=0;
		brightG=maxbright;
		brightB=0;
		zeichnelittleCube(3,2,2);		//Grün
		
		brightR=0;
		brightG=0;
		brightB=maxbright;
		zeichnelittleCube(1,2,0);		//Blau
		
		brightR=maxbright/2;
		brightG=0;
		brightB=maxbright/2;
		zeichnelittleCube(1,0,0);		//Rot+Blau
		
		brightR=maxbright/2;
		brightG=maxbright/2;
		brightB=0;
		zeichnelittleCube(3,0,2);		//Rot+Grün
		
		brightR=0;
		brightG=maxbright/2;
		brightB=maxbright/2;
		zeichnelittleCube(3,2,0);		//Blau+Grün

                brightR=maxbright;
		brightG=maxbright/2;
		brightB=maxbright/4;
		zeichnelittleCube(3,0,0);		//Rot+Grün+(blau+rot)
		
		brightR=maxbright/2;
		brightG=maxbright;
		brightB=maxbright/4;
		zeichnelittleCube(1,2,2);		//Grün+Blau+(grün+rot)

		
	}
      else if(FrameCount==1)
	{	
		brightR=maxbright;
		brightG=0;
		brightB=0;
		zeichnelittleCube(3,2,2);		//Rot
		
		brightR=0;
		brightG=maxbright;
		brightB=0;
		zeichnelittleCube(1,0,2);		//Grün
		
		brightR=0;
		brightG=0;
		brightB=maxbright;
		zeichnelittleCube(1,0,0);		//Blau
		
		brightR=maxbright/2;
		brightG=0;
		brightB=maxbright/2;
		zeichnelittleCube(1,2,0);		//Rot+Blau
		
		brightR=maxbright/2;
		brightG=maxbright/2;
		brightB=0;
		zeichnelittleCube(3,2,0);		//Rot+Grün
		
		brightR=0;
		brightG=maxbright/2;
		brightB=maxbright/2;
		zeichnelittleCube(3,0,2);		//Blau+Grün

                brightR=maxbright;
		brightG=maxbright/2;
		brightB=maxbright/4;
		zeichnelittleCube(1,2,2);		//Rot+Grün+(blau+rot)
		
		brightR=maxbright/2;
		brightG=maxbright;
		brightB=maxbright/4;
		zeichnelittleCube(3,0,0);		//Grün+Blau+(grün+rot)

		
	}
else if(FrameCount==2)
	{	
		brightR=maxbright;
		brightG=0;
		brightB=0;
		zeichnelittleCube(1,2,0);		//Rot
		
		brightR=0;
		brightG=maxbright;
		brightB=0;
		zeichnelittleCube(1,0,0);		//Grün
		
		brightR=0;
		brightG=0;
		brightB=maxbright;
		zeichnelittleCube(1,0,2);		//Blau
		
		brightR=maxbright/2;
		brightG=0;
		brightB=maxbright/2;
		zeichnelittleCube(3,2,2);		//Rot+Blau
		
		brightR=maxbright/2;
		brightG=maxbright/2;
		brightB=0;
		zeichnelittleCube(3,0,0);		//Rot+Grün
		
		brightR=0;
		brightG=maxbright/2;
		brightB=maxbright/2;
		zeichnelittleCube(1,2,0);		//Blau+Grün

                brightR=maxbright;
		brightG=maxbright/2;
		brightB=maxbright/4;
		zeichnelittleCube(3,0,2);		//Rot+Grün+(blau+rot)
		
		brightR=maxbright/2;
		brightG=maxbright;
		brightB=maxbright/4;
		zeichnelittleCube(3,2,0);		//Grün+Blau+(grün+rot)

		FrameCount=0;
	}
}

void zeichnelittleCube(int posl,int posy,int posx)
{
  for(int l=posl;l>posl-2;l--)
 {
   for(int y=posy;y<posy+2;y++)
  {
    for(int x=posx;x<posx+2;x++)
    {
      ValueLed[l][LedRed[y][x]]=brightR;
      ValueLed[l][LedGreen[y][x]]=brightG;
      ValueLed[l][LedBlue[y][x]]=brightB;
    }
			
  }
 }
}  
void aniTest()
{
  //Serial.println("---------FRAME----------");
  //micro=micros();
    for(int l=0;l<4;l++)
    {
      for(int y=0;y<4;y++)
      {
        for(int x=0;x<4;x++)
        {
          //animation                        : 1st LED Red 1st Frame
          //animation+1                      : 1st LED Green 1st Frame
          //animation+2                      : 1st LED Blue 1st Frame
          //animation+FrameCount*(NUM_TLC*16): 1st LED Red 2nd Frame
          //animation+(x*3+y*CUBE_SIZE*3)    : (x,y) LED Red 
          ValueLed[l][LedRed[y][x]]= pgm_read_word_near(animation+FrameCount*(NUM_TLCS *16*CUBE_SIZE)+l*NUM_TLCS*16+(x*3+y*CUBE_SIZE*3));
          ValueLed[l][LedGreen[y][x]]= pgm_read_word_near(animation+1+FrameCount*(NUM_TLCS *16*CUBE_SIZE)+l*NUM_TLCS*16+(x*3+y*CUBE_SIZE*3));
          ValueLed[l][LedBlue[y][x]]= pgm_read_word_near(animation+2+FrameCount*(NUM_TLCS *16*CUBE_SIZE)+l*NUM_TLCS*16+(x*3+y*CUBE_SIZE*3));
        }
      }
    }
    //Serial.println(micros()-micro);
}
