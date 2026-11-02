/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                   */
/* DIPs.c: source file for DIP operations                            */
/*********************************************************************/
#include <stdlib.h>
#include <assert.h>
#include "DIPs.h"
#include "Constants.h"
#include "string.h"
#include "math.h"
#include <stdio.h>

/* Black and White */
Frame *BlackNWhite(Frame *frame) {
	assert(frame);
	int x, y, tmp;

	for (y = 0; y < FrameHeight(frame); y++) {
		for (x = 0; x < FrameWidth(frame); x++) {
		tmp = (GetPixelR(frame, x, y) + GetPixelG(frame, x, y) + GetPixelB(frame, x, y)) / 3;
		SetPixelR(frame, x, y, tmp);
		SetPixelG(frame, x, y, tmp);
		SetPixelB(frame, x, y, tmp);
		}
	}
	return frame;
}

/* Mirror frame vertical */
Frame *VMirror(Frame *frame) {
	assert(frame);
	int x, y;
    for (y = 0; y < FrameHeight(frame)/2; y++) {
        for (x = 0; x < FrameWidth(frame); x++) {
			SetPixelG(frame, x, FrameHeight(frame) - 1 - y, GetPixelG(frame, x, y));
			SetPixelB(frame, x, FrameHeight(frame) - 1 - y, GetPixelB(frame, x, y));
            SetPixelR(frame, x, FrameHeight(frame) - 1 - y, GetPixelR(frame, x, y));
		}
	}
    return frame;
}

