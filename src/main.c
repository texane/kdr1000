/*
** Made by fabien le mentec <texane@gmail.com>
** 
** Started on  Thu Nov 19 20:03:19 2009 texane
** Last update Sat Jun 12 07:32:51 2010 texane
*/



#pragma stack 0x200 255

#include <pic18fregs.h>
#include "osc.h"
#include "serial.h"
#include "int.h"


#define KDR_TRIS_NOT_CLS TRISAbits.TRISA0
#define KDR_PIN_NOT_CLS PORTAbits.RA0

#define KDR_TRIS_NOT_RCL TRISAbits.TRISA1
#define KDR_PIN_NOT_RCL PORTAbits.RA1

#define KDR_TRIS_NOT_RDT TRISAbits.TRISA2
#define KDR_PIN_NOT_RDT PORTAbits.RA2


static void kdr_setup(void)
{
  KDR_TRIS_NOT_CLS = 1;
}


static void initialize(void)
{
  ADCON1 = 0x0F;
  CMCON = 0x07;
  INTCON = 0;

  int_setup();
  osc_setup();
  serial_setup();
  kdr_setup();
}


void main(void)
{
  unsigned char value = 0;
  unsigned int prev = 0;
  unsigned int bit_index = 0;
  unsigned int buf_index = 0;
  unsigned char is_cls = 0;

  unsigned char buf[16];

  initialize();

  serial_writeb('*');
  serial_writeb('*');
  serial_writeb('*');
  serial_writeb('*');

  while(1)
  {
    prev = KDR_PIN_NOT_RDT;

    /* card is loading */
    while (!KDR_PIN_NOT_CLS)
    {
      is_cls = 1;
    }

    if (is_cls)
    {
      serial_writeb('x');
      is_cls = 0;
    }

#if 0
    {
      /* wait for not_rcl to be high */
      while (!KDR_PIN_NOT_RCL)
	;

      value |= (!(prev == KDR_PIN_NOT_RDT)) << bit_index;
      ++bit_index;
      if (bit_index == 8)
      {
	if (buf_index == 16)
	  break;

	buf[buf_index] = value;
	value = 0;
	bit_index = 0;
	++buf_index;
      }
      
      /* wait for not_rcl to be low */
/*       while (KDR_PIN_NOT_RCL) */
/* 	; */
    }

    if (buf_index)
    {
      unsigned int i;
      for (i = 0; i < buf_index; ++i)
      {
	if (buf[i] >= 0xa)
	  serial_writeb('a' + buf[i] - 0xa);
	else
	  serial_writeb('0' + buf[i]);
      }
      serial_writeb('\r');
      serial_writeb('\n');

      for (i = 0xffff; i > 0; --i)
	;

      buf_index = 0;
    }
#endif

#if 0
    unsigned int i;
    for (i = 0xbfff; i > 0; --i)
      ;

    serial_writeb(KDR_PIN_NOT_CLS + '0');
    serial_writeb(KDR_PIN_NOT_RDT + '0');
    serial_writeb(KDR_PIN_NOT_RCL + '0');
    serial_writeb('\r');
    serial_writeb('\n');
#endif
  }
}
