/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                   */
/* Movie.h: header file for basic movie manipulations                */
/*********************************************************************/

#ifndef MOVIE_H
#define MOVIE_H

#include "FrameList.h"

/* the movie structure */
typedef struct {
	FLIST *Frames; /* Pointer to the frame list */
} Movie;

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void);

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie);

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(Movie *movie);

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(Movie *movie);


#endif
