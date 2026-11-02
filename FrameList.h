/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                  */
/* FrameList.h: header file for frame list manipulations             */
/*********************************************************************/

#ifndef FRAMELIST_H
#define FRAMELIST_H

#include "Frame.h"

typedef struct FrameEntry FENTRY;
typedef struct FrameList FLIST;

struct FrameEntry {
	FLIST *List;         /* Pointer to the list which this entry belongs to */
	FENTRY *Next;        /* Pointer to the next entry, or NULL */
	FENTRY *Prev;        /* Pointer to the previous entry, or NULL */
	Frame *RGBFrame;     /* Pointer to the RGB frame, or NULL */
	YUVFrame *YUVFrame;  /* Pointer to the YUV frame, or NULL */
};

struct FrameList {
	unsigned int Length;  /* Length of the list */
	FENTRY *First;        /* Pointer to the first entry, or NULL */
	FENTRY *Last;         /* Pointer to the last entry, or NULL */
};

/* Create a new frame list */
FLIST *CreateFrameList(void);

/* Delete a frame list (and all entries) */
void DeleteFrameList(FLIST *list);

/* Insert a RGB frame to the frame list at the end */
void AppendRGBFrame(FLIST *list, Frame *RGBframe);

/* Insert a YUV frame to the frame list at the end */
void AppendYUVFrame(FLIST *list, YUVFrame *YUVframe);

/* Crop a frame list */
void CropFrameList(FLIST *list, unsigned int start, unsigned int end);

/* Fast forward a frame list */
void FastFrameList(FLIST *list, unsigned int factor);

/* Reverse a frame list */
void ReverseFrameList(FLIST *list);

/* Insert a RGB frame to the frame list in the front */
void PrependRGBFrame(FLIST *list, Frame *RGBframe);

#endif
