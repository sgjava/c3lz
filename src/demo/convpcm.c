/*
 * C128 CP/M C Library C3L
 *
 * Convert 8 bit PCM to 4 bit PCM.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#pragma output noprotectmsdos

#include <cia.h>
#include <common.h>
#include <cpm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Size of conversion buffer (make divisible by 2, 4 and 8).
 */
#define BUF_SIZE 8192

/*
 Display program help.
 */
void dispHelp() {
	puts("\nconvpcm input output bits");
	puts(
			"convpcm FILENAME.SND 4 (FILENAME.SND is 8 bit and FILENAM.RAW is 4 bit)");
}

/*
 Convert 8 bit raw data to 4 bit raw data.
 */
unsigned int convert8to4(unsigned char *buffer, unsigned int bufSize) {
	unsigned int i;
	for (i = 0; i < bufSize; i += 2) {
		buffer[i >> 1] = (buffer[i] & 0xf0) | (buffer[i + 1] >> 4);
	}
	/* Two 4 bit samples in a byte */
	return bufSize / 2;
}

/*
 Convert 8 bit raw data to 2 bit raw data.
 */
unsigned int convert8to2(unsigned char *buffer, unsigned int bufSize) {
	unsigned int i, s;
	unsigned char sample;
	for (i = 0, s = 0; i < bufSize; i += 4, ++s) {
		sample = ((buffer[i] >> 6) & 0x03) | ((buffer[i + 1] >> 4) & 0x0c)
				| ((buffer[i + 2] >> 2) & 0x30) | (buffer[i + 3] & 0xc0);
		buffer[s] = sample;
	}
	/* Four 2-bit samples in a byte */
	return bufSize / 4;
}

/*
 Convert 8 bit raw data to 1 bit raw data.
 */
unsigned int convert8to1(unsigned char *buffer, unsigned int bufSize) {
	static unsigned char bitTable[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04,
			0x02, 0x01 };
	unsigned char sample;
	unsigned int i, b;
	for (i = 0; i < bufSize; i += 8) {
		sample = 0x00;
		/* Pack 8 bytes into 1 byte */
		for (b = 0; b < 8; ++b) {
			if (buffer[i + b] > 0x7f) {
				sample = sample | (bitTable[b]);
			}
		}
		buffer[i >> 3] = sample;
	}
	/* Eight 1 bit samples in a byte */
	return bufSize / 8;
}

/*
 Convert 8 bit raw data to 4, 2 or 1 bit raw data.
 */
void convert(char *inFileName, char *outFileName, unsigned char *buffer,
		unsigned int bufSize, unsigned char bits) {
	unsigned char tens;
	FILE *inFile, *outFile;
	unsigned int bytesRead, bytesWrite;
	unsigned long startCia, endCia;
	printf("\n");
	/* Check bits valid value */
	if (bits == 4 || bits == 2 || bits == 1) {
		if ((inFile = fopen(inFileName, "rb")) != NULL) {
			if ((outFile = fopen(outFileName, "wb")) != NULL) {
				printf("Converting %s to %s in ", inFileName, outFileName);
				tens = inp(cia1 + ciaTodTen);
				/* Wait for tenth of a second to change */
				while (inp(cia1 + ciaTodTen) == tens)
					;
				startCia = todToMs(cia1);
				do {
					bytesRead = fread(buffer, sizeof(unsigned char), bufSize,
							inFile);
					/* Convert 8 bit to 4, 2 or 1 */
					switch (bits) {
					case 1:
						bytesWrite = convert8to1(buffer, bytesRead);
						break;
					case 2:
						bytesWrite = convert8to2(buffer, bytesRead);
						break;
					case 4:
						bytesWrite = convert8to4(buffer, bytesRead);
						break;
					}
					fwrite(buffer, sizeof(unsigned char), bytesWrite, outFile);
				} while (bytesRead == bufSize);
				fclose(outFile);
				endCia = todToMs(cia1);
				printf("%lu ms\n", endCia - startCia);
			} else {
				puts("\nUnable to open output file.");
			}
			fclose(inFile);
		} else {
			puts("\nUnable to open input file.");
		}
	} else {
		puts("\nBits value must 4, 2 or 1.");
	}
}

void changeExt(char* filename, const char* newExtension) {
    char* dot = strrchr(filename, '.'); // Find the last occurrence of '.'
    if (dot != NULL) {
        strcpy(dot + 1, newExtension); // Copy the new extension over the old one
    } else {
        strcat(filename, "."); // Add a '.' if there is no extension
        strcat(filename, newExtension); // Add the new extension
    }
}

/*
 * Process files based on standard CP/M search.
 */
processFiles(char *inFileName, unsigned char *buffer, unsigned char bits) {
	int curDisk, curUser, retVal, dmaOffset, i, j;
	char name[9], ext[4], dest[13];
	node *head = NULL;
	struct fcb dirFcb, retFcb;
	curDisk = bdos(CPM_IDRV, 0);
	curUser = bdos(CPM_SUID, 0xff);
	// FCB is 36 bytes, but only 32 are returned by CPM_FFST and CPM_FNXT calls
	unsigned char dmaBuf[144];
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
	// Use current disk and user
	retVal = initDir(curDisk, curUser, name, ext, &dirFcb, dmaBuf);
	if (retVal == 0) {
		// Copy first file name to FCB from DMA buffer
		memcpy(&retFcb, dmaBuf, sizeof(struct fcb));
		// Add to linked list
		insertEnd(&head, fcbToFileName(&retFcb));
		// Get the rest of the file names.
		getDir(&dirFcb, dmaBuf, head);
		while (head != NULL) {
			strcpy(dest, head->data);
			changeExt(dest, "RAW");
			printf("File name: %s\n", head->data);
			convert(head->data, dest, buffer, BUF_SIZE, bits);
			head = head->next;
		}
		freeList(head);
	}
}

/*
 * Main function.
 */
int main(int argc, char *argv[]) {
	unsigned char *buffer, bits;
	/* Make sure we have 3 params */
	if (argc == 3) {
		/* Alloc conversion buffer */
		buffer = (unsigned char*) malloc(BUF_SIZE);
		if (buffer != NULL) {
			/* Convert bits param to unsigned char */
			sscanf(argv[2], "%u", &bits);
			/* Check bits valid value */
			if (bits == 4 || bits == 2 || bits == 1) {
				processFiles(argv[1], buffer, bits);
				/* Convert raw file */
				//convert(argv[1], argv[2], buffer, BUF_SIZE, bits);
			} else {
				puts("\nBits value must 4, 2 or 1.");
			}
			/* Dispose buffer */
			free(buffer);
		}
	} else {
		dispHelp();
	}
	return EXIT_SUCCESS;
}
