#include "PNGCodec.h"



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
Image::Image(int inWidth, int inHeight, int inDepth) 
{
	if (inWidth <= 0)
		throw new std::runtime_error("Width must be positive");
	if (inHeight <= 0)
		throw new std::runtime_error("Height must be positive");
	if (inDepth != 1)
		throw new std::runtime_error("Depth must be 1 - only greyscale images supported in this version");

	width = inWidth;
	height = inHeight;
	depth = inDepth;

	// Initialise memory - allocate row pointers, then allocate each row
	data = new unsigned char*[height];
	for (int y = 0; y < height; y++) {
		data[y] = new unsigned char[width * depth];
		memset(data[y], 0, width * depth);	// Init to zeros
	}
}

Image::~Image()
{
	if (data != NULL) {
		for (int y = 0; y < height; y++) {
			delete [] data[y];
        }
		delete[] data;
		data = NULL;
	}
}



//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// class PNGCodec
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//


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
	// Nothing to init - all members are static
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
	Image* img;

	if (filename == NULL)
		throw new std::runtime_error("PNGCodec::readPNG(): filename must not be NULL");


	if ((pngFP = fopen(filename, "rb")) == NULL)
		throw new std::runtime_error("PNGCodec::readPNG(): file not found");

	initPNGStructs(pngFP, png, pngInfo, 1);

	/* If we have already read some of the signature */
	///////////////   png_set_sig_bytes(png_ptr, sig_read);

	img = readPNGIntoImage(png, pngInfo);

	return img;
}



//
////////////////////////////////////////////////////
//
// void writePNG()
//
////////////////////////////////////////////////////
//
void PNGCodec::writePNG(const char *filename, Image& img)
{
	FILE *pngFP;
	png_structp png;
	png_infop pngInfo;

#ifndef _NO_ERR_CHECK
	if (filename == NULL)
		throw new std::runtime_error("PNGCodec::writePNG(): filename must not be NULL");
#endif

	if ((pngFP = fopen(filename, "wb")) == NULL)
		throw new std::runtime_error("PNGCodec::writePNG(): file could not be opened");


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

	if (pngFP == NULL)
		throw new std::runtime_error("PNGCodec::initPNGStructs(): pngFP must not be NULL");
	


	if (bForReading)
		png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
	else
		png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
	
	if (png == NULL) {
		fclose(pngFP);
		throw new std::runtime_error("PNGCodec::initPNGStructs(): Failed to create PNG struct");
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

		throw new std::runtime_error("PNGCodec::initPNGStructs(): Failed to create PNG info struct");
	}


   // Set up the input control if using the standard C streams
	png_init_io(png, pngFP);
}




//
////////////////////////////////////////////////////
//
// ImageRGB24* readPNGIntoImage()
//
////////////////////////////////////////////////////
//
Image* PNGCodec::readPNGIntoImage(png_structp &png, png_infop &pngInfo)
{
	unsigned int y;
	png_uint_32 iWidth, iHeight, iRowLength;
	png_byte iColourType, iBitsPerChannel, iNumChannels;
	png_bytepp pngRows;

	Image* img;

	// For colour images: png_read_png(png, pngInfo, PNG_TRANSFORM_BGR | PNG_TRANSFORM_STRIP_ALPHA, png_voidp_NULL);
	png_read_png(png, pngInfo, PNG_TRANSFORM_IDENTITY, png_voidp_NULL);
	
	// pixels are in info_ptr->row_pointers
	// where row_pointers is:
	//   png_bytep row_pointers[height];
	// and is probably not contiguous

	iColourType = png_get_color_type(png, pngInfo);
	// For 32-bit colour:	if (iColourType != PNG_COLOR_TYPE_RGB_ALPHA)
	// For 24-bit colour:	if (iColourType != PNG_COLOR_TYPE_RGB)
	if (iColourType != PNG_COLOR_TYPE_GRAY)
		throw new std::runtime_error("PNGCodec::readPNGIntoImage: Colour type not supported - greyscale only");



	iBitsPerChannel = png_get_bit_depth(png, pngInfo);
	iNumChannels = png_get_channels(png, pngInfo);
	if (iBitsPerChannel != 8)
		throw new std::runtime_error("PNGCodec::readPNGIntoImage(): Image format not supported - greyscale 8-bit only");

	iHeight = png_get_image_height(png, pngInfo);
	iWidth = png_get_image_width(png, pngInfo);

	iRowLength = iWidth * (iBitsPerChannel * iNumChannels) / 8; // Should be same as iWidth as we are greyscale

	img = new Image(iWidth, iHeight, 1);
    pngRows = png_get_rows(png, pngInfo);
	for (int y = 0; y < iHeight; y++) {
		memcpy(img->data[y], pngRows[y], iRowLength);
	}
			

	// Clean up
	png_destroy_read_struct(&png, &pngInfo, png_infopp_NULL);

	return img;
}







//
////////////////////////////////////////////////////
//
// void writeImageIntoPNG(unsigned char **imgBuf)
//
////////////////////////////////////////////////////
//
void PNGCodec::writeImageToPNG(Image& img, png_structp& png, png_infop& pngInfo)
{
	unsigned int y;
	png_uint_32 iWidth, iHeight;
	unsigned char** imgBuf;
	png_bytepp pngRows;

	iWidth = img.width;
	iHeight = img.height;

	// For colour images png_set_IHDR(png, pngInfo, iWidth, iHeight, 8, PNG_COLOR_TYPE_RGB,
	//			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_set_IHDR(png, pngInfo, iWidth, iHeight, 8, PNG_COLOR_TYPE_GRAY,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);


	png_write_info(png, pngInfo);


	/* flip BGR pixels to RGB */
	png_set_bgr(png);


	pngRows = new png_bytep[iHeight];

	// 'Cheat' a little for efficiency - don't actually copy into pngRows, but
	// instead let it re-use imgBuf's rows: as the write to PNG won't change the
	// row data, imgBuf won't be changed anyways.
	for (y = 0; y < iHeight; y++)
		pngRows[y] = img.data[y];

	png_write_image(png, pngRows);

	png_write_end(png, pngInfo);

	// Clean up
	delete [] pngRows;
	png_destroy_write_struct(&png, &pngInfo);

}




