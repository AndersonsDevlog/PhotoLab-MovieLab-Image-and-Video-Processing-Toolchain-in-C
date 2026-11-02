/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                  */
/* Frame.c: source file for basic image manipulations                */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Frame.h"

/* Get the intensity value of the Red channel of pixel (x, y) */
/* in the RGB frame */
unsigned char GetPixelR(const Frame *frame, unsigned int x, unsigned int y)
{
	assert(frame);

	assert(x < frame->W);
	assert(y < frame->H);

	assert(frame->R);
	assert(frame->G);
	assert(frame->B);

	return frame->R[x + y * frame->W];
}

/* Get the intensity value of the Green channel of pixel (x, y) */
/* in the RGB frame */
unsigned char GetPixelG(const Frame *frame, unsigned int x, unsigned int y)
{
	assert(frame);

	assert(x < frame->W);
	assert(y < frame->H);

	assert(frame->R);
	assert(frame->G);
	assert(frame->B);

	return frame->G[x + y * frame->W];
}

/* Get the intensity value of the Blue channel of pixel (x, y) */
/* in the RGB frame */
unsigned char GetPixelB(const Frame *frame, unsigned int x, unsigned int y)
{
	assert(frame);

	assert(x < frame->W);
	assert(y < frame->H);

	assert(frame->R);
	assert(frame->G);
	assert(frame->B);

	return frame->B[x + y * frame->W];
}

/* Set the intensity value of the Red channel of pixel (x, y) */
/* in the RGB frame with valueR */
void SetPixelR(Frame *frame, unsigned int x, unsigned int y,
               unsigned char valueR)
{
	assert(frame);

	assert(x < frame->W);
	assert(y < frame->H);

	assert(frame->R);
	assert(frame->G);
	assert(frame->B);

	frame->R[x + y * frame->W] = valueR;
}

/* Set the intensity value of the Green channel of pixel (x, y) */
/* in the RGB frame with valueG */
void SetPixelG(Frame *frame, unsigned int x, unsigned int y,
               unsigned char valueG)
{
	assert(frame);

	assert(x < frame->W);
	assert(y < frame->H);

	assert(frame->R);
	assert(frame->G);
	assert(frame->B);

	frame->G[x + y * frame->W] = valueG;
}

/* Set the intensity value of the Blue channel of pixel (x, y) */
/* in the RGB frame with valueB */
void SetPixelB(Frame *frame, unsigned int x, unsigned int y,
               unsigned char valueB)
{
	assert(frame);

	assert(x < frame->W);
	assert(y < frame->H);

	assert(frame->R);
	assert(frame->G);
	assert(frame->B);

	frame->B[x + y * frame->W] = valueB;
}

/* Allocate the memory space for the RGB frame and the memory spaces */
/* for the RGB intensity values. Return the pointer to the RGB frame. */
Frame *CreateFrame(unsigned int width, unsigned int height)
{
	Frame *frame = (Frame *)malloc(sizeof(Frame));
	if (frame == NULL) {
		return NULL;
	}

	frame->W = width;
	frame->H = height;

	frame->R = (unsigned char*)malloc(width * height * sizeof(unsigned char));
	if (frame->R == NULL) {
		free(frame);
		return NULL;
	}

	frame->G = (unsigned char*)malloc(width * height * sizeof(unsigned char));
	if (frame->G == NULL) {
		free(frame->R);
		free(frame);
		return NULL;
	}

	frame->B = (unsigned char*)malloc(width * height * sizeof(unsigned char));
	if (frame->B == NULL) {
		free(frame->G);
		free(frame->R);
		free(frame);
		return NULL;
	}

	return frame;
}

/* Release the memory spaces for the RGB intensity values. */
/* Release the memory space for the RGB frame. */
void DeleteFrame(Frame *frame)
{
	assert(frame);
	assert(frame->R);
	assert(frame->G);
	assert(frame->B);

	free(frame->R);
	free(frame->G);
	free(frame->B);
	frame->R = NULL;
	frame->G = NULL;
	frame->B = NULL;

	free(frame);
}

unsigned int FrameWidth(const Frame *frame){
	assert(frame);
	return frame->W;
}

unsigned int FrameHeight(const Frame *frame){
	assert(frame);
	return frame->H;
}

/* Get the intensity value of the Y channel of pixel (x, y) */
/* in the YUV frame */
unsigned char GetPixelY(const YUVFrame *YUVframe, unsigned int x, unsigned int y)
{
	assert(YUVframe);

	assert(x < YUVframe->W);
	assert(y < YUVframe->H);

	assert(YUVframe->Y);
	assert(YUVframe->U);
	assert(YUVframe->V);

	return YUVframe->Y[x + y * YUVframe->W];
}

