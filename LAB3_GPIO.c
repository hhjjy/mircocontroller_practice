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
void GPIO_Mode_Set()

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
	
	
	while(1)
	{
		switch(count++){
			case 0 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 1 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<13) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 2 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<14) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 3 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 4 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<15) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 5 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<14) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 6 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<13) ;  // set pin x 
				SYS_Delay(500*ms); 
				break ; 
			case 7 :
				*reg = 0xFFFF ; 
				*reg &= ~(1<<12) ;  // set pin x 
				SYS_Delay(500*ms); 
				count = 0 ; // reset counter 
				break ; 
			default:
				count = 0 ; 
				break ; 
		}

	}
	
}



