/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                    */
/* Movie.c: source file for basic movie manipulations                */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "Movie.h"
#include "FrameList.h"
#include "Frame.h"

/* Helper function for clipping values to [0, 255] */
static int clip(int x) {
    if (x < 0) return 0;
    if (x > 255) return 255;
    return x;
}

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void)
{
    Movie *movie = (Movie *)malloc(sizeof(Movie));
    if (!movie) {
        return NULL;
    }

    /* Allocate the frame list inside the movie */
    movie->Frames = CreateFrameList();
    if (!movie->Frames) {
        free(movie);
        return NULL;
    }

    return movie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie)
{
    if (movie) {
        if (movie->Frames) {
            DeleteFrameList(movie->Frames);
        }
        free(movie);
    }
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(Movie *movie)
{
    assert(movie);
    assert(movie->Frames);

    FENTRY *curr = movie->Frames->First;
    while (curr != NULL) {
        if (curr->YUVFrame != NULL && curr->RGBFrame == NULL) {
            unsigned int width = curr->YUVFrame->W;
            unsigned int height = curr->YUVFrame->H;

            Frame *newRGB = CreateFrame(width, height);
            assert(newRGB);

            for (unsigned int y = 0; y < height; y++) {
                for (unsigned int x = 0; x < width; x++) {
                    int Y = GetPixelY(curr->YUVFrame, x, y);
                    int U = GetPixelU(curr->YUVFrame, x, y);
                    int V = GetPixelV(curr->YUVFrame, x, y);

                    int C = Y - 16;
                    int D = U - 128;
                    int E = V - 128;

                    int R = clip((298 * C + 409 * E + 128) >> 8);
                    int G = clip((298 * C - 100 * D - 208 * E + 128) >> 8);
                    int B = clip((298 * C + 516 * D + 128) >> 8);

                    SetPixelR(newRGB, x, y, (unsigned char)R);
                    SetPixelG(newRGB, x, y, (unsigned char)G);
                    SetPixelB(newRGB, x, y, (unsigned char)B);
                }
            }

            DeleteYUVFrame(curr->YUVFrame);
            curr->YUVFrame = NULL;
            curr->RGBFrame = newRGB;
        }
        curr = curr->Next;
    }
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(Movie *movie)
{
    assert(movie);
    assert(movie->Frames);
    FENTRY *curr = movie->Frames->First;
    while (curr != NULL) {
        if (curr->RGBFrame != NULL && curr->YUVFrame == NULL) {
            unsigned int width = curr->RGBFrame->W;
            unsigned int height = curr->RGBFrame->H;

            YUVFrame *newYUV = CreateYUVFrame(width, height);
            assert(newYUV);

            for (unsigned int y = 0; y < height; y++) {
                for (unsigned int x = 0; x < width; x++) {
                    int R = GetPixelR(curr->RGBFrame, x, y);
                    int G = GetPixelG(curr->RGBFrame, x, y);
                    int B = GetPixelB(curr->RGBFrame, x, y);

                    int Y = clip(((66 * R + 129 * G + 25 * B + 128) >> 8) + 16);
                    int U = clip(((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128);
                    int V = clip(((112 * R - 94 * G - 18 * B + 128) >> 8) + 128);

                    SetPixelY(newYUV, x, y, (unsigned char)Y);
                    SetPixelU(newYUV, x, y, (unsigned char)U);
                    SetPixelV(newYUV, x, y, (unsigned char)V);
                }
            }

            DeleteFrame(curr->RGBFrame);
            curr->RGBFrame = NULL;
            curr->YUVFrame = newYUV;
        }
        curr = curr->Next;
    }
}
