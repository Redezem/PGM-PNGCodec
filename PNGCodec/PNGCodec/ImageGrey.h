
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
// class ImageGrey extends Image
//
// Purpose:
//  Represent an image consisting of greyscale pixels,
//  and allow manipulation of the image.
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//

//class ImageGrey;

#include <bgsub.h>


#ifndef _H_IMAGEGREY
#define _H_IMAGEGREY



class ImageGrey : public Image
{

 //
 //////////////////////////////////////////////////
 // PUBLIC METHODS:
 //////////////////////////////////////////////////
 //
 public:
	explicit ImageGrey(int width, int height, int initToZero);
	explicit ImageGrey(int width, int height, int initToZero, Bool bInvert);
	explicit ImageGrey(const uint8 *imgArray, Bool bIsGrey, int width, int height);
	explicit ImageGrey(const uint8 *imgArray, Bool bIsGrey, int width, int height, Bool bInvert);
	explicit ImageGrey(const Image &img);
	ImageGrey(const ImageGrey &img);
	virtual ~ImageGrey();

	virtual Image* clone() const;

	int bitsPerPixel() const;

	void clear();

	// Convenience function for getting a ref to the actual pixel in the buffer.
	// Only use it if you are desperate for performance - else use grey()/setGrey()

	inline void getRGB(int x, int y, uint8 &red, uint8 &green, uint8 &blue) const;
	inline RGB24 getRGB(int x, int y) const;
	inline void setRGB(int x, int y, uint8 red, uint8 green, uint8 blue);
	inline void setRGB(int x, int y, RGB24 rgb);
	inline void setRGB(int x, int y, const Image &other);

	uint8 red(int x, int y) const;
	void setRed(int x, int y, uint8 red);
	uint8 green(int x, int y) const;
	void setGreen(int x, int y, uint8 green);
	uint8 blue(int x, int y) const;
	void setBlue(int x,  int y, uint8 blue);

	inline uint8 grey(int x, int y) const;
	inline void setGrey(int x, int y, uint8 grey);

	Image* subsample(int factor) const;

	inline Bool isBackground(int x, int y) const;

	uint8 *imageBuffer() const;
	void setBuffer(const uint8 *greyBuf);
	void setBuffer(const uint8 *imgArray, Bool bIsGrey);

	Image* subImage(int x, int y, int width, int height) const;

	void copy(Image &destn, int srcX, int srcY, int destnX, int destnY,
					  int width, int height) const;

	void copy(ImageGrey &destn, int srcX, int srcY, int destnX, int destnY,
					  int width, int height) const;

	virtual Image &operator=(const Image &img);




 //
 //////////////////////////////////////////////////
 // PROTECTED METHODS:
 //////////////////////////////////////////////////
 //
 protected:
	void initImage(int width, int height, Bool bInvert);
	void setSize(int newWidth, int newHeight);
	void allocPixelMap();

	// NOT inline so that we can put a debug breakpoint on it
	void checkBounds(int x, int y) const;


 //
 //////////////////////////////////////////////////
 // MEMBER VARIABLES:
 //////////////////////////////////////////////////
 //
 protected:
	//
	// m_pixmap is a 2D array of pixels.
	// Note that in our allocation of this, we force the block to be
	// contiguous, facilitating fast duplication.
	//
	// This gives us an overhead of 4*height bytes (~1Kb for a 320x240 img)
	// since each row is pointed to as well as the entire block, but the
	// ease of manipulating the pixmap makes up for it.
	//
	// Final note: This 2D array is set up like a matrix, where the
	// y-coord is first, then the x-coord.
	uint8 **m_pixmap;
	uint8 *m_bufPtr;   // Pointer to the top of the image buffer
	                       // Independent of whether inverted or not 
};




//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// INLINE FUNCTIONS
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
inline void ImageGrey::getRGB(int x, int y, uint8 &red, uint8 &green, uint8 &blue) const
{
#ifndef _NO_ERR_CHECK
	checkBounds(x, y);
#endif
	red = green = blue = m_pixmap[y][x];
}


inline RGB24 ImageGrey::getRGB(int x, int y) const
{
#ifndef _NO_ERR_CHECK
	checkBounds(x, y);
#endif
	RGB24 retRGB;
	retRGB.red = retRGB.green = retRGB.blue = m_pixmap[y][x];
	return retRGB;
}


inline void ImageGrey::setRGB(int x, int y, uint8 red, uint8 green, uint8 blue)
{
#ifndef _NO_ERR_CHECK
	checkBounds(x, y);
#endif
	m_pixmap[y][x] = Image::RGBtoGrey(red, green, blue);
}


inline void ImageGrey::setRGB(int x, int y, RGB24 rgb)
{
#ifndef _NO_ERR_CHECK
	checkBounds(x, y);
#endif
	m_pixmap[y][x] = Image::RGBtoGrey(rgb);
}


inline void ImageGrey::setRGB(int x, int y, const Image &other)
{
#ifndef _NO_ERR_CHECK
	checkBounds(x, y);
#endif
	m_pixmap[y][x] = other.grey(x, y);
}


inline uint8 ImageGrey::grey(int x, int y) const
{
#ifndef _NO_ERR_CHECK
	checkBounds(x, y);
#endif
	return m_pixmap[y][x];
}


inline void ImageGrey::setGrey(int x, int y, uint8 grey)
{
	m_pixmap[y][x] = grey;
}



inline Bool ImageGrey::isBackground(int x, int y) const
{
#ifndef _NO_ERR_CHECK
	checkBounds(x, y);
#endif
	return !(m_pixmap[y][x]);
}



#endif // _H_IMAGEGREY

