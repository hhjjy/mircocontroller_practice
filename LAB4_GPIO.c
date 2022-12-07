#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"
#define GROUP_A_DOUT 0x008
#define GROUP_B_DOUT 0x048
#define GROUP_C_DOUT 0x088
#define GROUP_D_DOUT 0x0C8
#define GROUP_E_DOUT 0x108

#define GPIOA_MODE 0x000 
#define GPIOB_MODE 0x040 
#define GPIOC_MODE 0x080 
#define GPIOD_MODE 0x0C0 
#define GPIOE_MODE 0x100 
#define ms 1000 
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
void GPIO_Mode_Set(){}

void GPIO_Write(int group, int pin , int data)
{
		volatile unsigned int *reg ,reg_data; 
		reg = (unsigned int *)(GPIO_BASE + group) ; 
	// step 1 : clear that bit 
		reg_data = *reg & (~( 1 << pin )); // GGGGGGGG & 1111011111  = GGGG0GGGG
	// step 2 : set the bit which may be 1 or 0 
		*reg = (data << pin) | reg_data ;  // 
}
int count = 0 ; 
int start_pin = 12 ; 
volatile unsigned int *reg ; // Note : volatile tell the compiler not to touch any part of the variable itself 
int i , j ;
int x = 0 ; 
int tmp ; 
void mapping2_pin(int x)
{
	int t = 0xFFFFFFFF ; 
	int a = 0 ; 
	for(a = 0 ; a < 4 ; a ++)
	{
		if(x&(1<<a)) 
			t &= ~(1<<(15-a)) ; 
	}
	*reg = t ; 
}
int main(void)
{	
	UNLOCKREG();
	DrvSYS_Open(50000000);// set System Clock to run at 50MHz. 
	SYSCLK->CLKSEL0.STCLK_S = 0x3; // STCLK Clock Source.
	LOCKREG();		
	
	// GPIO IO MODE 
	reg = (volatile unsigned int *)(GPIO_BASE + 0x080) ; 
	*reg = 0x55555555;  //ALL SET TO PUSH PULL 
	
	// CONTROLL GPIOC DATA output 
	reg = (volatile unsigned int *)(GPIO_BASE + 0x088) ; 
	*reg = 0xFFFF ; 
	
	//*reg &= ~(1<<12) ; 
	
	

	/* METHOD 2 */
	while(1)
	{
		/* left to right 15->12 */
		for( i = 0 ; i < 3 ; i ++)
		{
			/* reset */
			tmp = 0 ; 
			for( j = 0 ; j < 4-i ; j ++)
			{
				// left shift 
				if (j == 0)
					tmp = (1<<0);
				else 
					tmp <<= 1 ; 
				mapping2_pin(x|tmp) ; 
				/* mapping */
				SYS_Delay(500*ms) ;  
			}
			// update 
			x |= tmp ; 
		}
		*reg = 0xFFFF ; 
		SYS_Delay(500*ms) ;  
		x = 0 ; 
		for( i = 0 ; i < 3 ; i ++)
		{
			/* reset */
			tmp = 0 ; 
			for( j = 0 ; j < 4-i ; j ++)
			{
				// right shift 
				if (j == 0)
					tmp = (1<<3);
				else 
					tmp = 1 >>1  ; 
				mapping2_pin(x|tmp) ; 
				/* mapping */
				SYS_Delay(500*ms) ;  
			}
			// update 
			x |= tmp ; 
		}
	}
	
	
	/* METHOD 1*/
	while(1)
	{
		switch(count++){
			case 0 :
				*reg = 0xFFFF ; 
				//*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 1 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 2 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<14) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 3 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<13) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 4 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 5 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<12) ;  // set pin x 
				*reg &= ~(1<<15) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 6 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<14) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 7 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<13) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 8 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<13) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 9 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<14) ;  // set pin x 
				*reg &= ~(1<<13) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 10 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<14) ;  // set pin x 
				*reg &= ~(1<<13) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ;	
			//  TYPE2 :
			case 11 :
				*reg = 0xFFFF ; 
				
				SYS_Delay(500*ms); 
				break ; 
			case 12:
				*reg = 0xFFFF ; 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 13 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<13) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 14 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<14) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 15 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 16 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 17 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<13) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 18 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<14) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 19 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<14) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 20 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<14) ;  // set pin x 
				*reg &= ~(1<<13) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 21 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				*reg &= ~(1<<14) ;  // set pin x 
				*reg &= ~(1<<13) ;  // set pin x 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			default:
				count = 0 ; 
				break ; 
		}

	}
	
}



