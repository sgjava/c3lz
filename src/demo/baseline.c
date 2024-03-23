/*
 * C128 CP/M C Library C3L
 *
 * Basline app.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <common.h>
#include <cpm.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#pragma output noprotectmsdos
//#pragma output CRT_STACK_SIZE = 1024

/*
 * Process files based on standard CP/M search.
 */
processFiles(char *inFileName) {
	int curDisk, curUser, retVal, dmaOffset, i, j;
	char name[9], ext[4];
	node *head = NULL;
	struct fcb dirFcb, retFcb;
	// FCB is 36 bytes, but only 32 are returned by CPM_FFST and CPM_FNXT calls
	unsigned char dmaBuf[144];
	curDisk = bdos(CPM_IDRV, 0);
	curUser = bdos(CPM_SUID, 0xff);
	memset(name, ' ', sizeof(name) - 1);
	name[sizeof(name) - 1] = '\0';
	for (i = 0; i < sizeof(name) && inFileName[i] != '.'; i++) {
		name[i] = inFileName[i];
	}
	if (inFileName[i] == '.') {
		i++;
	}
	memset(ext, ' ', sizeof(ext) - 1);
	ext[sizeof(ext) - 1] = '\0';
	for (j = 0; j < sizeof(ext); j++) {
		ext[j] = inFileName[i++];
	}
	// Zero out FCB
	memset(dirFcb, 0, sizeof(dirFcb));
	memcpy(dirFcb.name, name, sizeof(dirFcb.name));
	memcpy(dirFcb.ext, ext, sizeof(dirFcb.ext));
	// Use current disk and user
	retVal = initDir(curDisk, curUser, &dirFcb, dmaBuf);
	if (retVal == 0) {
		// Copy first file name to FCB from DMA buffer
		memcpy(&retFcb, dmaBuf, sizeof(struct fcb));
		// Add to linked list
		insertEnd(&head, fcbToFileName(&retFcb));
		// Get the rest of the file names.
		getDir(&dirFcb, dmaBuf, head);
		while (head != NULL) {
			printf("File name: %s\n", head->data);
			head = head->next;
		}
		freeList(head);
	}
}

int main(void) {
	unsigned int total, largest;
	unsigned int bytesRead;
	unsigned char *buffer;
	FILE *file;
	mallinfo(&total, &largest);
	printf("Heap size %u largest block %u\n", total, largest);
	processFiles("????????.SND");
	return EXIT_SUCCESS;
}
