#include "stm32f10x.h"
#include "lcd.h"
#include "init.h"
#include "literki.h"

void HI(uint16_t GPIO_Pin){
  STER->BSRR = GPIO_Pin; 
}


void LOW(uint16_t GPIO_Pin){
  STER->BRR = GPIO_Pin;
  }


void cog_wr0(uint16_t command){
	GPIO_Write(PORT, command);
        LOW(cs);
	LOW(rs);
        LOW(wr);
        Delay(10);
        HI(wr);			
	HI(rs);
        HI(cs);
}

void cog_wr1(uint16_t command, uint16_t p1){
	GPIO_Write(PORT, command);
	LOW(cs);
	LOW(rs);
        LOW(wr);
        Delay(10);
	HI(wr);			
	HI(rs);


	GPIO_Write(PORT, p1);
        LOW(wr);
        Delay(10);
        HI(wr);
        HI(cs);
}




void cog_wr4(uint16_t command, uint16_t p1, uint16_t p2, uint16_t p3, uint16_t p4){
	GPIO_Write(PORT,command);
	LOW(cs);
	LOW(rs);
	LOW(wr);
	Delay(10);
	HI(wr);			// tutaj wczytuje dopiero
	HI(rs);
	GPIO_Write(PORT, p1);
	LOW(wr);
	Delay(10);
	HI(wr);
	GPIO_Write(PORT, p2);
	LOW(wr);
	Delay(10);
	HI(wr);
	GPIO_Write(PORT, p3);
	LOW(wr);
	Delay(10);
	HI(wr);
	GPIO_Write(PORT, p4);
	LOW(wr);
	Delay(10);
	HI(wr);
	HI(cs);
}

void cog_write(uint16_t color, uint16_t n){

                LOW(cs);

		GPIO_Write(PORT, 0x2C); //14.2.22 memory write s.121
                LOW(rs);
		LOW(wr);
                Delay(10);
		HI(wr);
		HI(rs);
                
		while(n--){
                                //wszystkie piksele
				GPIO_Write(PORT, (color>>8)&0xff);
                                
                                LOW(wr);
                                Delay(10);
				HI(wr);

                                Delay(10);

                                GPIO_Write(PORT, (color)&0xff);
                                
                                LOW(wr);
                                Delay(10);
				HI(wr);

                              
		}
		HI(cs);
}


void cog_write_letter(uint8_t code, uint8_t x, uint8_t y){
                uint8_t height,width;
                uint16_t tmp;
                
                if(code==32)    code=79; //spacja
                else code-=48;
  
                cog_wr4(0x2A,0,x,0,x+9);
                cog_wr4(0x2B,0,y,0,y+15);
  
                LOW(cs);
  
                GPIO_Write(PORT, 0x2C); //14.2.22 memory write s.121
                LOW(rs);
		LOW(wr);
                Delay(10);
		HI(wr);
		HI(rs);
                
                                
                  for(width=0;width<16;width++){
                    
                    for(height=0;height<10;height++){
                      if(litery[code][width*10+9-height]==0)     tmp=0x0000;
                      else tmp=0xffff;
                      
                    
                        GPIO_Write(PORT, (tmp>>8)&0xff);                
                        LOW(wr);
                        Delay(10);
                        HI(wr);

                        GPIO_Write(PORT, tmp & 0xff);
                        LOW(wr);
                        Delay(10);
                        HI(wr);  
                  }
                }
                
		HI(cs);
}


void cog_write_number(uint8_t code, uint8_t x, uint8_t y){
                uint8_t height,width;
                uint16_t tmp;
                
                x=150-x;
                
                cog_wr4(0x2A,0,x,0,x+9);
                cog_wr4(0x2B,0,y,0,y+15);
  
                LOW(cs);
  
                GPIO_Write(PORT, 0x2C); //14.2.22 memory write s.121
                LOW(rs);
		LOW(wr);
                Delay(10);
		HI(wr);
		HI(rs);
                
                                
                  for(width=0;width<16;width++){
                    
                    for(height=0;height<10;height++){
                      if(litery[code][width*10+9-height]==0)     tmp=0x0000;
                      else tmp=0xffff;
                      
                    
                        GPIO_Write(PORT, (tmp>>8)&0xff);                
                        LOW(wr);
                        Delay(10);
                        HI(wr);

                        GPIO_Write(PORT, tmp & 0xff);
                        LOW(wr);
                        Delay(10);
                        HI(wr);  
                  }
                }
                
		HI(cs);
}


void cog_write_string(char *str, uint8_t x, uint8_t y){
  int i,l=dl(str),t=0;
  
  if((160-x)/11<l);  //za d³ugi
  else
  {
    x=150-x;
    for (i=0;i<l;i++){
      cog_write_letter(*str,x-t,y);
      t+=11;str++;
    }
  }
}
  

int dl(char *s){
  char *p=s;
  while(*p!='\0')       p++;
  return p-s;
}
  
