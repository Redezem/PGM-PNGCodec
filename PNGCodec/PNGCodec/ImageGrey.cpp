
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


#include <ImageGrey.h>

#ifdef USE_LIBTRACKER_NAMESPACE
namespace libTracker {
#endif

//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// CONSTRUCTORS / DESTRUCTORS
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////
//
// ImageGrey()
//
// Purpose:
//  Copy constructor for other Image types
//
// Parameters:
//  IN Image &img - Image to copy
//
////////////////////////////////////////////////////
//
ImageGrey::ImageGrey(const Image &img)
{
	operator=(img);
}



//
////////////////////////////////////////////////////
//
// ImageGrey()
//
// Purpose:
//  Copy constructor for ImageGrey types
//
// Parameters:
//  IN ImageGrey &img - Image to copy
//
////////////////////////////////////////////////////
//
ImageGrey::ImageGrey(const ImageGrey &img)
{
	operator=(img);
}




//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
ImageGrey::ImageGrey(int width, int height, int initToZero)
{
	Bool bInvert;

#ifndef _NO_ERR_CHECK
	if (width < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "width", 0);
	if (height < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "height", 0);
#endif

#ifdef WIN32
	bInvert = TRUE;
#else
	bInvert = FALSE;
#endif

	initImage(width, height, bInvert);

	// Initialise image to 0 if required
	if (initToZero)
		memset(m_bufPtr, 0, (m_iWidth * m_iHeight * sizeof(uint8)));
}



//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
ImageGrey::ImageGrey(int width, int height, int initToZero, Bool bInvert)
{
#ifndef _NO_ERR_CHECK
	if (width < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "width", 0);
	if (height < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "height", 0);
#endif

#ifdef WIN32
	bInvert = !bInvert;
#endif
	
	initImage(width, height, bInvert);

	// Initialise image to 0 if required
	if (initToZero)
		memset(m_bufPtr, 0, (m_iWidth * m_iHeight * sizeof(uint8)));
}



//
////////////////////////////////////////////////////
//
// ImageGrey()
//
// Purpose:
//  Create a new image based on a char buffer.
//  This buffer must be either BGRBGRBGR... or VVVVVV... (grey value)
//  depending on the isGrey flag.
//
// Parameters:
//  IN char *imgArray - buffer containing the image data
//  IN Bool bIsGrey - Flag indicating whether the buffer is 24-bit or 8-bit
//  IN int width - Width of the image
//  IN int height - Height of the image
//
////////////////////////////////////////////////////
//
ImageGrey::ImageGrey(const uint8 *imgArray, Bool bIsGrey, int width, int height)
{
	Bool bInvert;

#ifndef _NO_ERR_CHECK
	if (width < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "width", 0);
	if (height < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "height", 0);
	if (imgArray == NULL)
		throw new NullPointerException("ImageGrey::ImageGrey()", "imgArray");

#endif

#ifdef WIN32
	bInvert = TRUE;
#else
	bInvert = FALSE;
#endif

	initImage(width, height, bInvert);

	setBuffer(imgArray, bIsGrey);
}



//
////////////////////////////////////////////////////
//
// ImageGrey()
//
// Purpose:
//  Create a new image based on a char buffer.
//  This buffer must be either BGRBGRBGR... or VVVVVV... (grey value)
//  depending on the isGrey flag.
//
// Parameters:
//  IN char *imgArray - buffer containing the image data
//  IN Bool bIsGrey - Flag indicating whether the buffer is 24-bit or 8-bit
//  IN int width - Width of the image
//  IN int height - Height of the image
//
////////////////////////////////////////////////////
//
ImageGrey::ImageGrey(const uint8 *imgArray, Bool bIsGrey, int width, int height,
							Bool bInvert)
{
#ifndef _NO_ERR_CHECK
	if (width < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "width", 0);
	if (height < 0)
		throw new NumberOutOfRangeException("ImageGrey::ImageGrey()", "height", 0);
	if (imgArray == NULL)
		throw new NullPointerException("ImageGrey::ImageGrey()", "imgArray");
#endif

#ifdef WIN32
	bInvert = !bInvert;
#endif	

	initImage(width, height, bInvert);

	setBuffer(imgArray, bIsGrey);
}






//
////////////////////////////////////////////////////
//
// ~Image()
//
////////////////////////////////////////////////////
//
ImageGrey::~ImageGrey()
{
	//
	// Destroy the pixel map
	//
	if (m_pixmap != NULL) {
		// Delete the contiguous block allocation
		delete [] m_bufPtr;

		// Delete the pointer array
		delete [] m_pixmap;
		m_pixmap = NULL;
	}
}




//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// PUBLIC MEMBERS
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
Image* ImageGrey::clone() const
{
	return new ImageGrey(*this);
}


//
////////////////////////////////////////////////////
//
// int bitsPerPixel()
//
// Returns:
//  Number of bits per pixel (8 for grey).
//  Useful for determining the type of this image (grey or RGB24)
//
////////////////////////////////////////////////////
//
int ImageGrey::bitsPerPixel() const
{
	return sizeof(uint8) * 8;
}




//
////////////////////////////////////////////////////
//
// void clear()
//
// Purpose:
//  Clear the current image by setting all pixels to 0
//
////////////////////////////////////////////////////
//
void ImageGrey::clear()
{
	memset(m_bufPtr, 0, (m_iWidth * m_iHeight * sizeof(uint8)));
}




//
////////////////////////////////////////////////////
//
// char red()
//
// Parameters:
//  IN int x - (x, y) pixel
//  IN int y - (x, y) pixel
//
// Returns:
//  The red value of the pixel
//
////////////////////////////////////////////////////
//
uint8 ImageGrey::red(int x, int y) const
{
#ifndef _NO_ERR_CHECK
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::red()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::red()", "y", 0, m_iHeight-1);
#endif

	return m_pixmap[y][x];
}



//
////////////////////////////////////////////////////
//
// void setRed()
//
// Purpose:
//  Set the red value for the pixel
//
// Parameters:
//  IN int x - (x, y) pixel
//  IN int y - (x, y) pixel
//  IN uint8 red - New red value
//
////////////////////////////////////////////////////
//
void ImageGrey::setRed(int x, int y, uint8 red)
{
#ifndef _NO_ERR_CHECK
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::setRed()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::setRed()", "y", 0, m_iHeight-1);
#endif

	m_pixmap[y][x] = (uint8) ( ((double)m_pixmap[y][x] * 0.7)  + ((double)red * 0.3) );
}



//
////////////////////////////////////////////////////
//
// uint8 green()
//
// Parameters:
//  IN int x - (x, y) pixel
//  IN int y - (x, y) pixel
//
// Returns:
//  The green value of the pixel
//
////////////////////////////////////////////////////
//
uint8 ImageGrey::green(int x, int y) const
{
#ifndef _NO_ERR_CHECK
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::green()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::green()", "y", 0, m_iHeight-1);
#endif

	return m_pixmap[y][x];
}



//
////////////////////////////////////////////////////
//
// void setGreen()
//
// Purpose:
//  Set the green value for the pixel
//
// Parameters:
//  IN int x - (x, y) pixel
//  IN int y - (x, y) pixel
//  IN uint8 green - New green value
//
////////////////////////////////////////////////////
//
void ImageGrey::setGreen(int x, int y, uint8 green)
{
#ifndef _NO_ERR_CHECK
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::setGreen()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::setGreen()", "y", 0, m_iHeight-1);
#endif

	m_pixmap[y][x] = (uint8) ( ((double)m_pixmap[y][x] * 0.41)  + ((double)green * 0.59) );
}


//
////////////////////////////////////////////////////
//
// char blue()
//
// Parameters:
//  IN int x - (x, y) pixel
//  IN int y - (x, y) pixel
//
// Returns:
//  The blue value of the pixel
//
////////////////////////////////////////////////////
//
uint8 ImageGrey::blue(int x, int y) const
{
#ifndef _NO_ERR_CHECK
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::blue()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::blue()", "y", 0, m_iHeight-1);
#endif

	return m_pixmap[y][x];
}



//
////////////////////////////////////////////////////
//
// void setBlue()
//
// Purpose:
//  Set the blue value for the pixel
//
// Parameters:
//  IN int x - (x, y) pixel
//  IN int y - (x, y) pixel
//  IN uint8 blue - New blue value
//
////////////////////////////////////////////////////
//
void ImageGrey::setBlue(int x,  int y, uint8 blue)
{
#ifndef _NO_ERR_CHECK
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::setBlue()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::setBlue()", "y", 0, m_iHeight-1);
#endif

	m_pixmap[y][x] = (uint8) ( ((double)m_pixmap[y][x] * 0.89)  + ((double)blue * 0.11) );
}




//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
Image* ImageGrey::subsample(int factor) const
{
	throw new UnsupportedOperationException("ImageGrey::subsample()", "Not yet implemented");
}




//
////////////////////////////////////////////////////
//
// uint8 *imageBuffer()
//
// Purpose:
//  Give access to the image buffer.
//	Note that the buffer returned will be of size:
//		width() * height() * 1
//
// Returns:
//  The image buffer in VVVVVVVV... format.
//  The caller MUST NOT free this!
//
////////////////////////////////////////////////////
//
uint8 *ImageGrey::imageBuffer() const
{
	return (uint8 *)m_bufPtr;
}




//
////////////////////////////////////////////////////
//
// void setBuffer(uint8 *rgb24Array)
//
// Purpose:
//  Copy the given buffer over our internal buffer, effectively
//  replacing the image.
//
// Parameters:
//  uint8 *greyBuf- Buffer to copy over. MUST be of size
//                          m_iWidth * m_iHeight (ie: greyscale)
//
////////////////////////////////////////////////////
//
void ImageGrey::setBuffer(const uint8 *greyBuf)
{
	setBuffer(greyBuf, TRUE);
}



//
////////////////////////////////////////////////////
//
// void setBuffer(uint8 *imgArray, Bool bIsGrey)
//
// Purpose:
//  Copy the given buffer over our internal buffer, effectively
//  replacing the image.
//
// Parameters:
//  uint8 *imgArray - Buffer to copy and set as new image buffer
//  Bool isGrey - Boolean value indicating whether imgArray is grey or rgb24
//               If TRUE, imgArray MUST be BGRBGRBGR... and of size
//                          m_iWidth * m_iHeight * 3
//               If FALSE, imgArray MUST be VVVVV... and of size
//                          m_iWidth * m_iHeight * 1
//
////////////////////////////////////////////////////
//
void ImageGrey::setBuffer(const uint8 *imgArray, Bool bIsGrey)
{
	int iArrIdx, i, j, y;

#ifndef _NO_ERR_CHECK
	if (imgArray == NULL)
		throw new NullPointerException("ImageGrey::setBuffer()", "imgArray");
#endif

	if (bIsGrey) {
		// THIS ASSUMES THAT THE GIVEN BUFFER IS THE SAME INVERSION
		// FORMAT AS THIS OBJECT'S  m_bInvert FLAG IMPLIES!
		memcpy(m_bufPtr, imgArray, (m_iWidth * m_iHeight * sizeof(uint8)));
	}
	else {
		//
		// Translate the rgbArray into grey values and set our pixels.
		//
		iArrIdx = 0;
		for (j = 0; j < m_iHeight; j++) {
			for (i = 0; i < m_iWidth; i++) {

				if (m_bInvert)
					y = m_iHeight - j - 1;
				else
					y = j;

				// Assume BGR ordering
				setRGB(y, i, imgArray[iArrIdx+2], imgArray[iArrIdx+1], imgArray[iArrIdx]);
				iArrIdx += 3;
			}
		}
	}
}



//
////////////////////////////////////////////////////
//
// Image* subImage()
// 
// Purpose:
//  Extract a subimage from the current image
//
// Parameters:
//  IN int x - x position of top left (x,y) pixel
//  IN int y - y position of top left (x,y) pixel
//  IN int width - Width of subimage
//  IN int height - Height of subimage
//
// Returns:
//  Pointer to the new subimage.
//  The caller has responsibility to free this subimage.
//
////////////////////////////////////////////////////
//
Image* ImageGrey::subImage(int x, int y, int width, int height) const
{
	ImageGrey* img;

#ifndef _NO_ERR_CHECK
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::subImage()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::subImage()", "y", 0, m_iHeight-1);

	// Check that the requested width and height isn't too large (or even <= 0!)
	if ( ((width + x) > m_iWidth) || (width <= 0) )
		throw new OutOfBoundsException("ImageGrey::subImage()", "width", 1, m_iWidth);
	if ( ((height + y) > m_iHeight) || (height <= 0) )
		throw new OutOfBoundsException("ImageGrey::subImage()", "height", 1, m_iHeight);
#endif

	img = new ImageGrey(width, height, FALSE);
	copy(*img, x, y, 0, 0, width, height);

	return img;
}



//
////////////////////////////////////////////////////
//
// void copy()
//
// Purpose:
//  Copy a block from this image to any position in another image.
//
// Parameters:
//  IN/OUT Image &destn - Destination image
//  IN int srcX - x position from this source image (top left)
//  IN int srcY - y position from this source image (top left)
//  IN int destnX - x position for the destination image (top left)
//  IN int destnY - y position for the destination image (top left)
//  IN int width - width of block to copy
//  IN int height - height of block to copy
//
////////////////////////////////////////////////////
//
void ImageGrey::copy(Image &destn, int srcX, int srcY, int destnX, int destnY,
							int width, int height) const
{
	int iSrcX, iDestX, iSrcY, iDestY;

#ifndef _NO_ERR_CHECK
	if ((srcX < 0) || (srcX >= m_iWidth))
		throw new OutOfBoundsException("ImageGrey::copy()", "srcX", 0, m_iWidth-1);
	if ((srcY < 0) || (srcY >= m_iHeight))
		throw new OutOfBoundsException("ImageGrey::copy()", "srcY", 0, m_iHeight-1);
	if ((destnX < 0) || (destnX >= destn.width()))
		throw new OutOfBoundsException("ImageGrey::copy()", "destnX", 0, m_iWidth-1);
	if ((destnY < 0) || (destnY >= destn.height()))
		throw new OutOfBoundsException("ImageGrey::copy()", "destnY", 0, m_iHeight-1);

	if ( (width <= 0) || ((width + srcX) > m_iWidth) || ((width + destnX) > destn.width()) )
		throw new OutOfBoundsException("ImageGrey::copy()", "width", 1, m_iWidth);
	if ( (height <= 0) || ((height + srcY) > m_iHeight) || ((height + destnY) > destn.height()) )
		throw new OutOfBoundsException("ImageGrey::copy()", "height", 1, m_iHeight);
#endif

	//
 	// Copy one pixel at a time, setting the grey value
	//
	// THIS ASSUMES THAT THE GIVEN BUFFER IS THE SAME INVERSION
	// FORMAT AS THIS OBJECT'S m_bInvert FLAG IMPLIES!
	//
	for (iSrcY=srcY, iDestY=destnY; iSrcY < (srcY + height); iSrcY++, iDestY++) {
		for (iSrcX=srcX, iDestX=destnX; iSrcX < (srcX + width); iSrcX++, iDestX++) {
			destn.setGrey(iDestX, iDestY, m_pixmap[iSrcY][iSrcX]);
		}
	}
}



//
////////////////////////////////////////////////////
//
// void copy()
//
// Purpose:
//  Copy a block from this image to any position in another image.
//  We assume that the destination image is large enough to
//  receive the copy.
//  The only difference between this and the previous copy() function
//  is that this function takes an ImageGrey, not an Image. This
//  allows us to optimise the copy process using memcpy().
//
// Parameters:
//  IN/OUT ImageGrey &destn - Destination image
//  IN int srcX - x position from this source image (top left)
//  IN int srcY - y position from this source image (top left)
//  IN int destnX - x position for the destination image (top left)
//  IN int destnY - y position for the destination image (top left)
//  IN int width - width of block to copy
//  IN int height - height of block to copy
//
////////////////////////////////////////////////////
//
void ImageGrey::copy(ImageGrey &destn, int srcX, int srcY, int destnX, int destnY,
					 int width, int height) const
{
	int iSrcY, iDestY;

#ifndef _NO_ERR_CHECK
	if ((srcX < 0) || (srcX >= m_iWidth))
		throw new OutOfBoundsException("ImageGrey::copy()", "srcX", 0, m_iWidth-1);
	if ((srcY < 0) || (srcY >= m_iHeight))
		throw new OutOfBoundsException("ImageGrey::copy()", "srcY", 0, m_iHeight-1);
	if ((destnX < 0) || (destnX >= destn.width()))
		throw new OutOfBoundsException("ImageGrey::copy()", "destnX", 0, m_iWidth-1);
	if ((destnY < 0) || (destnY >= destn.height()))
		throw new OutOfBoundsException("ImageGrey::copy()", "destnY", 0, m_iHeight-1);

	if ( (width <= 0) || ((width + srcX) > m_iWidth) || ((width + destnX) > destn.width()) )
		throw new OutOfBoundsException("ImageGrey::copy()", "width", 1, m_iWidth);
	if ( (height <= 0) || ((height + srcY) > m_iHeight) || ((height + destnY) > destn.height()) )
		throw new OutOfBoundsException("ImageGrey::copy()", "height", 1, m_iHeight);
#endif

	//
	// Copy one row at a time
	//
	// THIS ASSUMES THAT THE GIVEN BUFFER IS THE SAME INVERSION
	// FORMAT AS THIS OBJECT'S m_bInvert FLAG IMPLIES!
	//
	for (iSrcY=srcY, iDestY=destnY; iSrcY < (srcY + height); iSrcY++, iDestY++) {
		memcpy(&(destn.m_pixmap[iDestY][destnX]), &(m_pixmap[iSrcY][srcX]), width);
	}
}



//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
Image &ImageGrey::operator=(const Image &obj)
{
	int i, j;
	const ImageGrey* imgGrey;
	const Image* img;

	if (&obj == this)
		return *this;


	Image::operator=(obj);


	imgGrey = dynamic_cast<const ImageGrey *>(&obj);
	if (imgGrey == NULL) {

		// Try as a normal Image
		img = dynamic_cast<const Image *>(&obj);
		if (img == NULL)
			throw new IllegalArgumentException("ImageGrey::operator=", "obj", "Incompatible type for assignment operation");
		else {
			// We must do a pixel-by-pixel copy
			initImage(img->width(), img->height(), img->isInverted());

			for (j = 0; j < m_iHeight; j++) {
				for (i = 0; i < m_iWidth; i++) {
					m_pixmap[j][i] = img->grey(i, j);
				}
			}
		}
	}
	else {
		// We can do things faster since we know it's another ImageGrey
		initImage(imgGrey->m_iWidth, imgGrey->m_iHeight, imgGrey->m_bInvert);
		memcpy(m_bufPtr, imgGrey->m_bufPtr, (m_iWidth * m_iHeight * sizeof(uint8)));
	}

	return *this;
}







//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// PROTECTED MEMBERS
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////
//
// void initImage()
//
// Purpose:
//  Initialise the m_pixmap buffer
//
// Parameters:
//  int width - Width of image buffer
//  int height - Width of image buffer
//
////////////////////////////////////////////////////
//
void ImageGrey::initImage(int width, int height, Bool bInvert)
{
	m_iWidth = width;
	m_iHeight = height;

	m_bInvert = bInvert;

	allocPixelMap();
}



//
////////////////////////////////////////////////////
//
// void allocPixelMap()
//
// Purpose:
//  Allocate space for the image
//
////////////////////////////////////////////////////
//
void ImageGrey::allocPixelMap()
{
	int i;

	//
	// Allocate the entire width*height block to the first row pointer, BUT
	// set each row pointer to point to the start of each LOGICAL
	// row within this block.
	// 
	// This is a nice fudge to force a contiguous block of memory
	// for the pixmap whilst still being able to access it like a 2D array.
	// The contiguous memory block facilitates quick duplication.
	//
	// NOTE: This is in matrix style, where y is the first dimension and 
	//       x is the second. We can't get away from this since the capture
	//       device provides the video stream row-by-row, not col-by-col.
	//
	// Allocating a char buffer and assigning that to the pixmap means
	// we can avoid having to call an empty constructor
	//
	m_pixmap = new uint8*[m_iHeight];
	m_bufPtr = new uint8[m_iWidth*m_iHeight];


	if (m_bInvert) {
		for (i = 0; i < m_iHeight; i++)
			m_pixmap[m_iHeight-i-1] = m_bufPtr + (i * m_iWidth);
	}
	else {
		for (i = 0; i < m_iHeight; i++) 
			m_pixmap[i] = m_bufPtr + (i * m_iWidth);
	}
}





//
////////////////////////////////////////////////////
//
// void checkBounds()
//
// Check the x,y coordinates are within the image bounds
// This function is needed for the inline get/set functions so that
// a debug breakpoint can be used to catch bounds errors.
//
////////////////////////////////////////////////////
//
void ImageGrey::checkBounds(int x, int y) const
{
	if ((x >= m_iWidth) || (x < 0))
		throw new OutOfBoundsException("ImageGrey::checkBounds()", "x", 0, m_iWidth-1);
	if ((y >= m_iHeight) || (y < 0))
		throw new OutOfBoundsException("ImageGrey::checkBounds()", "y", 0, m_iHeight-1);
}



//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
void ImageGrey::setSize(int newWidth, int newHeight)
{
	// Re-allocate m_pixmap with the new width/height.

	m_iWidth = newWidth;
	m_iHeight = newHeight;
	allocPixelMap();
}



#ifdef USE_LIBTRACKER_NAMESPACE
}
#endif


