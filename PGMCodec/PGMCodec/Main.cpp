#include <codeforge\PGMCodec.h>
#include <iostream>
#include <string>
#define THRESHOLD 50

int GetInput(int*,int*);

int main(int argc,char** argv)
{
	PGMCodec* MahCodec;
	std::string fileName;
	char* filenameArr;
	int** convoMat,**convoMat2, inWidth, inHeight,i,correctFlag,j;
	unsigned char **inMag, **outMag;
	
	std::cout << "Input file name: ";
	std::getline(std::cin,fileName);

	filenameArr=new char[fileName.size()+1];
	std::copy(fileName.begin(),fileName.end(),filenameArr);
	filenameArr[fileName.size()]='\0';

	MahCodec=new PGMCodec(filenameArr);

	correctFlag=0;

	while(correctFlag == 0)
	{
		correctFlag = GetInput(&inHeight,&inWidth);
	}

	convoMat=new int*[inHeight];
	convoMat2 = new int*[inHeight]; //This is a 90* rotation of convoMat. Booooom!
	for(i=0; i<inHeight; i++)
	{
		convoMat[i]=new int[inWidth]; //Was that really that hard?
		convoMat2[i]=new int[inWidth];
	}

	for(i=0;i<inHeight;i++)
	{
		for(j=0;j<inWidth;j++)
		{
			std::cout << "Enter cell " << j << ", " << i << ": ";
			std::cin >> convoMat[i][j];
		}
	}
	for(i=0;i<inHeight;i++)
	{
		for(j=0;j<inWidth;j++)
		{
			convoMat2[i][j]=convoMat[j][i]; //rotate right here
		}
	}
	for(i=0;i<inHeight;i++)
	{
		int* tempPointer;
		for(j=inHeight;j>0;j--)
		{
			tempPointer=convoMat2[j-1];
			convoMat2[j-1]=convoMat2[i];
			convoMat2[i]=tempPointer; //Tricky pointer disco!
		}
	}

	inMag=MahCodec->getImageMatrix();
	
	outMag = new unsigned char*[MahCodec->getHeight()];
	for(i=0; i<MahCodec->getHeight();i++)
	{
		outMag[i]=new unsigned char[MahCodec->getWidth()];
	}

	for(int a=0; a<MahCodec->getHeight(); a++)
	{
		for(int b=0; b<MahCodec->getWidth(); b++)
		{
			int result=0, result2=0;
			if(inHeight==1)
			{
				result = convoMat[0][0]*inMag[a][b];
				result2 = convoMat2[0][0]*inMag[a][b];
			}
			else if(inHeight==2)
			{
				if(!(a>MahCodec->getHeight()-2 || b>MahCodec->getWidth()-2))
				{
					result = (convoMat[0][0]*inMag[a][b])+(convoMat[0][1]*inMag[a][b+1])+(convoMat[1][0]*inMag[a+1][b])+(convoMat[1][1]*inMag[a+1][b+1]);
					result2 = (convoMat2[0][0]*inMag[a][b])+(convoMat2[0][1]*inMag[a][b+1])+(convoMat2[1][0]*inMag[a+1][b])+(convoMat2[1][1]*inMag[a+1][b+1]);

				}

			}
			else if(inHeight==3)
			{
				if(!(a>MahCodec->getHeight()-2 || b>MahCodec->getWidth()-2 || a==0 || b==0))
				{
					result = (convoMat[0][0]*inMag[a-1][b-1])+(convoMat[0][1]*inMag[a-1][b])+(convoMat[0][2]*inMag[a-1][b+1])+(convoMat[1][0]*inMag[a][b-1])+(convoMat[1][1]*inMag[a][b])+(convoMat[1][2]*inMag[a][b+1])+(convoMat[2][0]*inMag[a+1][b-1])+(convoMat[2][1]*inMag[a+1][b])+(convoMat[2][2]*inMag[a+1][b+1]);
					result2 = (convoMat2[0][0]*inMag[a-1][b-1])+(convoMat2[0][1]*inMag[a-1][b])+(convoMat2[0][2]*inMag[a-1][b+1])+(convoMat2[1][0]*inMag[a][b-1])+(convoMat2[1][1]*inMag[a][b])+(convoMat2[1][2]*inMag[a][b+1])+(convoMat2[2][0]*inMag[a+1][b-1])+(convoMat2[2][1]*inMag[a+1][b])+(convoMat2[2][2]*inMag[a+1][b+1]);
				}
			}
			result = result+result2;
			result=abs(result);
			if(result > THRESHOLD)
			{
				result = 255;
			}else
			{
				result = 0;
			}
			//Ok so now in theory, we're all good.
			outMag[a][b]=result;
		}
	}
	MahCodec->setImageMatrix(outMag);
	MahCodec->writeAsPGM("out.pgm");
	std::cout << "Image Manipulation Complete\n";
	for(i=0; i<inHeight; i++)
	{
		delete convoMat2[i];
		delete convoMat[i]; //Was that really that hard?
	}
	delete convoMat2;
	delete convoMat;
	return 0;
}

int GetInput(int* outHeight, int* outWidth)
{
	int inHeight, inWidth;
	std::cout << "Input Size of Convolution Matrix: ";
	std::cin >> inHeight;
	inWidth = inHeight;

	*outWidth=inWidth;
	*outHeight=inHeight;

	if(inHeight < 4 && inWidth < 4 && inHeight > 0 && inWidth > 0)
	{
		return 1;
	}else{
		std::cout << "Dimensions can only be a maximum of 3, min of 1" << "\n";
		return 0;
	}
}
