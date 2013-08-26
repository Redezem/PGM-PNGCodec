#include "PGMCodec.h"
#define NULL 0 //And bamf, NULL

PGMCodec::PGMCodec() //generic PGM container constructor
{
	currImageHead.pgm_depth=0;
	currImageHead.pgm_height=0;
	currImageHead.pgm_width=0;

	currImage = NULL;
}

PGMCodec::PGMCodec(char* inFile) //PGM loader. Loads PGM files *only*
{
	readPGMHead(inFile); //need to malloc here!! Can't just write to nothing derp for brains!
	allocImage(currImageHead.pgm_width,currImageHead.pgm_height);
	readPGM(inFile);	
}

void PGMCodec::allocImage(int width,int height)
{
	currImage=malloc_char_image(width,height);
}

void PGMCodec::freeImage()
{
	free_unsigned_char_image(currImage,currImageHead.pgm_height);
}

int PGMCodec::getDepth()
{
	return currImageHead.pgm_depth;
}

int PGMCodec:: getHeight()
{
	return currImageHead.pgm_height;
}

int PGMCodec::getWidth()
{
	return currImageHead.pgm_width;
}

unsigned char ** PGMCodec::getImageMatrix()
{
	return currImage;
}

void PGMCodec::setImageMatrix(unsigned char** inArr)
{
	currImage=inArr;
}

void PGMCodec::loadImage(char* inFile)
{
	readPGMHead(inFile);
	readPGM(inFile);
}

void PGMCodec::loadImage(char* inFile,int width, int height)
{
	currImageHead.pgm_height=height;
	currImageHead.pgm_width=width;
	readRaw(inFile);
}

void PGMCodec::readPGM(char* inFile)
{
	read_pgm_image(currImage,inFile,&currImageHead);
}

void PGMCodec::readPGMHead(char* inFile)
{
	get_pgm_header(inFile,&currImageHead);
}

void PGMCodec::readRaw(char* inFile)
{
	read_image(currImage,inFile,currImageHead.pgm_height,currImageHead.pgm_width);
}

void PGMCodec::writeAsPGM(char* outFile)
{
	write_pgm_image(currImage,outFile,&currImageHead);
}

void PGMCodec::writeAsRaw(char* outFile)
{
	write_image(currImage,outFile,currImageHead.pgm_height,currImageHead.pgm_width);
}