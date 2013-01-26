#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#define RTC_ADDR 0x70
#define b2b(bb) ((((bb) & 15) + ((bb) >> 6) * 10))

typedef struct time
{
	u32int	sec;
	u32int	min;
	u32int	hour;
	u32int	day;
	u32int	month;
	u32int	year;
} xtime_t;


/* Prototypes */
unsigned char readCMOS(unsigned char addr);
void          writeCMOS(unsigned char addr, u32int val);  // TODO: switch val to use time_t
xtime_t        read_RTCTime();

#endif
