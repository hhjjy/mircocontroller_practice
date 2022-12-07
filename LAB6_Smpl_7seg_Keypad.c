//
// Smpl_7seg_keypad
//
// Input:  3x3 keypad (input = 1~9 when key is pressed, =0 when key is not pressed
// Output: 7-segment LEDs
//
#include <stdio.h>																											 
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"

unsigned char seg[4] = {0x0,0x0,0x0,0x0};

#define SEG_N0   0x82 
#define SEG_N1   0xEE 
#define SEG_N2   0x07 
#define SEG_N3   0x46 
#define SEG_N4   0x6A  
#define SEG_N5   0x52 
#define SEG_N6   0x12 
#define SEG_N7   0xE6 
#define SEG_N8   0x02 
#define SEG_N9   0x62
unsigned char SEG_NUM[]={SEG_N0, SEG_N1, SEG_N2, SEG_N3, SEG_N4,SEG_N5,SEG_N6,SEG_N7,SEG_N8,SEG_N9}; 
uint32_t screen_num = 0 ; //

volatile unsigned int *reg_ptr;

void GPC_set(unsigned int pin)
{

reg_ptr = (unsigned int *)(0x50004000 + 0x8C);
*reg_ptr = (unsigned int)(0xf<<12);//DMASK
reg_ptr = (unsigned int *)(0x50004000 + 0x88);
*reg_ptr = (unsigned int)(1<<pin);//Set the display of 7-Seg 
}

void show_seven_segment(unsigned char place, unsigned char number)
{
  unsigned char temp,i;
	temp=SEG_NUM[number];
	
	for(i=0;i<8;i++)
	{
		if((temp&0x01)==0x01)
		{			
		   DrvGPIO_SetBit(E_GPE,i);
		}else{
		   DrvGPIO_ClrBit(E_GPE,i);		
		}			
		   temp=temp>>1;
		GPC_set(4+place);	
	}
}

void OpenKey_Pad(void)// done 
{
	/* Initial key pad */
	reg_ptr = (unsigned int*) (0x50004000 + 0x00);
	*reg_ptr = (unsigned int)(0xfff);
}

uint8_t Scan_key(void)
{
	uint8_t act[3]={0x3,0x5,0x6}; // Write your code here
	uint8_t i,temp,pin;
	volatile unsigned int * ptrPin;
	ptrPin = (unsigned int *) (0x50004000 + 0x10);

	for(i=0;i<3;i++)
	{
		temp=act[i];
		for(pin=0;pin<3;pin++)// SET GPA0-2 to output 
		{
			if((temp&0x01)==0x01)  
			{
				DrvGPIO_SetBit(E_GPA,pin); 
			}else{
				DrvGPIO_ClrBit(E_GPA,pin); 
			}
			temp>>=1;
		}
		
		if (((*ptrPin >>3 )& 0x1) == 0) { 
			return(i+1);
		} 
		if (((*ptrPin >>4 )& 0x1) == 0) {
			return(i+4);
		}
		if (((*ptrPin >>5 )& 0x1) == 0) {
			return(i+7);
		}
		
		// Write your code here
	}
		return 0;
}

void add(uint32_t n)
{ // Write your code here
	seg[3] = n/1000;
	n%= 1000; 
	seg[2] = n/100;
	n%= 100; 
	seg[1] = n/10;
	n%= 10; 
	seg[0] = n;
}

void show(void)
{
	int i;
	
	for(i = 0 ; i<4 ; i++){
		show_seven_segment(i,seg[i]);		
		DrvSYS_Delay(4000);//Run 4 times, 4*(4 ms) = 16 ms
	}
}


int32_t main (void)
{	
	int8_t number,temp_num,last_num;
	int count = 0 ; 
	UNLOCKREG();
  DrvSYS_Open(50000000);//Set System Clock to run at 50MHz. //about 20 ns
	LOCKREG();

	OpenKey_Pad();					 	
	
	// TASK1 : print the number of scan pin 
	
	while(1){	
		number = Scan_key();//Checking input number 2 times is for debouncing	
		temp_num = number;	
		
		if(number == temp_num){ //During the pushing button time (haven't relase the button)
														//or the other way around
			number = Scan_key();  //Keep running the loop during the steady state			
			show();
		}

		// main functon
		if(temp_num != 0  ){
			screen_num = ((screen_num *10) + temp_num)%10000  ; 
			add(screen_num);
			
			// wait for release which continue a lot of it 
			temp_num = 0 ; 
			while(1)
			{
				number = Scan_key();//Checking input number 2 times is for debouncing	
				if (temp_num == number)
					count += 1 ; 
				else 
					count = 0 ; 
				
				if (count > 1)
					break ; 	
				show();
				}
			}

		
	}
}
