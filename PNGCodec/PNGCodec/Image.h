
//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// Copyright © 2002 School of Computing, 
//                  Curtin University of Technology
//
// Change History:
//  Date        Who                 Comment
//  30-Jun-2002 Patrick Peursum     Original code
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// abstract class Image implements ISerialisable
//
// Purpose:
//  Represents an image consisting of RGB pixels, allowing manipulation 
//  of the image.
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//

//class Image;

#include <bgsub.h>


#ifndef _H_IMAGE
#define _H_IMAGE

// Constants for decompose()
#define DECOMPOSE_RGB	1
#define DECOMPOSE_HSV	2


class Image 
{
 //
 //////////////////////////////////////////////////
 // STATIC METHODS:
 //////////////////////////////////////////////////
 //
 public:
	static HSV24 RGBtoHSV(RGB24 rgb);
	static void RGBtoHSV(uint8 red, uint8 green, uint8 blue, 
						uint8 &hue, uint8 &sat, uint8 &val);

	static RGB24 HSVtoRGB(HSV24 hsv);
	static void HSVtoRGB(uint8 hue, uint8 sat, uint8 val, 
						uint8 &red, uint8 &green, uint8 &blue);

	static uint8 RGBtoGrey(RGB24 rgb);
	static uint8 RGBtoGrey(uint8 red, uint8 green, uint8 blue);

	static RGB24 generateColour(int seqNum);
 //
 //////////////////////////////////////////////////
 // PUBLIC METHODS:
 //////////////////////////////////////////////////
 //
 protected:
 	Image();   // For inherited classes
	// Don't allow copy constructor for this base Image class,
	// because the compiler errors generated from this are VERY confusing!
	explicit Image(const Image &img);	
 public:
	virtual ~Image();	// Force call to inherited destructors


	//
	// Abstract Members:
	//
    virtual Image* clone() const = 0;
	virtual int bitsPerPixel() const = 0;

	virtual void clear() = 0;

	virtual void getRGB(int x, int y, uint8 &red, uint8 &green, uint8 &blue) const = 0;
	virtual RGB24 getRGB(int x, int y) const = 0;
	virtual void setRGB(int x, int y, uint8 red, uint8 green, uint8 blue) = 0;
	virtual void setRGB(int x, int y, RGB24 rgb) = 0;
	virtual void setRGB(int x, int y, const Image &other) = 0;

	virtual void setARGB(int x, int y, RGB32 rgb);
	virtual void setARGB(int x, int y, uint8 alpha, uint8 red, uint8 green, uint8 blue);

	virtual uint8 red(int x, int y) const = 0;
	virtual void setRed(int x, int y, uint8 red) = 0;
	virtual uint8 green(int x, int y) const = 0;
	virtual void setGreen(int x, int y, uint8 green) = 0;
	virtual uint8 blue(int x, int y) const = 0;
	virtual void setBlue(int x,  int y, uint8 blue) = 0;

	virtual uint8 grey(int x, int y) const = 0;
	virtual void setGrey(int x, int y, uint8 grey) = 0;


	virtual HSV24 getHSV(int x, int y) const;
	virtual void getHSV(int x, int y, uint8 &hue, uint8 &sat, uint8 &val) const;
	virtual uint8 hue(int x, int y) const;	// 0 - 255, scaled from 0 - 360
	virtual uint8 sat(int x, int y) const;
	virtual uint8 val(int x, int y) const;


	virtual Image* subsample(int factor) const = 0;


	virtual Bool isBackground(int x, int y) const = 0;

	virtual int numForeground() const;
	virtual int numBackground() const;

	virtual void normalise(Bool bSquareMethod=FALSE);


	void dilate(int dilateSizeX, int dilateSizeY);
	void erode(int erodeSizeX, int erodeSizeY);
	void medianFilter(int medianDiamX, int medianDiamY, float medianThreshold);

	// threshold applies to greyscale
	void threshold(double threshold, Bool bMakeBinary=TRUE);	
	void mask(const Image &imgMask);

	virtual uint8 *imageBuffer() const = 0;
	virtual void setBuffer(const uint8 *rawArray) = 0;

	virtual Image* subImage(int x, int y, int width, int height) const = 0;

	virtual void copy(Image &destn, int srcX, int srcY, int destnX, int destnY,
					  int width, int height) const = 0;






