#include <istream>
#include <string>
#include <vector>


#include "Region.h"
#include "../PGMCodec/PGMCodec.h"

#define THRESHOLD 15

unsigned char ** GetRegionImage(Region*, int x, int y);

int main(int argc,char**argv)
{
	char* fileName;
	unsigned char** inMag,**outMag;
	RegionStack regionStack;

	unsigned char currChar;

	if(argc>1)
	{
		fileName=argv[1];
	}else{
	std::string inputString;

	std::cout << "Input file to process: ";
	std::getline(std::cin,inputString);

	fileName=new char[inputString.size()+1];
	std::copy(inputString.begin(),inputString.end(),fileName);
	fileName[inputString.size()]='\0';
	}
	
	PGMCodec* masterCodec;
	masterCodec = new PGMCodec(fileName);

	inMag = masterCodec->getImageMatrix();
	currChar = inMag[0][0];
	std::cout << "adding first region.../n";
	regionStack.AddRegion(new Region(inMag, 0, 0, masterCodec->getWidth()-1, masterCodec->getHeight()-1, THRESHOLD));
	for(int i=0; i<masterCodec.getHeight()-1; i++)
	{
		for(int j=0; j<masterCodec.getWidth()-1; j++)
		{
			if(abs(inMag[i][j]-currChar)>THRESHOLD)
			{
				std::cout << "found new region at " << j << ", " << i << "/n";
				regionStack.AddRegion(new Region(inMag,j,i,masterCodec->getWidth()-1,masterCodec->getHeight()-1,THRESHOLD))
			}
		}
	}

	regionStack.RemoveDuplicates();

	for(int i = 0; i < regionStack.StackSize(); i++)
	{
		outMag = GetRegionImage(regionStack.GetRegion(i),masterCodec.getWidth(),masterCodec.getHeight());
		masterCodec->setImageMatrix(outMag);
		masterCodec->writeAsPGM()
	}
}

unsigned char ** GetRegionImage(Region* inReg, int x, int y)
{
	unsigned char ** imageField;

	imageField = new unsigned char*[y];
	for(int i = 0; i < y; i++)
	{
		imageField[i]=new unsigned char[x];
	}

	for(int i = 0; i<y; i++)
	{
		for(int j = 0; j<x; j++)
		{
			if(inReg->GetRegionMap()[i][j]==1)
			{
				imageField[i][j]=0;
			}
			else
			{
				imageField[i][j]=255;
			}
		}
	}

	return imageField;
}
