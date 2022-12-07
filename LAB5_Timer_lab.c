#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvTIMER.h"
#include "Driver\DrvGPIO.h"
int RED = 255;
int GREEN = 255;
int BLUE = 255;
int status = 0 ;
int tmp = 0 ; // which stores the value of color table  
void SYS_Delay(unsigned int us)
{
		static unsigned char repeat;	
		// If STCLK is 25M Hz.
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

void TMR0_IRQHandler1(void)
{
	//Interrupt Process
//

	status = (status+1)% 6; 
	

	TIMER0->TISR.TIF = 1  ;//clear Interrupt flag
}

void PWM0_Init(void)
{
	//1. Setup clock source divider select register (CSR
	
	//2. Setup prescaler (PPR)
	
	
	//3. Setup inverter on/off, dead zone generator on/off, auto-reload/one-shot mode and StopPWM-timer (PCR)
	
	//4. Setup comparator register (CMR) for setting PWM duty.
	
	//5. Setup PWM down-counter register (CNR) for setting PWM period.
	
	//6. Setup interrupt enable register (PIER) (option)
	
	//7. Setup corresponding GPIO pins as PWM function (enable POE and disable CAPENR)for the corresponding PWM channel.
	
	//8. Enable PWM timer start running (Set CHxEN = 1 in PCR)

}
void TIMER0_Init(void)
{
	// Timer Clock Source
	SYSCLK->CLKSEL1.TMR0_S = 2 ; // Clock source from HCLK.
	
	//Enable Timer0 clock source
	SYSCLK->APBCLK.TMR0_EN = 	1;	
	
	// Enable Timer0 Interrupt
	NVIC_EnableIRQ(TMR0_IRQn);	
	
	// Setting Mode
  // 00 -> One-shot
	// 01 -> Periodic
	// 10 -> Toggle
	// 11 -> Continuous counting mode
	TIMER0->TCSR.MODE = 1 ;
	
	// Setting Prescale 50M/Prescler 
	TIMER0->TCSR.PRESCALE = 49 ;
	
	// Data Load Enable
	TIMER0->TCSR.TDR_EN = 1 ;
	
	// Counter Mode Enable Bit
	TIMER0->TCSR.CTB = 0;
	
	// Enable interrupt
	TIMER0->TCSR.IE = 1;
	
	// Timer Compared value
	TIMER0->TCMPR = 1000000;
	
	// Timer Counter reset
	TIMER0->TCSR.CRST = 0;
	
	// Timer Counter Enable
	TIMER0->TCSR.CEN = 1;	
}

void GPIO_Open()
{
	
}

int main(void)
{
	int i, j;
	static int R, G, B;

	int Color_Tab[7] = {
											0xFF0000,  // Red
											0xFF8000,  // Orange
											0xFFFF00,  // Yellow
											0x00FF00,  // Green
											0x0000FF,  // Blue
											0xFF00FF,  // Deep blue
										};   // 6 colors 

	UNLOCKREG();
	DrvSYS_Open(50000000);// set System Clock (HCLK) to run at 50MHz 
	LOCKREG();
	
	//Set the RGB pin mode(GPIO_PMD).
	GPIO_Open();
	
	// Init Timer.
	TIMER0_Init();

	while(1)
	{			
		//Main Process
		// R
		tmp = Color_Tab[status]; 
		if (i < ((tmp >>16)&0xFF))
			DrvGPIO_ClrBit(E_GPA,14); 
		else 
			DrvGPIO_SetBit(E_GPA,14); 
		// G
		if (i < ((tmp >> 8)&0xFF ))
			DrvGPIO_ClrBit(E_GPA,13); 
		else
			DrvGPIO_SetBit(E_GPA,13); 

		
		
		// B
		if (i < ((tmp) & 0xFF))
			DrvGPIO_ClrBit(E_GPA,12); 
		else 
			DrvGPIO_SetBit(E_GPA,12); 
		i = (i+1)%256;
	}
}
