#include "PNGCodec.h"

#define uint8 unsigned char

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
// PNGCodec()
//
////////////////////////////////////////////////////
//
PNGCodec::PNGCodec()
{

}


//
////////////////////////////////////////////////////
//
// ~PNGCodec()
//
////////////////////////////////////////////////////
//
PNGCodec::~PNGCodec()
{
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
// void readPNG()
//
////////////////////////////////////////////////////
//
Image* PNGCodec::readPNG(const char *filename)
{
	FILE *pngFP;
	png_structp png;
	png_infop pngInfo;
	Image* imgPNG;

#ifndef _NO_ERR_CHECK
	if (filename == NULL)
		throw new std::runtime_error("PNGCodec::readPNG(), filename");
#endif


	if ((pngFP = fopen(filename, "rb")) == NULL)
		throw new std::runtime_error("PNGCodec::readPNG() filename");


	initPNGStructs(pngFP, png, pngInfo, 1);

	/* If we have already read some of the signature */
	///////////////   png_set_sig_bytes(png_ptr, sig_read);

	imgPNG = readPNGIntoImage(png, pngInfo);

	return imgPNG;
}



//
////////////////////////////////////////////////////
//
// void writePNG()
//
////////////////////////////////////////////////////
//
void PNGCodec::writePNG(const char *filename, Image &img)
{
	FILE *pngFP;
	png_structp png;
	png_infop pngInfo;

#ifndef _NO_ERR_CHECK
	if (filename == NULL)
		throw new std::runtime_error("PNGCodec::writePNG(), filename");
#endif

	if ((pngFP = fopen(filename, "wb")) == NULL)
		throw new std::runtime_error("PNGCodec::writePNG(), filename");


	initPNGStructs(pngFP, png, pngInfo, 0);

	writeImageToPNG(img, png, pngInfo);
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
// void initPNGStructs()
//
////////////////////////////////////////////////////
//
void PNGCodec::initPNGStructs(FILE *pngFP, png_structp &png, png_infop &pngInfo, int bForReading)
{

#ifndef _NO_ERR_CHECK
	if (pngFP == NULL)
		throw new std::runtime_error("PNGCodec::initPNGStructs(), pngFP");
#endif
	


	if (bForReading)
		png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
	else
		png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
	
	if (png == NULL) {
		fclose(pngFP);
		throw new std::runtime_error("Failed to create PNG struct, PNGCodec::initPNGStructs()");
	}

	// Allocate/initialize the memory for image information.
	pngInfo = png_create_info_struct(png);
	if (pngInfo == NULL) {
		// Clean up
		fclose(pngFP);
		if (bForReading)
			png_destroy_read_struct(&png, png_infopp_NULL, png_infopp_NULL);
		else
			png_destroy_write_struct(&png, png_infopp_NULL);

		throw new std::runtime_error("Failed to create PNG info struct, PNGCodec::initPNGStructs()");
	}


   // Set up the input control if using the standard C streams
	png_init_io(png, pngFP);
}




//
////////////////////////////////////////////////////
//
// Image* readPNGIntoImage()
//
////////////////////////////////////////////////////
//
Image* PNGCodec::readPNGIntoImage(png_structp &png, png_infop &pngInfo)
{
	uint8 **buf;
	int y;
	png_uint_32 iWidth, iHeight, iRowLength;
	png_byte iColourType, iBitsPerChannel, iNumChannels;
	png_bytepp pngRows;
	Image* imgPNG;

	png_read_png(png, pngInfo, PNG_TRANSFORM_BGR | PNG_TRANSFORM_STRIP_ALPHA, png_voidp_NULL);
	
	// pixels are in info_ptr->row_pointers
	// where row_pointers is:
	//   png_bytep row_pointers[height];
	// and is probably not contiguous

	iColourType = png_get_color_type(png, pngInfo);
	//	if (iColourType != PNG_COLOR_TYPE_RGB_ALPHA)
	if (iColourType != PNG_COLOR_TYPE_RGB)
		throw new std::runtime_error("Colour type not supported - RGB only, PNGCodec::readPNGIntoImage()");



	iBitsPerChannel = png_get_bit_depth(png, pngInfo);
	iNumChannels = png_get_channels(png, pngInfo);

	iHeight = png_get_image_height(png, pngInfo);
	iWidth = png_get_image_width(png, pngInfo);

	iRowLength = iWidth * (iBitsPerChannel * iNumChannels) / 8; // Should be same as iWidth

	// 07-Jul-2009: Directly load the PNG into a pre-created image's
	//     buffer. Otherwise ImageRGB24 will make a copy of the buf, which
	//     is a waste of space (and problematic for very large PNGs!)
	imgPNG = new Image;
    imgPNG->width = iWidth;
    imgPNG->height = iHeight;
    imgPNG->data = new unsigned char*[iHeight];
	for (y = 0; y < iHeight; y++) {
        imgPNG->data[y] = new unsigned char[iRowLength];
    }

	buf = imgPNG->data;
	pngRows = png_get_rows(png, pngInfo);
	for (y = 0; y < iHeight; y++) {
		//		for (x = 0; x < iWidth; x++) {
		// This assumes BGR order, in readiness for the ImageRGB24 constructor
		// It also assumes 24-bit, no alpha
        // Yes, this is not good...
		memcpy(buf[y], pngRows[y], iRowLength);
		//memcpy((buf + (y * iRowLength)), pngRows[y], iRowLength);
	}
			

	// Clean up
	png_destroy_read_struct(&png, &pngInfo, png_infopp_NULL);

	return imgPNG;
}







//
////////////////////////////////////////////////////
//
// ImageRGB24* readPNGIntoImage()
//
////////////////////////////////////////////////////
//
void PNGCodec::writeImageToPNG(Image &img, png_structp &png, png_infop &pngInfo)
{
	int y;
	png_uint_32 iWidth, iHeight;
	uint8 **imgBuf;
	png_bytepp pngRows;

	iWidth = img.width;
	iHeight = img.height;

	png_set_IHDR(png, pngInfo, iWidth, iHeight, 8, PNG_COLOR_TYPE_RGB,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);


	png_write_info(png, pngInfo);


	/* flip BGR pixels to RGB */
	png_set_bgr(png);


	pngRows = new png_bytep[iHeight];
	imgBuf = img.data;
	for (y = 0; y < iHeight; y++)
		//pngRows[y] = imgBuf + (y * iWidth * 3);
		pngRows[y] = imgBuf[y];

	png_write_image(png, pngRows);

	png_write_end(png, pngInfo);

	// Clean up
	delete [] pngRows;
	png_destroy_write_struct(&png, &pngInfo);

}







#ifdef USE_LIBTRACKER_NAMESPACE
}
#endif


