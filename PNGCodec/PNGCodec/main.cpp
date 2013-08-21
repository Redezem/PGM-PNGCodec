#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdexcept>

#include "PNGCodec.h"

#define BLACK 0
#define WHITE 255

void processImage(const char* fileIn, const char* fileOut);

/* get options and input/output filename from user */

int main(int argc, char *argv[])
{

	if (argc != 3) {
		printf(" Usage:\n");
		printf(" %s file_in file_out\n", argv[0]);
		exit(0);
	}

	try {
		// Main image processing here using processImage
		processImage(argv[1], argv[2]);
	}
	catch (std::exception e) {
		printf("Error during processing: %s", e.what());
	}
  
	return 0;
}

// Open image file from file_in, invert the image and write it out to file_out
void processImage(const char *fileIn, const char *fileOut)
{
	Image* imgIn;
	Image* imgOut;

	// Read in the file
	printf("Reading from file %s\n", fileIn);
	imgIn = PNGCodec::readPNG(fileIn);

	// Allocate memory for imgOut
	imgOut = new Image(imgIn->width, imgIn->height, imgIn->depth);
    /*
	imgOut = new Image; 
    imgOut->width = imgIn->width;
    imgOut->height = imgIn->height;
    imgOut->data = new unsigned char*[imgOut->height];
    for (int y = 0; y < imgOut->height; y++) {
        imgOut->data[y] = new unsigned char[imgOut->width*3];
    }
    */



	// Processing
	// The below simply inverts the pixel values, changing dark to light and vice-versa
	for (int y = 0; y < imgOut->height; y++) {
		for (int x = 0; x < imgOut->width; x++) {
		      imgOut->data[y][x] = 255 - imgIn->data[y][x];
		}
	}

	// Write output image
	printf("Writing to file %s\n", fileOut); 
	PNGCodec::writePNG(fileOut, *imgOut);

	// Clean up memory - the Image destructor will clean up their data arrays
	delete imgIn;
	delete imgOut;
}
