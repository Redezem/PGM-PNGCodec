#include <istream>
#include <string>
#include <vector>


#include "Region.h"
#include "../PGMCodec/PGMCodec.h"

#define THRESHOLD 15

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
	regionStack.AddRegion(new Region(inMag, 0, 0, masterCodec->getWidth()-1, masterCodec->getHeight()-1, THRESHOLD));
	for(int i=0; i<masterCodec.getWidth()-1; i++)
	{
		for(int j=0; j<masterCodec.getHeight()-1; j++)
		{
			
		}
	}
}