/* Edge detection */
Frame *Edge(Frame *frame) {
	assert(frame);
	Frame *tmpFrame = CreateFrame(FrameWidth(frame), FrameHeight(frame));
	assert(tmpFrame);

	int x, y, m, n;
	memcpy(tmpFrame->R, frame->R, FrameWidth(frame) * FrameHeight(frame) * sizeof(unsigned char));
	memcpy(tmpFrame->G, frame->G, FrameWidth(frame) * FrameHeight(frame) * sizeof(unsigned char));
	memcpy(tmpFrame->B, frame->B, FrameWidth(frame) * FrameHeight(frame) * sizeof(unsigned char));
	for (y = 1; y < FrameHeight(frame) - 1; y++) {
		for (x = 1; x < FrameWidth(frame) - 1; x++) {
			int tmpR = 0;
			int tmpG = 0;
			int tmpB = 0;
			for (m = -1; m <= 1; m++) {
				for (n = -1; n <= 1; n++) {
					tmpR += (GetPixelR(tmpFrame, x, y) - GetPixelR(tmpFrame, x + n, y + m));
					tmpG += (GetPixelG(tmpFrame, x, y) - GetPixelG(tmpFrame, x + n, y + m));
					tmpB += (GetPixelB(tmpFrame, x, y) - GetPixelB(tmpFrame, x + n, y + m));
				}
			}
			SetPixelR(frame, x, y, (tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR);
			SetPixelG(frame, x, y, (tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG);
			SetPixelB(frame, x, y, (tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB);
		}
	}
	for (y = 0; y < FrameHeight(frame); y++) {
		x = 0;
		SetPixelR(frame, x, y, 0);
		SetPixelG(frame, x, y, 0);
		SetPixelB(frame, x, y, 0);
		x = FrameWidth(frame) - 1;
		SetPixelR(frame, x, y, 0);
		SetPixelG(frame, x, y, 0);
		SetPixelB(frame, x, y, 0);
	}
	for (x = 0; x < FrameWidth(frame); x++) {
		y = 0;
		SetPixelR(frame, x, y, 0);
		SetPixelG(frame, x, y, 0);
		SetPixelB(frame, x, y, 0);
		y = FrameHeight(frame) - 1;
		SetPixelR(frame, x, y, 0);
		SetPixelG(frame, x, y, 0);
		SetPixelB(frame, x, y, 0);
	}
	DeleteFrame(tmpFrame);
	tmpFrame = NULL;
	return frame;
}

Frame *Resize(Frame *frame, int newWidth, int newHeight) {
	assert(frame);
	double ScaleX, ScaleY;
	Frame *scaledFrame = CreateFrame(newWidth, newHeight);
	assert(scaledFrame);

    int x, y;
    int x2, y2;
    double M[2][2];

	ScaleX = (double)newWidth/FrameWidth(frame);
    ScaleY = (double)newHeight/FrameHeight(frame);
    M[0][0] = 1/ScaleX;
    M[0][1] = 0;
    M[1][0] = 0;
    M[1][1] = 1/ScaleY;

    for(y=0; y<newHeight; y++)
    {   for(x=0; x<newWidth; x++)
        {
            x2 = (x)*M[0][0] + (y)*M[0][1] ;
            y2 = (x)*M[1][0] + (y)*M[1][1] ;
			SetPixelR(scaledFrame, x, y, GetPixelR(frame,x2,y2));
			SetPixelG(scaledFrame, x, y, GetPixelG(frame,x2,y2));
			SetPixelB(scaledFrame, x, y, GetPixelB(frame,x2,y2));
        }
    }

	DeleteFrame(frame);
	return scaledFrame;
}

Frame *ColorFill(Frame *frame, int startX, int startY,
                 const char *AnteaterColor) {
    assert(frame);
	int fillR = 255;
    int fillG = 255;
    int fillB = 255;
	if (!strcmp(AnteaterColor, "red")) {
        fillR = 255;
        fillG = 0;
        fillB = 0;
    } else if (!strcmp(AnteaterColor, "green")) {
        fillR = 0;
        fillG = 255;
        fillB = 0;
    } else if (!strcmp(AnteaterColor, "blue")) {
        fillR = 0;
        fillG = 0;
        fillB = 255;
    } else if (!strcmp(AnteaterColor, "yellow")) {
        fillR = 255;
        fillG = 255;
        fillB = 0;
    } else if (!strcmp(AnteaterColor, "cyan")) {
        fillR = 0;
        fillG = 255;
        fillB = 255;
    } else if (!strcmp(AnteaterColor, "pink")) {
        fillR = 255;
        fillG = 192;
        fillB = 203;
    } else if (!strcmp(AnteaterColor, "orange")) {
        fillR = 255;
        fillG = 165;
        fillB = 0;
    } else {
        printf("Unsurported color.\n");
        return frame;
    }

    const int W = FrameWidth(frame);
    const int H = FrameHeight(frame);

    if (startX < 0 || startX >= W || startY < 0 || startY >= H) return frame;

    /* boundary is "black": anything not pure white blocks */
    if (GetPixelR(frame, startX, startY) != 255 ||
        GetPixelG(frame, startX, startY) != 255 ||
        GetPixelB(frame, startX, startY) != 255) {
        return frame;
    }

    /* already fill color */
    if (GetPixelR(frame, startX, startY) == fillR &&
        GetPixelG(frame, startX, startY) == fillG &&
        GetPixelB(frame, startX, startY) == fillB) {
        return frame;
    }

    /* Fixed-size stack */
    int stackX[W * H];
    int stackY[W * H];
    int top = 0;

    /* push start */
    stackX[top] = startX;
    stackY[top] = startY;
    top++;

    /* mark to avoid pushing the same pixel again */
    SetPixelR(frame, startX, startY, fillR);
    SetPixelG(frame, startX, startY, fillG);
    SetPixelB(frame, startX, startY, fillB);

    static const int dx[4] = { 1, -1, 0, 0 };
    static const int dy[4] = { 0, 0, 1, -1 };

    while (top > 0) {
        top--;
        int x = stackX[top];
        int y = stackY[top];

        for (int k = 0; k < 4; k++) {
            int nx = x + dx[k];
            int ny = y + dy[k];

            if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;

            /* Only expand into pure-white pixels */
            if (GetPixelR(frame, nx, ny) == 255 &&
                GetPixelG(frame, nx, ny) == 255 &&
                GetPixelB(frame, nx, ny) == 255) {

                /* fill immediately (so it won't be pushed twice) */
                SetPixelR(frame, nx, ny, fillR);
                SetPixelG(frame, nx, ny, fillG);
                SetPixelB(frame, nx, ny, fillB);

                /* push */
                stackX[top] = nx;
                stackY[top] = ny;
                top++;
            }
        }
    }

    return frame;
}

Frame *ZotZot(Frame *frame, Frame *anteater,
                    int newW, int newH,
                    int placeX, int placeY,
                    const char *ZotColor) {
    assert(frame);
    assert(anteater);

    int x, y;
    int Wd = FrameWidth(frame);
    int Hd = FrameHeight(frame);

    Frame *temp_anteater = CreateFrame(FrameWidth(anteater), FrameHeight(anteater));
    memcpy(temp_anteater->R, anteater->R, FrameWidth(anteater) * FrameHeight(anteater) * sizeof(unsigned char));
    memcpy(temp_anteater->G, anteater->G, FrameWidth(anteater) * FrameHeight(anteater) * sizeof(unsigned char));
    memcpy(temp_anteater->B, anteater->B, FrameWidth(anteater) * FrameHeight(anteater) * sizeof(unsigned char));


    /* 1) ColorFill Anteater */
    temp_anteater = ColorFill(temp_anteater, 96, 54, ZotColor);

    /* 2) Resize Anteater */
    temp_anteater = Resize(temp_anteater, newW, newH);

    int Ws = FrameWidth(temp_anteater);
    int Hs = FrameHeight(temp_anteater);

    /* 3) Paste Anteater onto frame (white is transparent) */
    for (y = 0; y < Hs; y++) {
        for (x = 0; x < Ws; x++) {

            int X = placeX + x;
            int Y = placeY + y;

            if (X < 0 || X >= Wd || Y < 0 || Y >= Hd)
                continue;

            unsigned char r = GetPixelR(temp_anteater, x, y);
            unsigned char g = GetPixelG(temp_anteater, x, y);
            unsigned char b = GetPixelB(temp_anteater, x, y);

            /* treat white as transparent */
            if (r == 255 && g == 255 && b == 255)
                continue;

            SetPixelR(frame, X, Y, r);
            SetPixelG(frame, X, Y, g);
            SetPixelB(frame, X, Y, b);
        }
    }

    DeleteFrame(temp_anteater);
    return frame;
}
/* Spotlight */
Frame *Spotlight(Frame *frame, int centerX, int centerY, unsigned int radius)
{
	assert(frame);
    int x, y;
    unsigned int width = FrameWidth(frame);
    unsigned int height = FrameHeight(frame);
    long long r_squared = (long long)radius * radius;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            /* Calculate squared distance from center to avoid slow sqrt() calls */
            long long dx = x - centerX;
            long long dy = y - centerY;
            long long distance_squared = (dx * dx) + (dy * dy);

            /* If the pixel is outside the circle, make it black */
            if (distance_squared > r_squared) {
                SetPixelR(frame, x, y, 0);
                SetPixelG(frame, x, y, 0);
                SetPixelB(frame, x, y, 0);
            }
            /* Else: pixel is inside, keep original values as per instructions */
        }
    }
    return frame;
}

/* Rotate and zoom an frame */
Frame *Rotate(Frame *frame, double Angle, double ScaleFactor)
{
	assert(frame);
    unsigned int W = FrameWidth(frame);
    unsigned int H = FrameHeight(frame);
    Frame *tempFrame = CreateFrame(W, H);
    assert(tempFrame);

    double rad = Angle * 3.1415926535897932384626433832795028841971  / 180.0; /* Convert degrees to radians */
    double cosA = cos(-rad);
    double sinA = sin(-rad);

    int midX = W / 2;
    int midY = H / 2;

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            /* Shift coordinates so center is (0,0) */
            int x_shift = x - midX;
            int y_shift = y - midY;

            /* Rotate and scale */
            int x2 = (int)((x_shift * cosA - y_shift * sinA) / ScaleFactor) + midX;
            int y2 = (int)((x_shift * sinA + y_shift * cosA) / ScaleFactor) + midY;

            if (x2 >= 0 && x2 < W && y2 >= 0 && y2 < H) {
                SetPixelR(tempFrame, x, y, GetPixelR(frame, x2, y2));
                SetPixelG(tempFrame, x, y, GetPixelG(frame, x2, y2));
                SetPixelB(tempFrame, x, y, GetPixelB(frame, x2, y2));
            } else {
                /* Fill remaining pixels with black */
                SetPixelR(tempFrame, x, y, 0);
                SetPixelG(tempFrame, x, y, 0);
                SetPixelB(tempFrame, x, y, 0);
            }
        }
    }

    /* Replace the original frame content with the rotated content */
    memcpy(frame->R, tempFrame->R, W * H * sizeof(unsigned char));
    memcpy(frame->G, tempFrame->G, W * H * sizeof(unsigned char));
    memcpy(frame->B, tempFrame->B, W * H * sizeof(unsigned char));

    DeleteFrame(tempFrame);
    return frame;
}
/* EOF */
