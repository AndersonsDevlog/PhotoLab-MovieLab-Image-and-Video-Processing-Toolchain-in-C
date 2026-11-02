DEBUG  = -g
CFLAGS = -std=c11 -Wall -c
LFLAGS = -std=c11 -Wall
LIBS   = -lm

# Default target
all: MovieLab


# Target for MovieLab.o
MovieLab.o: MovieLab.c FileIO.h DIPs.h Movie.h Constants.h Frame.h FrameList.h
	$(CC) $(CFLAGS) MovieLab.c -o MovieLab.o

# Target for FileIO.o
FileIO.o: FileIO.c FileIO.h Frame.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

# Target for DIPs.o
DIPs.o: DIPs.c DIPs.h Frame.h
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o

# Target for Frame.o
Frame.o: Frame.c Frame.h
	$(CC) $(CFLAGS) Frame.c -o Frame.o

# Target for FrameList.o
FrameList.o: FrameList.c FrameList.h Frame.h
	$(CC) $(CFLAGS) FrameList.c -o FrameList.o

# Target for Movie.o
Movie.o: Movie.c Movie.h FrameList.h
	$(CC) $(CFLAGS) Movie.c -o Movie.o


# Target for MovieLab
MovieLab: MovieLab.o Movie.o FrameList.o Frame.o DIPs.o FileIO.o
	$(CC) $(LFLAGS) MovieLab.o Movie.o FrameList.o Frame.o DIPs.o FileIO.o $(LIBS) -o MovieLab


clean:
	rm -f *.o
	rm -f MovieLab
	rm -f out.yuv
