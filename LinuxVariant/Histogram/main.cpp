#include <PGMCodec.h>

#include <iostream>
#include <string>
#define THRESHOLD 10

int main(int argc, char**argv)
{
	PGMCodec* MahCodec;

	std::string inputString;
	char* inputCharArr;
	unsigned char** inMag, **outMag;
	int bottomNum, topNum,*bucketedarray,shuffledist;


	std::cout << "Input file to equalise: ";
	std::getline(std::cin,inputString);

	inputCharArr=new char[inputString.size()+1];
	std::copy(inputString.begin(),inputString.end(),inputCharArr);
	inputCharArr[inputString.size()]='\0';

	MahCodec = new PGMCodec(inputCharArr);
	inMag=MahCodec->getImageMatrix();

	bucketedarray = new int[255];
	for(int i=0; i<255; i++)
	{
		bucketedarray[i]=0;
	}
	for(int i=0; i<MahCodec->getHeight();i++)
	{
		for(int j=0; j<MahCodec->getWidth();j++)
		{
			bucketedarray[inMag[i][j]]++;
		}
	}

	outMag = new unsigned char*[MahCodec->getHeight()];
	for(int i=0; i<MahCodec->getHeight();i++)
	{
		outMag[i]=new unsigned char[MahCodec->getWidth()];
	}

	for(int i=0; i<255; i++)
	{
		if(bucketedarray[i]>THRESHOLD)
		{
			bottomNum=i;
			break;
		}
	}
	for(int i=255; i!=0; i--)
	{
		if(bucketedarray[i] > THRESHOLD)
		{
			topNum=i;
			break;
		}
	}

	for(int i=0; i<MahCodec->getHeight();i++)
	{
		for(int j=0; j<MahCodec->getWidth();j++)
		{
			outMag[i][j]=((inMag[i][j]-bottomNum)*(255/topNum));
		}
	}

	MahCodec->setImageMatrix(outMag);

	MahCodec->writeAsPGM("out.pgm");

	return 0;
}