 public:

	//
	// TODO:
	// Allow copying of arbitrary areas. Specifically, we'd like to
	// copy the area bounded by a silhouette (represented by InnerBorder),
	// since then we could grab the actual pixels, avoiding any holes
	// due to BG subtraction.
	// PROBLEM: Sometimes the holes are valid (eg: crook of the arm). Bummer.
	//

	//
	// Other Members
	//
	virtual int width() const;
	virtual int height() const;
	virtual Bool isInverted() const;

	virtual RGB24 drawColour();
	virtual void setDrawColour(uint8 red, uint8 green, uint8 blue);
	virtual RGB24 fillColour();
	virtual void setFillColour(uint8 red, uint8 green, uint8 blue);
	// virtual void setDrawWidth(uint32 width) const;

	virtual void drawRect(FloatRect rect, Bool bFill=FALSE);
	virtual void drawRect(Point2D topLeft, Point2D bottomRight, Bool bFill=FALSE);
	virtual void drawRect(double left, double top, double right, double bottom, Bool bFill=FALSE);
	virtual void drawRect(int left, int top, int right, int bottom, Bool bFill=FALSE);

	virtual void drawLine(Point2D startPt, Point2D endPt);
	virtual void drawLine(double x1, double y1, double x2, double y2);
	virtual void drawLine(int x1, int y1, int x2, int y2);


	//
	// TODO:
	// Other drawing members:
	//
	// virtual void drawPolygon(IPolygon poly, Bool bClosed=TRUE);
	//    where IPolygon has functions numSides() and vertex(int index)
	//
	//virtual void setRGBAll(uint8 red, uint8 green, uint8 blue); 
	//

	virtual Image &operator=(const Image &obj);

#ifdef CVAPI
	virtual IplImage *getOpenCVImage(Bool bMakeCopy=TRUE) const = 0;
#endif


 //
 //////////////////////////////////////////////////
 // PROTECTED METHODS:
 //////////////////////////////////////////////////
 //
 protected:

	//
	// Abstract methods:
	//
	virtual void initImage(int width, int height, Bool bInvert) = 0;
	virtual void allocPixelMap() = 0;
	virtual void setSize(int newWidth, int newHeight) = 0;

/*
 //
 //////////////////////////////////////////////////
 // INTERFACE METHODS:
 //////////////////////////////////////////////////
 //
 public:
	virtual void AddRef();
	virtual void DeleteRef();
	virtual Status GetInterface(Name name, Interface *&result);


 //
 //////////////////////////////////////////////////
 // IIMAGE METHODS:
 //////////////////////////////////////////////////
 //
 public:
	virtual void GetSize(int & width, int & height) const;
	virtual Status Get(int x, int y, Pixel & result) const;
	virtual Status Set(int x, int y, Pixel value);
	virtual void GetRaster(Pixel * & result, int & size) const;


 //
 //////////////////////////////////////////////////
 // IMASK METHODS:
 //////////////////////////////////////////////////
 //
 public:
	// See IImage: virtual void GetSize(int & width, int & height);
	virtual Status Get(int x, int y, Label & result) const;
	virtual Status Set(int x, int y, Label value);
	virtual void GetRaster(Label * & result, int & size) const;
*/

 //
 //////////////////////////////////////////////////
 // MEMBER VARIABLES:
 //////////////////////////////////////////////////
 //
 protected:
	int m_iWidth;
	int m_iHeight;
	// Inverted images are upside-down
	int m_bInvert;

	// This flag indicates whether the image should be inverted
	// on this host for right-side up viewing - AVI frames under Win32
	// are assumed to be inverted by the Win32 version of libeasympeg.
	int m_bHostInvertFlag;

	RGB24 m_iDrawRGB;
	RGB24 m_iFillRGB;
	RGB24 m_iTextRGB;

/*
	// Support for IImage 32-bit rasters - this is a read-only buffer
	// that is (re)generated every time IImage::GetRaster() is called.
	mutable Pixel **m_rgbaBuf;


	// Support for IMask 8-bit rasters - this is a read-only buffer
	// that is (re)generated every time IMask::GetRaster() is called.
	mutable Label **m_labelBuf;
*/
};

#endif // _H_IMAGE