/* Get the intensity value of the U channel of pixel (x, y) */
/* in the YUV frame */
unsigned char GetPixelU(const YUVFrame *YUVframe, unsigned int x, unsigned int y)
{
	assert(YUVframe);

	assert(x < YUVframe->W);
	assert(y < YUVframe->H);

	assert(YUVframe->Y);
	assert(YUVframe->U);
	assert(YUVframe->V);

	return YUVframe->U[x + y * YUVframe->W];
}

/* Get the intensity value of the V channel of pixel (x, y) */
/* in the YUV frame */
unsigned char GetPixelV(const YUVFrame *YUVframe, unsigned int x, unsigned int y)
{
	assert(YUVframe);

	assert(x < YUVframe->W);
	assert(y < YUVframe->H);

	assert(YUVframe->Y);
	assert(YUVframe->U);
	assert(YUVframe->V);

	return YUVframe->V[x + y * YUVframe->W];
}

/* Set the intensity value of the Y channel of pixel (x, y) */
/* in the YUV frame with valueY */
void SetPixelY(YUVFrame *YUVframe, unsigned int x, unsigned int y,
               unsigned char valueY)
{
	assert(YUVframe);

	assert(x < YUVframe->W);
	assert(y < YUVframe->H);

	assert(YUVframe->Y);
	assert(YUVframe->U);
	assert(YUVframe->V);

	YUVframe->Y[x + y * YUVframe->W] = valueY;
}

/* Set the intensity value of the U channel of pixel (x, y) */
/* in the YUV frame with valueU */
void SetPixelU(YUVFrame *YUVframe, unsigned int x, unsigned int y,
               unsigned char valueU)
{
	assert(YUVframe);

	assert(x < YUVframe->W);
	assert(y < YUVframe->H);

	assert(YUVframe->Y);
	assert(YUVframe->U);
	assert(YUVframe->V);

	YUVframe->U[x + y * YUVframe->W] = valueU;
}

/* Set the intensity value of the V channel of pixel (x, y) */
/* in the YUV frame with valueV */
void SetPixelV(YUVFrame *YUVframe, unsigned int x, unsigned int y,
               unsigned char valueV)
{
	assert(YUVframe);

	assert(x < YUVframe->W);
	assert(y < YUVframe->H);

	assert(YUVframe->Y);
	assert(YUVframe->U);
	assert(YUVframe->V);

	YUVframe->V[x + y * YUVframe->W] = valueV;
}

/* Allocate the memory space for the YUV frame and the memory spaces */
/* for the YUV intensity values. Return the pointer to the YUV frame. */
YUVFrame *CreateYUVFrame(unsigned int width, unsigned int height)
{
	YUVFrame *YUVframe = (YUVFrame *)malloc(sizeof(YUVFrame));
	if (YUVframe == NULL) {
		return NULL;
	}

	YUVframe->W = width;
	YUVframe->H = height;

	YUVframe->Y = (unsigned char*)malloc(width * height * sizeof(unsigned char));
	if (YUVframe->Y == NULL) {
		free(YUVframe);
		return NULL;
	}

	YUVframe->U = (unsigned char*)malloc(width * height * sizeof(unsigned char));
	if (YUVframe->U == NULL) {
		free(YUVframe->Y);
		free(YUVframe);
		return NULL;
	}

	YUVframe->V = (unsigned char*)malloc(width * height * sizeof(unsigned char));
	if (YUVframe->V == NULL) {
		free(YUVframe->U);
		free(YUVframe->Y);
		free(YUVframe);
		return NULL;
	}

	return YUVframe;
}

/* Release the memory spaces for the YUV intensity values. */
/* Release the memory space for the YUV frame. */
void DeleteYUVFrame(YUVFrame *YUVframe)
{
	assert(YUVframe);
	assert(YUVframe->Y);
	assert(YUVframe->U);
	assert(YUVframe->V);

	free(YUVframe->Y);
	free(YUVframe->U);
	free(YUVframe->V);
	YUVframe->Y = NULL;
	YUVframe->U = NULL;
	YUVframe->V = NULL;

	free(YUVframe);
}

/* Copy RGB frame */
Frame *CopyRGBFrame(Frame *SrcFrame)
{

  assert(SrcFrame);
  assert(SrcFrame->R);
  assert(SrcFrame->G);
  assert(SrcFrame->B);

  unsigned int width  = SrcFrame->W;
  unsigned int height = SrcFrame->H;

  Frame* frame = CreateFrame(width, height);

  if (frame == NULL)
  {
    return NULL;
  }

  memcpy(frame->R, SrcFrame->R, width * height * sizeof(unsigned char));
  memcpy(frame->G, SrcFrame->G, width * height * sizeof(unsigned char));
  memcpy(frame->B, SrcFrame->B, width * height * sizeof(unsigned char));

  return frame;
}

/* EOF */
