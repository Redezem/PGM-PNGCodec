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

void Region::IsolateTrueStart()
{
	for(int i=0;i<maxY;i++)
	{
		for(int j=0;i<maxX;j++)
		{
			if(regionMap[i][j]==1)
			{
				starty=i;
				startx=j;
				return; //Sooooo close to using a GoTo. PRAISE RAPTOR JESUS FOR HIS INSIGHT
			}
		}
	}
}

int Region::FindSize()
{
	totalSize=0;
	for(int i=0;i<maxY;i++)
	{
		for(int j=0;i<maxX;j++)
		{
			if(regionMap[i][j]==1)
			{
				totalSize++;
			}
		}
	}
	return totalSize;
}

int** Region::AllocRegionMapArea()
{
	int** newRegio;
	newRegio = new int[maxY];
	for(int i=0;i<maxY;i++)
	{
		newRegio[i]=new int[maxX];
	}
	return newRegio;
}

//---------------Now for the Stack----------------------

RegionStack::RegionStack()
{
	//Do something... I guess...
}

RegionStack::~RegionStack()
{
	//Okay, so, nuke them vector elements
	while(regionVector.size()>0)
	{
		delete regionVector.last();
		regionVector.pop_last();
	}
}

void RegionStack::AddRegion(Region* inReg)
{
	regionVector.push_last(inReg);
}

void RegionStack::DelRegion(Region* inReg)
{
	for (std::vector::iterator i = regionVector.begin(); i != regionVector.end(); ++i)
	{
		if(*i == inReg)
		{
			regionVector.erase(i);
			return; //What? It's not like the same region will be pointed at twice *directly*...
		}
	}
}

void RegionStack::RemoveDuplicates()
{
	for (std::vector::iterator i = regionVector.begin(); i != regionVector.end(); ++i)
	{
		for (std::vector::iterator j = regionVector.begin(); i != regionVector.end(); ++i)
		{
			if (*i != *j && **i.GetStartCoords() == **j.GetStartCoords())
			{
				regionVector.erase(j);
			}
		}
	}
}

Region* RegionStack::GetRegion( int i )
{
	return regionVector[i];
}

int RegionStack::GetStackSize()
{
	return regionVector.size();
}
