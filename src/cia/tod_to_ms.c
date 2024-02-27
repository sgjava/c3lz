/*
 * C128 6526 CIA demo functions..
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <cia.h>
#include <stdlib.h>

/*
 Convert TOD clock to milliseconds since midnight.
 */
unsigned long todToMs(unsigned int cia) {
	/* Reading hours first stops updating registers */
	unsigned char hour = inp(cia + ciaTodHrs);
	/* Bit 7 of hour is used as a flag for AM/PM (1 = PM, 0 = AM) */
	if (hour & 0x80) {
		hour = bcdToByte(hour) + 12;
	}
	return (hour * 3600000) + (bcdToByte(inp(cia + ciaTodMin)) * 60000)
			+ (bcdToByte(inp(cia + ciaTodSec)) * 1000)
			+ (bcdToByte(inp(cia + ciaTodTen)) * 100);
}
