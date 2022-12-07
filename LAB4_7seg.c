#include <stdio.h>																											 
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"

//define 7seg codex here
#define SEG_0   0x82 
#define SEG_1   0xEE 
#define SEG_2   0x07 
#define SEG_3   0x46 
#define SEG_4   0x6A 
#define SEG_5   0x52 
#define SEG_6   0x12 
#define SEG_7		0xE6 
#define SEG_8   0x02 
#define SEG_9   0x42 
// Write your code

unsigned int SEG[]={SEG_0, SEG_1,SEG_2,SEG_3,SEG_4,SEG_5,SEG_6,SEG_7,SEG_8,SEG_9}; // Write your code

int sec, min ,x ;
int total_sec ; 

void SYS_Delay(unsigned int us)
{
		static unsigned char repeat;
	
		// If STCLK is 25M Hz.
	  //
		repeat = 25;
		SysTick->CTRL &= ~( 1 | 1 << 16 ); 
		SysTick->LOAD = us;
		SysTick->VAL  = 0;
		SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
		while(repeat--){
			/* Waiting for down-count to zero */
			while((SysTick->CTRL & (1 << 16)) == 0);
			SysTick->VAL  = 0;
		}	
}

void GPC_set(unsigned int place)
{	
	// Write your code
	// Enable all of the gpio port  and set to 1  
		if (place == 1 )
	{
		DrvGPIO_ClrBit(E_GPC,4); 
		DrvGPIO_ClrBit(E_GPC,5); 
		DrvGPIO_ClrBit(E_GPC,6); 
		DrvGPIO_ClrBit(E_GPC,7); 
		DrvGPIO_SetBit(E_GPC,4); 
	}
	else if (place == 2 )
	{
		DrvGPIO_ClrBit(E_GPC,4); 
		DrvGPIO_ClrBit(E_GPC,5); 
		DrvGPIO_ClrBit(E_GPC,6); 
		DrvGPIO_ClrBit(E_GPC,7); 
		DrvGPIO_SetBit(E_GPC,5); 
	}
	else if (place == 3 )
	{
		DrvGPIO_ClrBit(E_GPC,4); 
		DrvGPIO_ClrBit(E_GPC,5); 
		DrvGPIO_ClrBit(E_GPC,6); 
		DrvGPIO_ClrBit(E_GPC,7); 
		DrvGPIO_SetBit(E_GPC,6); 
	}
	else if (place == 4 )
	{
		DrvGPIO_ClrBit(E_GPC,4); 
		DrvGPIO_ClrBit(E_GPC,5); 
		DrvGPIO_ClrBit(E_GPC,6); 
		DrvGPIO_ClrBit(E_GPC,7); 
		DrvGPIO_SetBit(E_GPC,7); 
	}
	
	
}

void GPC_clear()
{

}
void show_seven_segment(unsigned int place, unsigned int number)
{
	unsigned int temp,i;
	temp=SEG[number];
	// Set segemnt 
	for(i=0;i<8;i++){
	  if((temp&0x01)==0x01){		   	  
			DrvGPIO_SetBit(E_GPE,i);
	  }else{
			DrvGPIO_ClrBit(E_GPE,i);	
	  }	  
			temp=temp>>1;
	}

	
	GPC_set(place);	// set up line segement 
}


void show_all_seven_segment(int min,int sec)
{
	show_seven_segment(1,sec%10);
	SYS_Delay(4000);  // 4000 us = 4 ms
		
	show_seven_segment(2,sec/10);
	SYS_Delay(4000);  // 4000 us = 4 ms
		
	show_seven_segment(3,min%10);
	SYS_Delay(4000);  // 4000 us = 4 ms
		
	show_seven_segment(4,min/10);
	SYS_Delay(4000);  // 4000 us = 4 ms
}
int32_t main (void)
{
	
	UNLOCKREG();
	DrvSYS_Open(50000000);// set System Clock to run at 50MHz. 
	SYSCLK->CLKSEL0.STCLK_S = 0x3; // STCLK Clock Source.
	LOCKREG();	
	
	
	min = 59 ; 
	sec = 50 ; 
	total_sec = min *60 + sec   ; 
	while(1)
	{	
		// Write your code
		
		// show and wait for time out 
		for ( x = 0 ; x < 50 ; x ++){
				show_all_seven_segment(min,sec) ;  //16ms 
				SYS_Delay(4*1000) ; // 4ms 
		}
		
		// count += 1 
		total_sec = (total_sec + 1)%3600  ; 
		min = total_sec /60 ; 
		sec = total_sec %60 ; 
	}	

}
