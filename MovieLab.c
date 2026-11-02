#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Frame.h"
#include "FrameList.h"

Frame *CopyFrame(Frame *frame) {
    if (!frame) return NULL;
    Frame *newF = CreateFrame(frame->W, frame->H);
    for (unsigned int y = 0; y < frame->H; y++) {
        for (unsigned int x = 0; x < frame->W; x++) {
            SetPixelR(newF, x, y, GetPixelR(frame, x, y));
            SetPixelG(newF, x, y, GetPixelG(frame, x, y));
            SetPixelB(newF, x, y, GetPixelB(frame, x, y));
        }
    }
    return newF;
}

/* Function Prototypes */
YUVFrame* LoadOneFrame(const char* fname, int n, unsigned int width, unsigned int height);
Movie *LoadMovie(const char *fname, int frameNum, unsigned int width, unsigned int height);
void SaveOneFrame(YUVFrame *frame, const char *fname, FILE *file);
int SaveMovie(const char *fname, Movie *movie);
void PrintUsage();

int main(int argc, char *argv[])
{
    int x = 1;
    char *fin = NULL, *fout = NULL;
    int frameNum = 0;
    unsigned int width = 0, height = 0;
    /* Operation Flags and Parameters */
    int doBW = 0, doVMirror = 0, doEdge = 0, doReverse = 0, doSpotlight = 0, doRotate = 0;
    int doCrop = 0, doFast = 0, doZotZot = 0;
    unsigned int cropStart = 0, cropEnd = 0, fastFactor = 0;
    char *zotzotFile = NULL, *zotzotColor = NULL;

    /* 1. Parse Command Line Arguments */
    while (x < argc) {
        if (strcmp(argv[x], "-i") == 0) {
            if (x < argc - 1) fin = argv[++x];
            else { printf("Missing argument for -i\n"); return 5; }
        } else if (strcmp(argv[x], "-o") == 0) {
            if (x < argc - 1) fout = argv[++x];
            else { printf("Missing argument for -o\n"); return 5; }
        } else if (strcmp(argv[x], "-f") == 0) {
            if (x < argc - 1) frameNum = atoi(argv[++x]);
            else { printf("Missing argument for -f\n"); return 5; }
        } else if (strcmp(argv[x], "-s") == 0) {
            if (x < argc - 1) {
                if (sscanf(argv[++x], "%ux%u", &width, &height) != 2) {
                    printf("Error: Invalid resolution format for -s.\n"); return 5;
                }
            } else { printf("Missing argument for -s\n"); return 5; }
        } else if (strcmp(argv[x], "-bw") == 0) { doBW = 1; }
        else if (strcmp(argv[x], "-vmirror") == 0) { doVMirror = 1; }
        else if (strcmp(argv[x], "-edge") == 0) { doEdge = 1; }
        else if (strcmp(argv[x], "-reverse") == 0) { doReverse = 1; }
        else if (strcmp(argv[x], "-spotlight") == 0) { doSpotlight = 1; }
        else if (strcmp(argv[x], "-rotate") == 0) { doRotate = 1; }
        else if (strcmp(argv[x], "-crop") == 0) {
            if (x < argc - 1) {
                if (sscanf(argv[++x], "%u-%u", &cropStart, &cropEnd) == 2) doCrop = 1;
                else { printf("Error: Invalid format for -crop.\n"); return 5; }
            } else { printf("Missing argument for -crop\n"); return 5; }
        } else if (strcmp(argv[x], "-fast") == 0) {
            if (x < argc - 1) { fastFactor = atoi(argv[++x]); doFast = 1; }
            else { printf("Missing factor for -fast\n"); return 5; }
        } else if (strcmp(argv[x], "-zotzot") == 0) {
            if (x < argc - 2) { zotzotFile = argv[++x]; zotzotColor = argv[++x]; doZotZot = 1; }
            else { printf("Missing arguments for -zotzot\n"); return 5; }
        } else if (strcmp(argv[x], "-h") == 0) {
            PrintUsage(); return 0;
        }
        x++;
    }

    /* 2. Validate Mandatory Arguments */
    if (!fin || !fout || frameNum <= 0 || width == 0 || height == 0) {
        printf("Error: Missing or invalid mandatory arguments (-i, -o, -f, -s).\n");
        PrintUsage();
        return 5;
    }

    /* 3. Load Movie */
    Movie *movie = LoadMovie(fin, frameNum, width, height);
    if (!movie) return 10;

    /* Conversion required before DIP operations */
    YUV2RGBMovie(movie);

    /* 4. PERFORM OPERATIONS IN MANDATORY ORDER (Per Section 1.3.4) */
    if (doBW) {
        FENTRY *curr = movie->Frames->First;
        while (curr) { BlackNWhite(curr->RGBFrame); curr = curr->Next; }
        printf("Operation Black and White is done!\n");
    }
    if (doVMirror) {
        FENTRY *curr = movie->Frames->First;
        while (curr) { VMirror(curr->RGBFrame); curr = curr->Next; }
        printf("Operation VMirror is done!\n");
    }
    if (doEdge) {
        FENTRY *curr = movie->Frames->First;
        while (curr) { Edge(curr->RGBFrame); curr = curr->Next; }
        printf("Operation Edge is done!\n");
    }
    if (doCrop) {
        CropFrameList(movie->Frames, cropStart, cropEnd);
        printf("Operation Crop is done! New number of frames is %u.\n", movie->Frames->Length);
    }
    if (doFast) {
        FastFrameList(movie->Frames, fastFactor);
        printf("Operation Fast Forward is done! New number of frames is %u.\n", movie->Frames->Length);
    }
    if (doReverse) {
        ReverseFrameList(movie->Frames);
        printf("Operation Reverse is done!\n");
    }
    if (doZotZot) {
        Frame *anteater = LoadFrame(zotzotFile);
        if (anteater) {
            unsigned int count = 0;
            FENTRY *curr = movie->Frames->First;
            while (curr) {
                int corner = (count / ZOTZOT_PER_FRAMES) % 4;
                int posX = (corner == 1 || corner == 2) ? (width - (48 * ZOTZOT_RATIO)) : 0;
                int posY = (corner == 2 || corner == 3) ? (height - (27 * ZOTZOT_RATIO)) : 0;
                ZotZot(curr->RGBFrame, anteater, 48 * ZOTZOT_RATIO, 27 * ZOTZOT_RATIO, posX, posY, zotzotColor);
                curr = curr->Next; count++;
            }
            DeleteFrame(anteater);
            printf("Operation ZotZot is done!\n");
        } else {
            printf("Error: Could not load ZotZot image %s\n", zotzotFile);
        }
    }
    if (doSpotlight) {
        double maxR = sqrt((width/2.0)*(width/2.0) + (height/2.0)*(height/2.0));
        Frame *first = movie->Frames->First->RGBFrame;
        for (int i = 0; i < 20; i++) {
            Frame *copy = Spotlight(CopyFrame(first), width/2, height/2, (unsigned int)(i * maxR / 20));
            PrependRGBFrame(movie->Frames, copy);
        }
        Frame *last = movie->Frames->Last->RGBFrame;
        for (int i = 0; i < 20; i++) {
            Frame *copy = Spotlight(CopyFrame(last), width/2, height/2, (unsigned int)((19-i) * maxR / 20));
            AppendRGBFrame(movie->Frames, copy);
        }
        printf("Operation Spotlight is done!\n");
    }
    if (doRotate) {
	    /* 1. Get references to the original first and last frames */
        Frame *first = movie->Frames->First->RGBFrame;
        Frame *last = movie->Frames->Last->RGBFrame;

        /* 2. Process First 20 Frames (Clockwise Rotation & Zoom In) */
        /* We loop backwards (19 down to 0) because PrependRGBFrame pushes frames
           to the front. Prepending the smallest frame (scale 0.20, angle 20) LAST
           ensures it becomes the very first frame of the modified movie. */
        for (int i = 19; i >= 0; i--) {
            double scale = 0.20 + (i * 0.04); /* Scales from 0.20 up to 0.96 */
            double angle = 20.0 - (i * 1.0);  /* Angles from 20.0 down to 1.0 */

            Frame *copy = CopyFrame(first);
            copy = Rotate(copy, angle, scale);
            PrependRGBFrame(movie->Frames, copy);
        }

        /* 3. Process Last 20 Frames (Counter-Clockwise Rotation & Zoom Out) */
        /* AppendRGBFrame adds to the tail of the list, so we use a normal forward loop. */
        for (int i = 0; i < 20; i++) {
            double scale = 0.96 - (i * 0.04);  /* Scales from 0.96 down to 0.20 */
            double angle = -1.0 - (i * 1.0);   /* Angles from -1.0 down to -20.0 */

            Frame *copy = CopyFrame(last);
            copy = Rotate(copy, angle, scale);
            AppendRGBFrame(movie->Frames, copy);
        }

        printf("Operation Rotate is done!\n");
    }

    /* 5. Save and Clean up */
    RGB2YUVMovie(movie);
    SaveMovie(fout, movie);
    DeleteMovie(movie);

    return 0;
}

