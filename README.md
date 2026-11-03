# PhotoLab & MovieLab

An image and video processing toolchain written in C. **PhotoLab** is a menu-driven tool for editing PPM images. **MovieLab** is a command-line tool that applies filters and edits to 120-frame YUV video. No external libraries.

- **16 image operations** (filters, geometric transforms, flood fill, compositing)
- **8+ video operations** on frames stored in a doubly-linked list
- Dynamic memory with bounds-checked pixel accessors, tested clean under Valgrind
- Integer-only math for color conversion and filtering (no floating point in the video path)

---

## Build

Requires `gcc`, `make`, and (optionally) `valgrind`. Compiled with `-std=c11 -Wall`.

```bash
# PhotoLab
cd PhotoLab
make            # builds PhotoLab (interactive) and PhotoLabTest (auto test)
make clean

# MovieLab
cd MovieLab
make            # builds MovieLab
make clean
```

| Target | Output |
|---|---|
| `PhotoLab` | Interactive menu program (DEBUG off) |
| `PhotoLabTest` | Runs every operation automatically (DEBUG on, built with `-DDEBUG`) |
| `MovieLab` | Command-line video tool |

The Makefile packages the filters into a static library (`libfilter.a`) and builds both PhotoLab binaries from the same source.

---

## PhotoLab

Works on **PPM (P6)** images of any size. Enter file names **without** the `.ppm` extension.

```bash
./PhotoLab
```

### Menu

| # | Operation | Prompts / behavior |
|---|---|---|
| 1 | Load a PPM image | File name |
| 2 | Save an image | File name |
| 3 | Black & white | Sets R, G, B to `(R+G+B)/3` |
| 4 | Negative | Each channel becomes `255 - value` |
| 5 | Color filter | Target R, G, B, threshold, replacement R, G, B. Pixels within ±threshold of the target on all channels are replaced |
| 6 | Edge detection | 3×3 kernel (`8` center, `-1` neighbors) with saturated arithmetic. Border pixels are skipped |
| 7 | Shuffle | Splits the image into 4×4 blocks and swaps block *i* with block *17 − i* |
| 8 | Flip horizontally | Reverses each row |
| 9 | Mirror vertically | Copies the top half, reversed, onto the bottom half |
| 10 | Add border | Border width, color: `black white red green blue yellow cyan pink orange` |
| 11 | Fisheye | Distortion factor, scaling factor (example: `0.6`, `1.9`). Out-of-range source pixels are black |
| 12 | Posterize | Number of bits (1 to 8) for R, G, B. The *n*th lowest bit is set to 0 and the lower *n−1* bits to 1 |
| 13 | Rotate and zoom | Angle (degrees), scale, center X, center Y |
| 14 | Motion blur | Blur amount. Each pixel is half itself plus half the average of the pixels below it |
| 15 | Crop | X offset, Y offset, width, height. Clamped to the image bounds |
| 16 | Resize | New width, new height (enlarge or shrink) |
| 17 | Fill color | Seed X, seed Y, color: `red green blue yellow cyan pink orange` |
| 18 | ZotZot | Overlay file, new width, new height, place X, place Y, color |
| 19 | Test all functions | Runs `AutoTest()` |
| 20 | Exit | |

Error handling:
- Options 2 to 18 with no image loaded print `No image to process!`
- Any other input prints `Invalid selection!`

### Example session

```
Please make your choice: 1
Please input the file name to load: EngHall
Please make your choice: 11
Enter a value for distortion factor: 0.6
Enter a value for scaling factor: 1.9
"FishEye" operation is done!
Please make your choice: 2
Please input the file name to save: fisheye
Please make your choice: 20
You exit the program.
```

### Notes on selected operations

- **Fill color (17)** is a 4-connected flood fill (up, down, left, right). It is designed for a binary black-and-white image: black `(0,0,0)` pixels are boundaries, and only white `(255,255,255)` pixels connected to the seed are recolored.
- **ZotZot (18)** recolors the overlay with the flood fill from seed `(96, 54)`, resizes it, then composites it onto the loaded image at `(placeX, placeY)`. White pixels in the overlay are treated as transparent.
- **Resize (16)** and **Rotate (13)** map each *target* pixel back to a source pixel, so enlarging leaves no holes.

### Automated test

```bash
./PhotoLabTest
```

Loads the test image, applies each operation with fixed parameters, and saves one output per operation.

---

## MovieLab

Operates on raw **YUV** video. Frames are converted YUV → RGB after loading and RGB → YUV before saving. The default test stream is 120 frames at 640×360, but other sizes work.

