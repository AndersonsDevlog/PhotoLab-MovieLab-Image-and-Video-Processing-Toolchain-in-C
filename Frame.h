/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                   */
/* Frame.h: header file for basic frame manipulations                */
/*********************************************************************/

#ifndef FRAME_H
#define FRAME_H

typedef struct {
	unsigned int W;       /* Frame width */
	unsigned int H;       /* Frame height */
	unsigned char *R;     /* Pointer to the memory storing */
	                      /* all the R intensity values */
	unsigned char *G;     /* Pointer to the memory storing */
	                      /* all the G intensity values */
	unsigned char *B;     /* Pointer to the memory storing */
	                      /* all the B intensity values */
} Frame;

/* Get the intensity value of the Red channel of pixel (x, y) */
/* in the RGB frame */
unsigned char GetPixelR(const Frame *frame, unsigned int x, unsigned int y);

/* Get the intensity value of the Green channel of pixel (x, y) */
/* in the RGB frame */
unsigned char GetPixelG(const Frame *frame, unsigned int x, unsigned int y);

/* Get the intensity value of the Blue channel of pixel (x, y) */
/* in the RGB frame */
unsigned char GetPixelB(const Frame *frame, unsigned int x, unsigned int y);

/* Set the intensity value of the Red channel of pixel (x, y) */
/* in the RGB frame with valueR */
void SetPixelR(Frame *frame, unsigned int x, unsigned int y,
               unsigned char valueR);

/* Set the intensity value of the Green channel of pixel (x, y) */
/* in the RGB frame with valueG */
void SetPixelG(Frame *frame, unsigned int x, unsigned int y,
               unsigned char valueG);

/* Set the intensity value of the Blue channel of pixel (x, y) */
/* in the RGB frame with valueB */
void SetPixelB(Frame *frame, unsigned int x, unsigned int y,
               unsigned char valueB);

/* Allocate the memory space for the RGB frame and the memory spaces */
/* for the RGB intensity values. Return the pointer to the RGB frame. */
Frame *CreateFrame(unsigned int width, unsigned int height);

/* Release the memory spaces for the RGB intensity values. */
/* Release the memory space for the RGB frame. */
void DeleteFrame(Frame *frame);

unsigned int FrameWidth(const Frame *frame);
unsigned int FrameHeight(const Frame *frame);

typedef struct {
	unsigned int W;       /* Frame width */
	unsigned int H;       /* Frame height */
	unsigned char *Y;     /* Pointer to the memory storing */
	                      /* all the Y intensity values */
	unsigned char *U;     /* Pointer to the memory storing */
	                      /* all the U intensity values */
	unsigned char *V;     /* Pointer to the memory storing */
	                      /* all the V intensity values */
} YUVFrame;

/* Get the intensity value of the Y channel of pixel (x, y) */
/* in the YUV frame */
unsigned char GetPixelY(const YUVFrame *YUVframe, unsigned int x, unsigned int y);

/* Get the intensity value of the U channel of pixel (x, y) */
/* in the YUV frame */
unsigned char GetPixelU(const YUVFrame *YUVframe, unsigned int x, unsigned int y);

/* Get the intensity value of the V channel of pixel (x, y) */
/* in the YUV frame */
unsigned char GetPixelV(const YUVFrame *YUVframe, unsigned int x, unsigned int y);

/* Set the intensity value of the Y channel of pixel (x, y) */
/* in the YUV frame with valueY */
void SetPixelY(YUVFrame *YUVframe, unsigned int x, unsigned int y,
               unsigned char valueY);

/* Set the intensity value of the U channel of pixel (x, y) */
/* in the YUV frame with valueU */
void SetPixelU(YUVFrame *YUVframe, unsigned int x, unsigned int y,
               unsigned char valueU);

/* Set the intensity value of the V channel of pixel (x, y) */
/* in the YUV frame with valueV */
void SetPixelV(YUVFrame *YUVframe, unsigned int x, unsigned int y,
               unsigned char valueV);

/* Allocate the memory space for the YUV frame and the memory spaces */
/* for the YUV intensity values. Return the pointer to the YUV frame. */
YUVFrame *CreateYUVFrame(unsigned int width, unsigned int height);

/* Release the memory spaces for the YUV intensity values. */
/* Release the memory space for the YUV frame. */
void DeleteYUVFrame(YUVFrame *YUVframe);

/* Copy RGB Frame */
Frame *CopyRGBFrame(Frame *SrcFrame);

#endif
