/*
 * C128 CP/M C Library C3L
 *
 * Basline app.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void) {
	unsigned char *buffer;
	struct stat statRec;
	buffer = (unsigned char*) malloc(8192);
	if (stat("ciademo.com", &statRec) == 0) {
	puts("Hello World");}
	free(buffer);
	return EXIT_SUCCESS;
}