```bash
./MovieLab -i <input.yuv> -o <output.yuv> -f <frames> -s <WxH> [options]
```

### Required options

| Option | Meaning |
|---|---|
| `-i <file>` | Input movie file |
| `-o <file>` | Output movie file |
| `-f <n>` | Number of frames to read |
| `-s <WxH>` | Frame resolution, e.g. `640x360` |

A missing required option or a missing option argument prints an error plus the usage text, then exits.

### Operations

| Option | Effect | Result for a 120-frame input |
|---|---|---|
| `-bw` | Black and white on every frame | 120 frames |
| `-vmirror` | Vertical mirror on every frame | 120 frames |
| `-edge` | Edge detection on every frame | 120 frames |
| `-crop <start-end>` | Keep frames *start* to *end* (0-based, inclusive) | `-crop 20-40` gives 21 frames |
| `-fast <factor>` | Keep every *factor*-th frame (factor ≥ 1) | `-fast 3` gives 40 frames |
| `-reverse` | Reverse the frame order | 120 frames |
| `-zotzot <ppm> <color>` | Overlay a recolored, 48×27-scaled image that moves through the four frame corners | 120 frames |
| `-spotlight` | Duplicates the first and last frames 20 times each, then fades in and out with a growing or shrinking circle | 160 frames |
| `-rotate` | Duplicates the first and last frames 20 times each, then rotates and zooms the copies | 160 frames |
| `-h` | Print usage | |

Multiple options can be combined in one run. They always execute in this order:

`-bw` → `-vmirror` → `-edge` → `-crop` → `-fast` → `-reverse` → `-zotzot` → `-spotlight` → `-rotate`

### Examples

```bash
# Black and white
./MovieLab -i EngHall_640_360.yuv -o out_640_360.yuv -f 120 -s 640x360 -bw

# Frames 20 to 40, then reversed
./MovieLab -i EngHall_640_360.yuv -o out_640_360.yuv -f 120 -s 640x360 -crop 20-40 -reverse

# Overlay
./MovieLab -i EngHall_640_360.yuv -o out_640_360.yuv -f 120 -s 640x360 -zotzot Anteater.ppm yellow
```

Sample output:

```
The movie file EngHall_640_360.yuv has been read successfully!
Operation Fast Forward is done! New number of frames is 40.
The movie file out_640_360.yuv has been written successfully!
40 frames are written to the file out_640_360.yuv in total.
```

---

## Design

| Module | Responsibility |
|---|---|
| `PhotoLab.c` | Menu loop and input handling |
| `FileIO.c/.h` | PPM and YUV file loading and saving |
| `Image.c/.h` | `Image` struct, allocation, pixel get/set with assertions |
| `DIPs.c/.h` | Basic operations (B&W, negative, color filter, edge, shuffle, flips, border) |
| `Advanced.c/.h` | Fisheye, posterize, rotate, blur, crop, resize, fill, ZotZot |
| `Test.c/.h` | `AutoTest()` |
| `Frame.c/.h` | RGB and YUV frame structs and pixel accessors |
| `FrameList.c/.h` | Doubly-linked list of frames |
| `Movie.c/.h` | `Movie` struct, YUV ↔ RGB conversion |
| `MovieLab.c` | Argument parsing and the operation pipeline |

**Key decisions**

- **1D pixel storage.** Each channel is one heap buffer indexed as `x + y * Width`, so any resolution works. All access goes through get/set functions that assert valid pointers and coordinates.
- **Doubly-linked frame list.** Crop, fast-forward, and reverse relink entries in place instead of copying frames.
- **Fixed-point color conversion.** Integer math with bit shifts and saturating clip:
  ```
  Y = clip((( 66*R + 129*G +  25*B + 128) >> 8) +  16)
  U = clip(((-38*R -  74*G + 112*B + 128) >> 8) + 128)
  V = clip((( 112*R -  94*G -  18*B + 128) >> 8) + 128)
  ```
- **Saturated arithmetic.** Filters compute in `int` and clamp to 0..255 before storing as `unsigned char`.
- **Bitwise posterize.** Per-channel masks instead of arithmetic.

## Memory checking

```bash
valgrind --leak-check=full ./PhotoLabTest
valgrind --leak-check=full ./MovieLab -i EngHall_640_360.yuv -o out_640_360.yuv -f 120 -s 640x360 -bw
```

Both report zero errors and no leaks.

## Not included

Test media (`EngHall.ppm`, `Anteater.ppm`, `EngHall_640_360.yuv`) is not in this repo. Use any PPM image or raw YUV clip of known dimensions.

## Author

Anderson Kwon, UC Irvine, Computer Engineering
