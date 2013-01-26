/*
 * rtc.c: CMOS Real Time Clock driver
 *
 * Written by Aaron Blakely <aaron@ephasic.org>
 */

#include "rtc.h"
#include <stdint.h>
#include <iomem.h>

unsigned char readCMOS(unsigned char addr)
{
	outportb(RTC_ADDR, addr);
	asm volatile("jmp 1f; 1: jmp 1f; 1:");
	addr = inportb(0x71);
	asm volatile("jmp 1f; 1: jmp 1f; 1:");

	return addr;
}

xtime_t read_RTCTime()
{
	xtime_t *ct;
	asm volatile("cli");	// disable interrupts while reading the RTC

	ct->sec		= (int)(b2b(readCMOS(0x0)));		// seconds
	ct->min		= (int)(b2b(readCMOS(0x2)));		// mins
	ct->hour	= (int)(b2b(readCMOS(0x4)));		// hours
	ct->day		= (int)(b2b(readCMOS(0x7)));		// days
	ct->month	= (int)(b2b(readCMOS(0x8)));		// months
	ct->year	= (int)(b2b(readCMOS(0x9)));		// years

	asm volatile("sti");	// re-enable interrupts after reading RTC

	return *ct;
}
