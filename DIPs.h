/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                   */
/* DIPs.h: header file for DIP operations                            */
/*********************************************************************/

#ifndef DIPS_H
#define DIPS_H

#include "Frame.h"


/* Black and White */
Frame *BlackNWhite(Frame *frame);

/* Mirror frame vertical */
Frame *VMirror(Frame *frame);

/* Edge detection */
Frame *Edge(Frame *frame);

/* Apply ZotZot to a frame */
Frame *ZotZot(Frame *frame, Frame *anteater, int newW, int newH, int placeX, int placeY, const char *ZotColor);

/* Resize an frame */
Frame *Resize(Frame *frame, int newWidth, int newHeight);

/* Fill a color */
Frame *ColorFill(Frame *frame, int startX, int startY, const char *AnteaterColor);

/* Spotlight */
Frame *Spotlight(Frame *frame, int centerX, int centerY, unsigned int radius);

/* Rotate anf zoom an frame */
Frame *Rotate(Frame *frame, double Angle, double ScaleFactor);

#endif