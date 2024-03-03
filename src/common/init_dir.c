/*
 * C128 CP/M C Library C3L
 *
 * Common code.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <common.h>
#include <cpm.h>
#include <stdlib.h>
#include <string.h>

/*
 * Initialize BDOS to retrieve directory. Search for the first occurrence of the
 * specified file; the filename should be stored in the supplied FCB. The filename
 * can include ? marks, which match anything on disc. If the first byte of the FCB
 * is ?, then any directory entry (including disc labels, date stamps etc.) will
 * match. The EX byte is also checked; normally it should be set to zero, but if it
 * is set to ? then all suitable extents are matched.
 */
int initDir(int login, int user, const char *name, const char *ext,
		struct fcb *fcb, unsigned char dmaBuf[]) {
	int retVal;
	// Login into disk
	retVal = bdos(CPM_LGIN, login);
	if (retVal == 0) {
		// Set user
		retVal = bdos(CPM_SUID, user);
		if (retVal == 0) {
			// Set the Direct Memory Access address
			retVal = bdos(CPM_SDMA, dmaBuf);
			if (retVal == 0) {
				// Default drive
				fcb->drive = 0;
				// Default UID
				fcb->uid = 0;
				memcpy(fcb->name, name, sizeof(fcb->name));
				memcpy(fcb->ext, ext, sizeof(fcb->ext));
				fcb->extent = 0;
				fcb->next_record = 0;
				// This should return -1 if no matching search or other failure
				retVal = bdos(CPM_FFST, fcb);
			}
		}
	}
	return retVal;
}
