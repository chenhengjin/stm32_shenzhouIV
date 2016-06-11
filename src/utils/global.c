/*******************************************************************************
File name  : global.c
Description: the basic types and others
Created  at: 2016-04-17
Created  by: chenheng
Modified at:
         by:

*******************************************************************************/


/*******************************************************************************
	Function Description:
		software time delay
	Parameters:
		[count]		used to define delay time
	Return:
		none
*******************************************************************************/
#include "stm32f10x.h"
#include "global.h"


void delay(uint32_t count)
{
	unsigned int i;

  	for(i=0; i<count; i++);
}



void delay_ms(uint16_t time)
{
	uint16_t i = 0;
    
	while (time--)
	{
		i = 12000;
		while(i--);
	}
}

void delay_us(uint16_t time)
{
	uint16_t i = 0;
    
	while (time--)
	{
		i = 10;
		while(i--);
	}
}



