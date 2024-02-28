/*
 * C128 6581/8580 SID functions.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <sid.h>
#include <stdlib.h>

/*
 * Start release cycle.
 */
void setSidRel(unsigned int Voice, unsigned char Waveform) {
	outp(Voice + 4, Waveform);
}
