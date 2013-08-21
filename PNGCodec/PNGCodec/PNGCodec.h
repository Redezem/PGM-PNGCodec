#include <png.h>
#include <stdexcept>


//#ifndef _H_PNGCODEC
//#define _H_PNGCODEC
 //OR, we could pragma once
//WHY DIDN'T YOU PRAGMA ONCE MAN?! WHYY?!
#pragma once
//WE COULD HAVE JUST... GONE HOME. BUT NO. C STYLE PREPROCESSING. >=(

//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// class Image
//
// Purpose:
//  Basic struct for images
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
class Image
{
 //
 ////////////////////////////////////////////////////
 // PUBLIC MEMBERS
 // Everything is public for maximum speed, and forces students to deal with the data directly
 ////////////////////////////////////////////////////
 //
 public:
	int width;				// Width of image
	int height;				// Height of image
	int depth;				// Number of channels (greyscale = 1 channel, RGB = 3)
	unsigned char** data;		// Pixel data, indexed by [y][x]

	Image(int inWidth, int inHeight, int inDepth);
	~Image();

 private: //Redezem -- Static? Pff, don't need none of that...
	void initPNGStructs(FILE *pngFP, png_structp &png, png_infop &pngInfo, int bForReading); 
	void readPNGIntoImage(png_structp &png, png_infop &pngInfo);
//	void writeImageToPNG(Image &img, png_structp &png, png_infop &pngInfo); Yeah... I'ma make this public
};



//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// class PNGCodec
//
// Purpose:
//  Helper functions to read and write PNG files
//  These *could* have been put inside Image, but then Image would be
//  intrinsically tied to PNG files, and that's probably not great design
// ==REDEZEM==
//	-- Unless, of course, you're me, and you say "Nah mans, it *is* good design"
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
class PNGCodec 
{
 //
 ////////////////////////////////////////////////////
 // PUBLIC MEMBERS
 ////////////////////////////////////////////////////
 //
 public:
	PNGCodec();
	virtual ~PNGCodec();

	static Image* readPNG(const char *filename);
	static void writePNG(const char *filename, Image &img);

 //
 ////////////////////////////////////////////////////
 // PROTECTED MEMBERS
 ////////////////////////////////////////////////////
 //

	//Redezem:Just excuse me as I destroy this pretty design in favor of a worse, mutant-zombie one
 protected:
	static void initPNGStructs(FILE *pngFP, png_structp &png, png_infop &pngInfo, int bForReading);
	static Image* readPNGIntoImage(png_structp &png, png_infop &pngInfo);
	static void writeImageToPNG(Image &img, png_structp &png, png_infop &pngInfo);

 //
 ////////////////////////////////////////////////////
 // MEMBER VARIABLES
 ////////////////////////////////////////////////////
 //
 protected:
};

//#endif	// _H_PNGCODEC -- Redezem: NO :<

