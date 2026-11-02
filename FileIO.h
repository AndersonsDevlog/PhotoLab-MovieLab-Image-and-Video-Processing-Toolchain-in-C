/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                   */
/* FileIO.h: header file for basic file manipulations                */
/*********************************************************************/


#ifndef FILEIO_H
#define FILEIO_H

#include "Frame.h"

/* Read a frame from a file.                                        */
/* The size of the frame needs to be pre-set.                        */
/* The memory space of the frame will be allocated in this function. */
/* Return values:                                                    */
/* NULL: fail to load or create a frame                             */
/* frame: load or create a frame successfully                       */
Frame *LoadFrame(const char *fname);

/* Save a processed frame.             */
/* Return values:                      */
/* 0: successfully saved the frame     */
/* 1: Cannot open the file for writing */
/* 2: File error while writing to file */
int SaveFrame(const char *fname, const Frame *frame);

#endif
