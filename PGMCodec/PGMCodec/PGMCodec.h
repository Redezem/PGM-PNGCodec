//unified PGM codec class
#pragma once
#include "pgmfile.h"
#include "malloc_image.h"
class PGMCodec
{
public:
	PGMCodec();
	PGMCodec(char*);
	~PGMCodec();

	void loadImage(char*); //This will only work for pgm. Not at fault for you self destructing your program or files because of this.
	void loadImage(char*, int, int);

	void writeAsPGM(char*);
	void writeAsRaw(char*);

	int getHeight();
	int getWidth();
	int getDepth();

	unsigned char ** getImageMatrix(); //this *actually* gives you the image. Watch out!
	void setImageMatrix(unsigned char **);

private:
	pgmfile currImageHead;
	unsigned char ** currImage;

	void allocImage(int,int);
	void readRaw(char*);
	void readPGM(char*);
	void readPGMHead(char*);
	void freeImage();
};