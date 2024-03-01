/*
 * C128 CP/M C Library C3L
 *
 * Convert 8 bit PCM to 4 bit PCM.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#pragma output noprotectmsdos

#include <cia.h>
#include <cpm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

/*
 Size of conversion buffer (make divisible by 8).
 */
#define BUF_SIZE 8192

/*
 Display program help.
 */
void dispHelp() {
	puts("\nconvpcm {U:D:}filespec {U:D:}filespec bits");
	puts(
			"convpcm FILENAME.SND FILENAME.RAW 4 (.SND is 8 bit and .RAW is 4 bit)");
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
	struct stat statRec;
	unsigned int bytesRead, bytesWrite;
	unsigned long cnvBytes, cnvStep, cnvNext, startCia, endCia;
	printf("\n");
	/* Check bits valid value */
	if (bits == 4 || bits == 2 || bits == 1) {
		if (stat(inFileName, &statRec) == 0) {
			if ((inFile = fopen(inFileName, "rb")) != NULL) {
				if ((outFile = fopen(outFileName, "wb")) != NULL) {
					cnvBytes = 0;
					/* Progress steps */
					cnvStep = statRec.st_size / 10;
					cnvNext = cnvStep;
					printf(
							"Converting %s, %ld bytes, ..........\b\b\b\b\b\b\b\b\b\b",
							inFileName, statRec.st_size);
					tens = inp(cia1 + ciaTodTen);
					/* Wait for tenth of a second to change */
					while (inp(cia1 + ciaTodTen) == tens)
						;
					startCia = todToMs(cia1);
					do {
						bytesRead = fread(buffer, sizeof(unsigned char),
								bufSize, inFile);
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
						fwrite(buffer, sizeof(unsigned char), bytesWrite,
								outFile);
						cnvBytes += bytesRead;
						while (cnvBytes >= cnvNext) {
							printf("*"); /* show progress */
							cnvNext += cnvStep;
						}
					} while (bytesRead == bufSize);
					fclose(outFile);
					endCia = todToMs(cia1);
					printf(", %lu ms\n", endCia - startCia);
				} else {
					puts("\nUnable to open output file.");
				}
				fclose(inFile);
			} else {
				puts("\nUnable to open input file.");
			}
		} else {
			puts("\nUnable to open input file.");
		}
	} else {
		puts("\nBits value must 4, 2 or 1.");
	}
}

/*
 * Main function.
 */
int main(int argc, char *argv[]) {
	unsigned char *buffer, bits;
	/* Make sure we have 4 params */
	if (argc == 4) {
		/* Alloc conversion buffer */
		buffer = (unsigned char*) malloc(BUF_SIZE);
		if (buffer != NULL) {
			/* Convert bits param to unsigned char */
			sscanf(argv[3], "%u", &bits);
			/* Check bits valid value */
			if (bits == 4 || bits == 2 || bits == 1) {
				/* Convert raw file */
				convert(argv[1], argv[2], buffer, BUF_SIZE, bits);
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
