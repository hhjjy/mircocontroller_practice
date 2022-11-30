// Smpl_PWM_Music : use PWM to generate musical notes
//
// Output : PWM0 (GPB11)

#include <stdio.h>																											 
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "note_freq.h"

#define  D125ms 125000
#define  D190ms 190000
#define  D250ms 250000

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
int durations[] = {
  2, 4,
  4, 8, 4,
  2, 4,
  2, 
  2,
  4, 8, 4,
  2, 4,
  1, 
  4,
  
  4, 8, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1,
  4,
   
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1, 
  4, 4,  
  
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1
};
void PWM4_Init(void)
{ // us 
	PWMB->CSR.CSR0 = 4 ; // Divider Input clock divided by 1
	PWMB->PPR.CP01= 49  ; // prescaler divided by 50
	
	PWMB->PCR.CH0MOD = 1 ; // Auto-reload Mode.
	
	PWMB->CMR0 = 100;  // PWM Group A/B Comparator Register 
	PWMB->CNR0 = 1000;  // PWM Group A/B Counter Register 
	
	PWMB->POE.PWM0 = 1 ;  // PWM0: PWM output PWM channel x output to pin Enabled
	PWMB->PCR.CH0EN = 1 ;  // PWM Timer Starting.
}

void PWM_Freq(unsigned int freq, unsigned char duty_cycle)
	{ // duty from 0 % ~ 100 % 
	static unsigned short M, N;
	
	if(freq==0){
		M=0;
		N=0;
	}else{

		N =(1000000/freq)-1 ; 
		M =((1000000/freq)-1 )*(duty_cycle+1)/256;  
	}
	PWMB->CMR0 = M;
	PWMB->CNR0 = N;
}

void tone(unsigned int freq, unsigned int dtime)
{
	PWM_Freq(freq,98);
	DrvSYS_Delay(dtime * 1000);
}
void playsong()
{
		uint16_t music[] = {E6,  D6u,E6 ,D6u,E6 ,B5 ,D6 ,C6 ,A5 ,A5,0,0 ,C5 ,E5 ,A5 ,B5 ,B5 , 0 ,C5 ,A5 ,B5 ,C6 ,C6 , 0 ,E6 ,D6u,E6 ,D6u,E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0, C5 ,E5 ,A5 ,B5 ,B5 , 0 ,E5 ,C6 ,B5 ,A5 ,A5 , 0 ,B5 ,C6 ,D6 ,E6 ,E6 , 0 ,G5 ,F6 ,E6 ,D6 ,D6 , 0 ,F5 ,E6 ,D6 ,C6 ,C6 , 0 ,E5 ,D6 ,C6 ,B5 ,B5 , 0 ,E6 ,D6u,E6 ,D6u,E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0, C5 ,E5 ,A5 ,B5 ,B5 , 0 ,E5 ,C6 ,B5 ,A5 ,A5 , 0 }; 
		int x = 0 ; 
		int size = sizeof(music) ; 
		for (x = 0 ;  x < size/sizeof(int) ; x ++)
		{
			tone(music[x],250) ; 


		}
			
}
int main (void)
{
  int i, j, duty_cycle;
	
	UNLOCKREG();
	DrvSYS_Open(50000000);// set System Clock to run at 50MHz.
	SYSCLK->CLKSEL0.STCLK_S = 0x3; 
	SYSCLK->CLKSEL2.PWM45_S = 2 ; // PWM45 clock source
	SYSCLK->APBCLK.PWM45_EN = 1 ; // PWM45 clock enabled
	SYS->ALTMFP.PB11_PWM4 = 1  ; //PINMUX buzzer
	SYS->GPBMFP.TM3_PWM4 = 1  ; //PINMUX 
	LOCKREG();

	PWM4_Init();
  PWM_Freq(20,60);
	
	
	while(1) // freq and delay time 
	{
		playsong();

		
		}
	
}