void PrintUsage() {
    printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> -s <WidthxHeight> [options]\n"
           "Options:\n"
           "-bw                Activate the black and white filter\n"
           "-vmirror           Activate vertical mirror\n"
           "-edge              Activate the edge filter\n"
           "-crop <start-end> Crop movie frames\n"
           "-fast <factor>    Fast forward movie\n"
           "-reverse          Reverse frame order\n"
           "-zotzot <f> <c>    Apply ZotZot image <f> with color <c>\n"
           "-spotlight        Fade in/out copies of first/last frames\n"
           "-rotate           Rotate/zoom first/last frames (Bonus)\n"
           "-h                Display usage information\n");
}

Movie *LoadMovie(const char *fname, int frameNum, unsigned int width, unsigned int height) {
    Movie *movie = CreateMovie();
    if (!movie) return NULL;
    for (int i = 0; i < frameNum; i++) {
        YUVFrame *frame = LoadOneFrame(fname, i, width, height);
        if (frame) AppendYUVFrame(movie->Frames, frame);
    }
    printf("The movie file %s has been read successfully!\n", fname);
    return movie;
}

YUVFrame* LoadOneFrame(const char* fname, int n, unsigned int width, unsigned int height) {
    FILE *file = fopen(fname, "r");
    if (!file) return NULL;
    YUVFrame* YUVframe = CreateYUVFrame(width, height);
    if (!YUVframe) { fclose(file); return NULL; }

    fseek(file, (long)(1.5 * n * width * height), SEEK_SET);
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) SetPixelY(YUVframe, x, y, fgetc(file));
    }
    for (unsigned int y = 0; y < height; y += 2) {
        for (unsigned int x = 0; x < width; x += 2) {
            unsigned char c = fgetc(file);
            SetPixelU(YUVframe, x, y, c); SetPixelU(YUVframe, x+1, y, c);
            SetPixelU(YUVframe, x, y+1, c); SetPixelU(YUVframe, x+1, y+1, c);
        }
    }
    for (unsigned int y = 0; y < height; y += 2) {
        for (unsigned int x = 0; x < width; x += 2) {
            unsigned char c = fgetc(file);
            SetPixelV(YUVframe, x, y, c); SetPixelV(YUVframe, x+1, y, c);
            SetPixelV(YUVframe, x, y+1, c); SetPixelV(YUVframe, x+1, y+1, c);
        }
    }
    fclose(file);
    return YUVframe;
}

int SaveMovie(const char *fname, Movie *movie) {
    FILE *file = fopen(fname, "w");
    if (!file) return 1;
    int count = 0;
    FENTRY *curr = movie->Frames->First;
    while (curr) {
        SaveOneFrame(curr->YUVFrame, fname, file);
        curr = curr->Next; count++;
    }
    fclose(file);
    printf("The movie file %s has been written successfully!\n", fname);
    printf("%d frames are written to the file %s in total.\n", count, fname);
    return 0;
}

void SaveOneFrame(YUVFrame *frame, const char *fname, FILE *file) {
    for (unsigned int y = 0; y < frame->H; y++) {
        for (unsigned int x = 0; x < frame->W; x++) fputc(GetPixelY(frame, x, y), file);
    }
    for (unsigned int y = 0; y < frame->H; y += 2) {
        for (unsigned int x = 0; x < frame->W; x += 2) fputc(GetPixelU(frame, x, y), file);
    }
    for (unsigned int y = 0; y < frame->H; y += 2) {
        for (unsigned int x = 0; x < frame->W; x += 2) fputc(GetPixelV(frame, x, y), file);
    }
}
