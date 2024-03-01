/*
 * C128 CP/M C Library C3L
 *
 * Basline app.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(void) {
	int bytesRead;
	unsigned char *buffer;
	FILE *file;
	/* Alloc conversion buffer */
	buffer = (unsigned char*) malloc(BUF_SIZE);
	if ((file = fopen("ciademo.com", "rb")) != NULL) {
		bytesRead = fread(buffer, sizeof(unsigned char), BUF_SIZE, file);
		printf("%d byted read\n", bytesRead);
		fclose(file);
	}
	free(buffer);
	return EXIT_SUCCESS;
}
