
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


#include <Image.h>


#ifdef USE_LIBTRACKER_NAMESPACE
namespace libTracker {
#endif



//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// STATIC MEMBERS
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
uint8 Image::RGBtoGrey(RGB24 rgb)
{
	// Convert to Grey, rounding as we do
	return (uint8) ( ((double)rgb.red * 0.3) + ((double)rgb.green * 0.59) + 
	                ((double)rgb.blue * 0.11) + 0.4999 );
}


//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
uint8 Image::RGBtoGrey(uint8 red, uint8 green, uint8 blue)
{
	// Convert to Grey, rounding as we do
	return (uint8) ( ((double)red * 0.3) + ((double)green * 0.59) + 
	                ((double)blue * 0.11) + 0.4999 );
}


//
////////////////////////////////////////////////////
//
//
// Purpose:
//  Convert from the RGB cube colourspace to the HSL double-hexcone colourspace
//  Note that there are alternatives to HSL, such as the HSV hexcone.
//  HSL's main problem is that with a very high/low brightness L, H and S 
//  are irrelevant, and for a very low S, H is irrelevant. Hence it tends
//  to lose information if you consider H and S as numeric values in the
//  range 0..255 (which we do for convenience/compatibility).
//  HSV (single-hexcone) has less problems with this, but only just.
//
////////////////////////////////////////////////////
//
HSV24 Image::RGBtoHSV(RGB24 rgb)
{
	HSV24 hsv;

	RGBtoHSV(rgb.red, rgb.green, rgb.blue, hsv.hue, hsv.sat, hsv.val);
	return hsv;
}


void Image::RGBtoHSV(uint8 red, uint8 green, uint8 blue, 
					uint8 &hue, uint8 &sat, uint8 &val)
{
	double fHue;
	uint8 iMin, iMax;
	double fDiff;

	iMin = MIN(red, green, blue);
	iMax = MAX(red, green, blue);

	val = iMax;
	fDiff = iMax - iMin;

	if (iMax != 0) {
		sat = (uint8) (fDiff / (float)iMax * 255.0);
	}
	else {
		// red = green = blue = 0		// sat = 0, hue is undefined
		sat = 0;
		val = 0;
		hue = 0;
		return;
	}

	if (red == iMax)
		fHue = (float)(green - blue) / fDiff;		// between yellow & magenta
	else if (green == iMax)
		fHue = 2 + ( (float)(blue - red) / fDiff );	// between cyan & yellow
	else
		fHue = 4 + ( (float)(red - green) / fDiff ); // between magenta & cyan

	if (fHue < 0)
		fHue += 6.0;	// Get in range [0..6]

	hue = (uint8) ((fHue / 6.0) * 255.0);
}



/*

  HSL variant:

void Image::RGBtoHSL(uint8 red, uint8 green, uint8 blue, 
					uint8 &hue, uint8 &sat, uint8 &val)
{
	int iMin, iMax;
	double fHue, fVal, fSat, fDiff;

	
	iMin = MIN(red, green, blue);
	iMax = MAX(red, green, blue);

	fDiff = iMax - iMin;


	if (iMax == iMin) {
		// Achromatic (grey-scale)
		fVal = iMax;
		fHue = 0;
		fSat = 0;
	}
	else {
		// Chromatic (colours)
		//
		// Calculate Value (brightness)
		//
		// Brightness is derived only from min and max
		// - see Color Theory and Modeling for Computer Graphics..., page 66.
		// (the GHLS model, where Wmin = Wmax = 0.5 and Wmid = 0
		//  This is the HLS - or HSV - double-hexcone model).
		fVal = ((double)(iMax + iMin)) * 0.5;

		
		//
		// Calculate Saturation
		//
		if (fVal < 128)
			fSat = (fVal - iMin) / fVal;
		else
			fSat = (iMax - fVal) / (255.0 - fVal);


		//
		// Calculate Hue
		//
		if(red == iMax)
			fHue = (float)(green - blue) / fDiff;		// between yellow & magenta
		else if(green == iMax)
			fHue = 2 + (float)(blue - red) / fDiff;		// between cyan & yellow
		else
			fHue = 4 + (float)(red - green) / fDiff;		// between magenta & cyan

		fHue = fHue / 6.0;				// Convert to range 0..1
		if(fHue < 0)
			fHue += 1.0;

	}	 // if (iMax == iMin)

	// Convert h and s to 0-255, rounding while we do so.
	hue = (uint8)((fHue * 255.0) + 0.5);
	sat = (uint8)((fSat * 255.0) + 0.5);
	val = (uint8)(fVal + 0.5);

}
*/


//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
RGB24 Image::HSVtoRGB(HSV24 hsv)
{
	RGB24 rgb;

	HSVtoRGB(hsv.hue, hsv.sat, hsv.val, rgb.red, rgb.green, rgb.blue);
	return rgb;
}


void Image::HSVtoRGB(uint8 hue, uint8 sat, uint8 val, 
					uint8 &red, uint8 &green, uint8 &blue)
{
	int iSector;
	double f, fHue, fSat, fVal;
	uint8 p, q, t;

	if (sat == 0)  {
		//
		// Achromatic (grey)
		//
		red = green = blue = val;
	}
	else {
		//
		// Chromatic
		//
		// Perform conversions for use in algorithm
		// The algorithm expects:
		// - Hue E (0..360)
		// - Val, Sat E (0..1)
		//
		fHue = (float)hue / 255.0 * 360.0;
		fSat = (float)sat / 255.0;
		fVal = (float)val / 255.0;

		if (fHue == 360)
			fHue = 0.0;
		iSector = (int)(fHue / 60.0);			// sector 0 to 5
		f = ((fHue/60.0) - iSector);			// factorial part of fHue
		
		// Don't ask me what 'p', 'q' and 't' are supposed to be - I just work here.
		p = (uint8) ( (fVal * (1 - fSat)) * 255.0 );
		q = (uint8) ( (fVal * (1 - fSat * f)) * 255.0 );
		t = (uint8) ( (fVal * (1 - fSat * (1 - f))) * 255.0 );

		switch (iSector) {
			case 0: red = val;
					green = t;
					blue = p;
					break;
			case 1:	red = q;
					green = val;
					blue = p;
					break;
			case 2:	red = p;
					green = val;
					blue = t;
					break;
			case 3:	red = p;
					green = q;
					blue = val;
					break;
			case 4:	red = t;
					green = p;
					blue = val;
					break;
			case 5: red = val;
					green = p;
					blue = q;
					break;
			default:
				throw new NumberOutOfRangeException("Image::HSVtoRGB()", "iSector", 0, 5);
		}
	}
}

/*

HSL variant

void Image::HSLtoRGB(uint8 hue, uint8 sat, uint8 val, 
					uint8 &red, uint8 &green, uint8 &blue)
{
	int iMin, iMid, iMax;
	double fMin, fMid, fMax;
	double fHue, fVal, fSat, f;
	int iSector;

	if (sat == 0) {
		//
		// Achromatic (grey scale)
		//
		red = green = blue = val;
	}
	else {

		//
		// Chromatic
		//
		// Perform conversions for use in algorithm
		// The algorithm expects:
		// - Hue E (0..360)
		// - Val, Sat E (0..1)
		//
		fHue = (float)hue / 255.0 * 360.0;
		fVal = (float)val / 255.0;
		fSat = (float)sat / 255.0;


		iSector = (int)(fHue / 60.0);
		f = (fHue / 60.0) - iSector;

		if ((iSector % 2) != 0)
			f = (1 - f);
	
		if (fVal < 0.5) {
			fMin = (1 - fSat) * fVal;
			fMid = (f * fVal + fMin * ((1 - f) * 0.5 - f * 0.5)) / 0.5;
			fMax = (fVal - (0.5 * fMin)) * 2.0;
		}
		else {
			fMax = fSat + ((1 - fSat) * fVal);
			fMid = ((1 - f) * fVal - fMax * ((1 - f) * 0.5 - (f * 0.5))) / 0.5;
			fMin = (fVal - (0.5 * fMax)) * 2.0;
		}

		// Convert from scale (0..1) to (0..255), rounding as we do
		iMin = (int)(fMin * 255.0 + 0.5);
		iMid = (int)(fMid * 255.0 + 0.5);
		iMax = (int)(fMax * 255.0 + 0.5);


		switch (iSector) {
			case 0: red = iMax;
					green = iMid;
					blue = iMin;
					break;
			case 1: red = iMid;
					green = iMax;
					blue = iMin;
					break;
			case 2: red = iMin;
					green = iMax;
					blue = iMid;
					break;
			case 3: red = iMin;
					green = iMid;
					blue = iMax;
					break;
			case 4: red = iMid;
					green = iMin;
					blue = iMax;
					break;
			default: red = iMax;
					green = iMin;
					blue = iMid;
					break;
		}

	}	// if (sat == 0) {
}
*/


//
////////////////////////////////////////////////////
//
// RGB24 generateColour()
//
// Purpose:
//  Generate unique colours in a sequence, making sure that each colour is
//  reasonably different from its neighbours.
//
// Algorithm:
//  The colours are generated via a triply-nested for loop, with each loop
//  corresponding to hue, sat and val respectively. Each iteration of a loop
//  steps the relevant channel forward by a significant amount so that
//  the next colour is significantly different from the previous colour.
//  A loop is cancelled whenever the channel 'clocks over', the current value
//  of the channel is saved for later looping and the outer
//  loop is incremented by one step. 
//  The Stepping amounts must be chosen carefully so that the channel can
//  clock over without repeating values (otherwise we'll start to generate
//  the same colour more than once!). This means that even numbers are
//  unacceptable if the cycle is even. Furthermore, you want to clock
//  over by about STEP/2 so that the first value of the next cycle is 
//  about halfway between the first and second values of the last cycle.
//
//  Since we are being called for each seqNum, we don't want to do this 
//  with for loops since that would be very slow (this function is designed 
//  to be called multiple times with different, probably consecutive, 
//  sequence numbers).
//  Instead, we do calculations that give us what the for loops WOULD have 
//  gotten us if we had done for loops. See bottom of function to see
//  what the looping method would look like - it's easier to understand.
// 
//
////////////////////////////////////////////////////
//
RGB24 Image::generateColour(int seqNum)
{

	RGB24 rgb;
	HSV24 hsv;
	double fNumHuesPerCycle, fNumSatsPerCycle, fNumValsPerCycle;
	int iHueNumIters, iSatNumIters, iValNumIters;
	int iHue, iSat, iVal;

//
// NOTE: We only go in the range 64..255 for V since when V=0,
//       H and S don't matter (because HSV is a hexcone).
//       This is disastrous since it will mean that we get duplicate colours!
//       A similar situation exists for low Saturations.
//       Thus this function generates approx 9 million colours (256*192*192),
//       with the first 128152 guaranteed unique (I've checked this).
//       Theoretically, we should be able to get all 9million unique,
//       but HSV is a smaller colourspace than RGB, so you're bound
//       to end up with duplicates.
//
#define HUE_CYCLE 256
#define SAT_CYCLE 192
#define VAL_CYCLE 192
	

//
// Good choices for steps:
// Range 256: 19, 27, 39, 47, 73
// Range 192: 29, 35, 43, 55, 77
// Range 128: 17, 23, 29, 37, 51
//
	
#define HUE_STEP	39		// 6 colours per cycle (slow change, since hue is obvious)
#define SAT_STEP	55		// 3 sats per cycle (fast changing)
#define VAL_STEP	77		// 2 shades per cycle (v. fast changing)



	fNumHuesPerCycle = (float)HUE_CYCLE / (float)HUE_STEP;
	fNumSatsPerCycle = (float)SAT_CYCLE / (float)SAT_STEP;
	fNumValsPerCycle = (float)VAL_CYCLE / (float)VAL_STEP;

	// The loops are nested sat >> val >> hue, since hue and val
	// have the most noticeable effects on colour and thus 
	// should change most often.
	iHueNumIters = seqNum;
	iValNumIters = (int)((iHueNumIters - 1) / fNumHuesPerCycle) + 1;
	iSatNumIters = (int)((iValNumIters - 1) / fNumValsPerCycle) + 1;
	iHue = (iHueNumIters * HUE_STEP) % HUE_CYCLE;
	iVal = (iValNumIters * VAL_STEP) % VAL_CYCLE;
	iSat = (iSatNumIters * SAT_STEP) % SAT_CYCLE;


	// NOTE: We reduce sat and val from their starting maximums since
	// colours look best starting with high sat/val.
	hsv.hue = iHue;
	hsv.sat = 255 - iSat;	// Reduce saturation from 255 -> 0.
	hsv.val = 255 - iVal;	// Reduce value from 255 -> 64
	rgb = HSVtoRGB(hsv);

	return rgb;
/*
// Loop-based version:

	iHue = 0;
	iSat = 0;
	iVal = 0;

	int iValIters, iSatIters, iHueIters;
	int i;

	iValIters = 0;
	iSatIters = 0;
	iHueIters = 0;

	i = 0;
	while (i < seqNum) {
   
		iSat = iSat % SAT_CYCLE;
		while (iSat < SAT_CYCLE) {

			iVal = iVal % VAL_CYCLE;
			while (iVal < VAL_CYCLE) {
         
				iHue = iHue % HUE_CYCLE;
				while (iHue < HUE_CYCLE) {
                  
					if (i >= seqNum)
						break;
					iHue += HUE_STEP;
					iHueIters++;
					i++;
				}
         
				if (i >= seqNum)
					break;
				iVal += VAL_STEP;
				iValIters++;
			}
      
			if (i >= seqNum)
				break;
			iSat += SAT_STEP;
			iSatIters++;
		}
	}


	hsv.hue = iHue;
	hsv.sat = 255 - iSat;
	hsv.val = 255 - iVal;

	rgb = HSVtoRGB(hsv);
	return rgb;
*/
}



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
// Image()
//
// Purpose:
//  Blank default constructor to facilitate sub-classes
//
////////////////////////////////////////////////////
//
Image::Image()
{
#ifdef WIN32
	m_bInvert = TRUE;
	m_bHostInvertFlag = TRUE;
#else
	m_bInvert = FALSE;
	m_bHostInvertFlag = FALSE;
#endif

	m_iDrawRGB.red = m_iDrawRGB.green = m_iDrawRGB.blue = 255;
	m_iFillRGB = m_iDrawRGB;
	m_iTextRGB = m_iDrawRGB;

//	m_rgbaBuf = NULL;
}



//
////////////////////////////////////////////////////
//
// Image()
//
// Purpose:
//  Declared and set as protected and explicit to stop the compiler
//  from making it's own copy constructor and using it to coerce 
//  const Images to non-const Images when calling a function.
//  This way, the compiler is forced to tell you that it can't pass
//  a const Image as a non-const parameter rather than coming up with
//  obscure messages that it can't instantiate an Image because it is virtual!
//
////////////////////////////////////////////////////
//
Image::Image(const Image &img)
{
	throw new UnsupportedOperationException("Image::Image", "Copy constructor invalid for Image - this code is only here to stop the compiler from using it's own copy constructor");
}




//
////////////////////////////////////////////////////
//
// ~Image()
//
////////////////////////////////////////////////////
//
Image::~Image()
{
	// Nothing to do - this just exists to force the
	// compiler to call inherited class's destructors
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
// int width()
//
// Returns:
//  Image width
//
////////////////////////////////////////////////////
//
int Image::width() const
{
	return m_iWidth;
}



//
////////////////////////////////////////////////////
//
// int height()
//
// Returns:
//  Image height
//
////////////////////////////////////////////////////
//
int Image::height() const
{
	return m_iHeight;
}


//
////////////////////////////////////////////////////
//
// Bool isInverted()
//
// Returns:
//  Whether the image is inverted or not
//
////////////////////////////////////////////////////
//
Bool Image::isInverted() const
{
	return m_bInvert;
}



//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
void Image::setARGB(int x, int y, uint8 alpha, uint8 red, uint8 green, uint8 blue)
{
	RGB32 argb;	

	argb.alpha = alpha;
	argb.red = red;
	argb.green = green;
	argb.blue = blue;

	setARGB(x, y, argb);
}

void Image::setARGB(int x, int y, RGB32 argb)
{
	RGB24 currRGB;
	double fAlpha, fInvAlpha;

	if (argb.alpha == 255)
		// Fully opaque colour, so same as setRGB()
		setRGB(x, y, argb.red, argb.green, argb.blue);

	else if (argb.alpha != 0) {	// The 0 case means fully transparent
		currRGB = getRGB(x, y);

		// Apply transparency by reducing the currRGB in proportion to 255-alpha 
		// and adding the new value in proportion to alpha
		fAlpha = (float)argb.alpha / 255.0;
		fInvAlpha = 1.0 - fAlpha;

		currRGB.red = (uint8) ROUND( ((double)currRGB.red * fInvAlpha) + ((double)argb.red * fAlpha) + 0.001 );
		currRGB.green = (uint8) ROUND( ((double)currRGB.green * fInvAlpha) + ((double)argb.green * fAlpha) + 0.001 );
		currRGB.blue = (uint8) ROUND( ((double)currRGB.blue * fInvAlpha) + ((double)argb.blue * fAlpha) + 0.001 );

		// Update the RGB value
		setRGB(x, y, currRGB);
	}
	
}




//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
void Image::getHSV(int x, int y, uint8 &hue, uint8 &sat, uint8 &val) const
{
	HSV24 hsv;

	hsv = getHSV(x, y);

	hue = hsv.hue;
	sat = hsv.sat;
	val = hsv.val;
}

HSV24 Image::getHSV(int x, int y) const
{
	return RGBtoHSV(getRGB(x, y));
}



//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
uint8 Image::hue(int x, int y) const
{
	return getHSV(x, y).hue;
}

uint8 Image::sat(int x, int y) const
{
	return getHSV(x, y).sat;
}

uint8 Image::val(int x, int y) const
{
	return getHSV(x, y).val;
}




//
////////////////////////////////////////////////////
//
// void setDrawColour()
//
// Purpose:
//  Set the color to use for all subsequent drawing operations
//
// Parameters:
//  IN uint8 red - Red color
//  IN uint8 green - Green color
//  IN uint8 blue - Blue color
//
////////////////////////////////////////////////////
//
RGB24 Image::drawColour()
{
	return m_iDrawRGB;
}

void Image::setDrawColour(uint8 red, uint8 green, uint8 blue)
{
	m_iDrawRGB.red = red;
	m_iDrawRGB.green = green;
	m_iDrawRGB.blue = blue;
}


//
////////////////////////////////////////////////////
//
RGB24 Image::fillColour()
{
	return m_iFillRGB;
}

void Image::setFillColour(uint8 red, uint8 green, uint8 blue)
{
	m_iFillRGB.red = red;
	m_iFillRGB.green = green;
	m_iFillRGB.blue = blue;
}





//
////////////////////////////////////////////////////
//
// void drawRect()
//
// Purpose:
//  Draw a line on the image in the currently set color
//  and at the currently set width
//
// Parameters:
//  IN int left - Left of rect
//  IN int top - Bottom of rect
//  IN int right - Right of rect
//  IN int bottom - Bottom of rect
//  IN Bool bFill - Boolean indicating whether to fill in the rectangle
//
////////////////////////////////////////////////////
//
void Image::drawRect(Point2D topLeft, Point2D bottomRight, Bool bFill/*=FALSE*/)
{
	drawRect((int)topLeft.x, (int)topLeft.y, (int)bottomRight.x, (int)bottomRight.y, bFill);
}

void Image::drawRect(FloatRect rect, Bool bFill/*=FALSE*/)
{
	drawRect((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom, bFill);
}

void Image::drawRect(double left, double top, double right, double bottom, Bool bFill/*=FALSE*/)
{
	drawRect((int)left, (int)top, (int)right, (int)bottom, bFill);
}

	
void Image::drawRect(int left, int top, int right, int bottom, Bool bFill/*=FALSE*/)
{
	int x, y;
	int iMinX, iMinY, iMaxX, iMaxY;

	// Don't draw the box if it is totally out of the image!
	if ((right < 0) || (bottom < 0) || (left >= m_iWidth) || (top >= m_iHeight)) {
		return;
	}

	//
	// Force rectangle to be within image bounds - also, don't draw an
	// edge if it exists OUTSIDE of the image bounds
	//
	iMinX = MAX(left, 0);
	iMaxX = MIN(right, m_iWidth-1);
	iMinY = MAX(top, 0);
	iMaxY = MIN(bottom, m_iHeight-1);


	if (bFill) {
		//
		// Fill in the rectangle
		//
		for (x = iMinX+1; x < iMaxX; x++)
			for (y = iMinY+1; y < iMaxY; y++)
				setRGB(x, y, m_iFillRGB);
	}

	//
	// Outline the rectangle
	// This is also done for filling so that we have an edge
	// and a fill that can be a separate colour
	//
	for (y = iMinY; y <= iMaxY; y++)
		setRGB(iMinX, y, m_iDrawRGB);
	for (y = iMinY; y <= iMaxY; y++) 
		setRGB(iMaxX, y, m_iDrawRGB);
	for (x = iMinX; x <= iMaxX; x++)
		setRGB(x, iMinY, m_iDrawRGB);
	for (x = iMinX; x <= iMaxX; x++)
		setRGB(x, iMaxY, m_iDrawRGB);
}




//
////////////////////////////////////////////////////
//
// void drawLine()
//
// Purpose:
//  Draw a line on the image in the currently set color
//  and at the currently set width
//
// Parameters:
//  IN int x1
//  IN int y1 - (x1,y1) coordinates of the first point
//  IN int x2
//  IN int y2 - (x2,y2) coordinates of the second point
//
////////////////////////////////////////////////////
//
void Image::drawLine(Point2D startPt, Point2D endPt)
{
	drawLine((int)startPt.x, (int)startPt.y, (int)endPt.x, (int)endPt.y);
}


void Image::drawLine(double x1, double y1, double x2, double y2)
{
	drawLine((int)x1, (int)y1, (int)x2, (int)y2);
}
	
void Image::drawLine(int x1, int y1, int x2, int y2)
{
	int iDeltaX, iDeltaY;
	int xInc1, xInc2, yInc1, yInc2;
	int iNumer, iNumerAdd, iDenom;
	int iNumPixels;
	int x, y, i;


	//
	// Uses the Bresenham line drawing algorithm
	// Code from http://www.gamedev.net/reference/articles/article1275.asp
	//
	// Calculate the relative increments for x and y
	//
	iDeltaX = abs(x2 - x1);        
	iDeltaY = abs(y2 - y1);

	// Check if the x-values are increasing
	if (x2 >= x1) {
		xInc1 = 1;
		xInc2 = 1;
	}
	else {
      // The x-values are decreasing
		xInc1 = -1;
		xInc2 = -1;
	} 

	// Check if the y-values are increasing
	if (y2 >= y1) {
		yInc1 = 1;
		yInc2 = 1;
	}
	else {
		// The y-values are decreasing
		yInc1 = -1;
		yInc2 = -1;
	} 


	if (iDeltaX >= iDeltaY) {
		// There is at least one x-value for every y-value
		xInc1 = 0;   // Don't change the x when numerator >= denominator
		yInc2 = 0;   // Don't change the y for every iteration
		iDenom = iDeltaX;
		iNumer = iDeltaX / 2;
		iNumerAdd = iDeltaY;
		iNumPixels = iDeltaX;         // There are more x-values than y-values
	}
	else {
		// There is at least one y-value for every x-value
		xInc2 = 0;   // Don't change the x for every iteration
		yInc1 = 0;   // Don't change the y when numerator >= denominator
		iDenom = iDeltaY;
		iNumer = iDeltaY / 2;
		iNumerAdd = iDeltaX;
		iNumPixels = iDeltaY;         // There are more y-values than x-values
	} 


	//
	// Finally, draw the line
	//
	x = x1;
	y = y1;

	for (i = 0; i <= iNumPixels; i++) {

		//
		// Clip the line so that it is within the draw-able area of the image
		//
		if ((x >= 0) && (x < m_iWidth) && (y >= 0) && (y < m_iHeight))
			setRGB(x, y, m_iDrawRGB);

		iNumer += iNumerAdd; // Increase the numerator by the top of the fraction

		if (iNumer >= iDenom) {  // Check if numerator >= denominator
			iNumer -= iDenom;   // Calculate the new numerator value
			x += xInc1;
			y += yInc1;
		}
		
		x += xInc2;
		y += yInc2;
	}

}












//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
Image &Image::operator=(const Image &img)
{
	m_iWidth = img.m_iWidth;
	m_iHeight = img.m_iHeight;
	m_bInvert = img.m_bInvert;
	m_bHostInvertFlag = img.m_bHostInvertFlag;

	m_iDrawRGB = img.m_iDrawRGB;
	m_iFillRGB = img.m_iFillRGB;
	m_iTextRGB = img.m_iTextRGB;
	
	return *this;
}







//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
int Image::numForeground() const
{
	return (m_iWidth * m_iHeight) - numBackground();
}

int Image::numBackground() const
{
	int x, y;
	int iNumBGPixels;

	iNumBGPixels = 0;
	for (y = 0; y < m_iHeight; y++) {
		for (x = 0; x < m_iWidth; x++) {
			if (isBackground(x, y)) 
				iNumBGPixels++;
		}
	}

	return iNumBGPixels;
}


//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
void Image::normalise(Bool bSquareMethod/*=FALSE*/)
{
	int x, y;
	int iWidth, iHeight;
	RGB24 pixel, normPix;
	float fRGBTot;

	iWidth = width();
	iHeight = height();

	for (y = 0; y < iHeight; y++) {
		for (x = 0; x < iWidth; x++) {
			pixel = getRGB(x,y);
			if (bSquareMethod)
				fRGBTot = sqrt((float)(pixel.red*pixel.red + pixel.green*pixel.green + pixel.blue*pixel.blue));
			else
				fRGBTot = pixel.red + pixel.green + pixel.blue;
			normPix.red = (uint8)( (float)pixel.red / fRGBTot * 255.0 );
			normPix.green = (uint8)( (float)pixel.green / fRGBTot * 255.0 );
			normPix.blue = (uint8)( (float)pixel.blue / fRGBTot * 255.0 );
			setRGB(x, y, normPix);
		}
	}
}


//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
void Image::mask(const Image &imgMask)
{
	int x, y;

	//
	// Set any (0,0,0) pixels in the mask to (0,0,0) in this image
	// This effectively treats imgMask as a binary image
	//
	for (y = 0; y < m_iHeight; y++) {
		for (x = 0; x < m_iWidth; x++) {
			if (imgMask.isBackground(x, y))
				setGrey(x, y, 0);
		}
	}
	
}


//
////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
//
void Image::threshold(double threshold, Bool bMakeBinary/*=TRUE*/)
{
	int x, y;

	for (y = 0; y < m_iHeight; y++) {
		for (x = 0; x < m_iWidth; x++) {
			if (grey(x, y) < threshold) 
				setGrey(x, y, 0);
			else {
				if (bMakeBinary) 
					setGrey(x, y, 255);
			}
		}	
	}
}
















#ifdef USE_LIBTRACKER_NAMESPACE
}
#endif


