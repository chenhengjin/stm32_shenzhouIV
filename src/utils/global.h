/*******************************************************************************
File name  : init.h
Description: the basic types and others
Created  at: 2011-09-20
Created  by: benjamin
			 benhaha@yeah.net(send your suggestion to me about my works)
Modified at:
         by:

*******************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "stm32f10x.h"


#ifndef	NULL
#define	NULL	(0)
#endif	/* NULL */


#define DISABLE_INTERRUPT() do{__disable_irq();}while(0)

typedef enum {FALSE = 0, TRUE = !FALSE}bool_t;


#define MIN(a,b) (((a)<(b))? (a):(b))

void delay(uint32_t count);

void delay_us(uint16_t time);

void delay_ms(uint16_t time);


#endif
