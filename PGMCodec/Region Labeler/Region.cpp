#include "Region.h"
#include <math.h>
#define NULL 0 //This source is Null-ified :D Get it? Get it?!

				//..... YOU GUYS SUCK ;_;


Region::Region() //Default Contstructor. Set all the things to nothings.
{
	startx=0;
	starty=0;
	totalSize=0;
	regionMap=NULL;
	imageMap=NULL;
	propThres=0;
}

Region::Region(unsigned char ** inMag,int X, int Y,int inMaxX,int inMaxY, int Threshold) //Not so default constructor. Set Image to this thing, allocate the imagemap
{
	startx=0;
	starty=0;
	totalSize=0;
	regionMap=NULL;
	imageMap=inMag;
	propThres=0;

	SetPropThres(Threshold);
	SetMaxCoords(inMaxX,inMaxY);
	regionMap=AllocRegionMapArea();
	SetStartCoords(X,Y);
	Propogate(startx,starty);
	IsolateTrueStart();
}

Region::~Region() //Blows up the region map. Try not to nuke the Image Map, miiight be being used by other parts of the program.
{
	for(int i=0;i<maxY;i++)
	{
		delete regionMap[i];
	}
	delete regionMap;
}

void Region::SetImageMap(unsigned char** inMag)
{
	imageMap = inMag;
}

int ** Region::GetRegionMap() //Now I know what you're thinking "Woah, Red, won't this mean that destroying the object will destroy the region map?" Let me answer that question, with another question: Is your fly down?
{
	return regionMap;
}

void Region::SetStartCoords(int inX, int inY)
{
	startx=inX;
	starty=inY;
}

void Region::SetMaxCoords(int inX, int inY)
{
	maxX=inX;
	maxY=inY;
}

void Region::GetStartCoords(int * inXPoint, int * inYPoint) //Need data in a jiffy? Look no further than gross memory mismanagement! Efficiency trumps sanity ANY and EVERY day of the week. :D
{
	*inXPoint=startx;
	*inYPoint=starty;
}

void Region::GoProp() // Now assuming you *SET EVERYTHING*.... this will not SegFault the crap out of your computer. Have fun :D
{
	regionMap= AllocRegionMapArea();
	Propogate(startx,starty);
	IsolateTrueStart();
}

void Region::Propogate(int X, int Y)
{
	if(regionMap[Y][X]==0)
	{
		totalSize++;
		regionMap[Y][X]=1;

		//and now the horrible part
		for(int i=-1;i<2;i++)
		{
			for(int j=-1;j<2;j++)
			{
				if(!(Y+i<0 || X+j<0 || Y+i > maxY || X+j > maxX))
				{
					if(abs(imageMap[Y+i][X+j]-imageMap[Y][X])<propThres && imageMap[Y+i][X+j]==0)
					{
						Propogate(X+j,Y+i);
					}else if(imageMap[Y+i][X+j]==0)
					{
						imageMap[Y+i][X+j]=-1;
					}
				}
			}
		}
	}
}