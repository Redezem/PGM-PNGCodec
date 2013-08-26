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
	std::vector<Region> regionStack;

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

	inMag = masterCodec.getImageMatrix();
}