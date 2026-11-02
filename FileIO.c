/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                   */
/*                                                                   */
/* Initial author: Tim Schmidt                                       */
/*                                                                   */
/* FileIO.c: source file for basic file manipulations                */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Constants.h"
#include "FileIO.h"
#include "Frame.h"

Frame *LoadFrame(const char *fname)
{
	FILE         *File;
	char         Type[SLEN];
	int          W, H, MaxValue;
	unsigned int x, y;
	char         fname_tmp[SLEN];
	Frame        *frame;
	strcpy(fname_tmp, fname);
	File = fopen(fname_tmp, "r");
	if (!File) {
#ifdef DEBUG
		printf("\nCan't open file \"%s\" for reading!\n", fname);
#endif
		return NULL;
	}

	fscanf(File, "%79s", Type);
	if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
#ifdef DEBUG
		printf("\nUnsupported file format!\n");
#endif
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &W);
	if (W <= 0) {
#ifdef DEBUG
		printf("\nUnsupported frame width %d!\n", W);
#endif
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &H);
	if (H <= 0) {
#ifdef DEBUG
		printf("\nUnsupported frame height %d!\n", H);
#endif
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &MaxValue);
	if (MaxValue != 255) {
#ifdef DEBUG
		printf("\nUnsupported frame maximum value %d!\n", MaxValue);
#endif
		fclose(File);
		return NULL;
	}
	if ('\n' != fgetc(File)) {
#ifdef DEBUG
		printf("\nCarriage return expected at the end of the file!\n");
#endif
		fclose(File);
		return NULL;
	}

	frame = CreateFrame(W, H);

	if (!frame) {
#ifdef DEBUG
		printf("\nError creating frame from %s!\n", fname_tmp);
#endif
		fclose(File);
		return NULL;
	}
	else {
		for (y = 0; y < frame->H; y++)
			for (x = 0; x < frame->W; x++) {
				SetPixelR(frame, x, y, fgetc(File));
				SetPixelG(frame, x, y, fgetc(File));
				SetPixelB(frame, x, y, fgetc(File));
			}

		if (ferror(File)) {
#ifdef DEBUG
			printf("\nFile error while reading from file!\n");
#endif
			DeleteFrame(frame);
			return NULL;
		}

#ifdef DEBUG
		printf("%s was read successfully!\n", fname_tmp);
#endif
		fclose(File);
		return frame;
	}
}

int SaveFrame(const char *fname, const Frame *frame)
{
    assert(frame != NULL && "No frame to save!\n");
	FILE         *File;
	int          x, y;
	char         SysCmd[SLEN * 5];
	char         fname_tmp[SLEN];
	char         fname_tmp2[SLEN];
	unsigned int Width = frame->W;
	unsigned int Height = frame->H;
	strcpy(fname_tmp, fname);
	strcpy(fname_tmp2, fname);

	File = fopen(fname_tmp2, "w");
	if (!File) {
#ifdef DEBUG
		printf("\nCan't open file \"%s\" for writing!\n", fname);
#endif
		return 1;
	}
	fprintf(File, "P6\n");
	fprintf(File, "%d %d\n", Width, Height);
	fprintf(File, "255\n");

	for (y = 0; y < Height; y++)
		for (x = 0; x < Width; x++) {
			fputc(GetPixelR(frame, x, y), File);
			fputc(GetPixelG(frame, x, y), File);
			fputc(GetPixelB(frame, x, y), File);
		}

	if (ferror(File)) {
#ifdef DEBUG
		printf("\nError while writing to file!\n");
#endif
		return 2;
	}
	fclose(File);
#ifdef DEBUG
	printf("%s was saved successfully. \n", fname_tmp2);
#endif
	sprintf(SysCmd, "/users/grad2/doemer/eecs22/bin/pnmtojpeg_hw4.tcsh %s",
			fname_tmp2);
	if (system(SysCmd) != 0) {
#ifdef DEBUG
		printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
#endif
		return 3;
	}
#ifdef DEBUG
	printf("%s.jpg was stored for viewing. \n", fname_tmp);
#endif
	return 0;
}

/* EOF */

