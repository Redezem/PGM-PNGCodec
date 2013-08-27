#pragma once
#include <vector>
class Region
{
public:
	Region();
	Region(unsigned char ** inMag,int X, int Y,int inMaxX,int inMaxY, int Threshold);
	~Region();

	void SetImageMap(unsigned char **);
	int ** GetRegionMap();
	void SetPropThres(int);
	int GetTotalSize();
	void SetStartCoords(int, int);
	void SetMaxCoords(int,int);
	void GetStartCoords(int*, int*);
	void GoProp(); //GO PROP! GO! <thrash guitar solo>
private:
	unsigned char ** imageMap;
	int ** regionMap;
	int startx,starty,propThres, totalSize,maxX, maxY;
	

	void Propogate(int,int);
	void IsolateTrueStart();
	int FindSize();
	int ** AllocRegionMapArea();
};

class RegionStack
{
public:
	RegionStack();
	~RegionStack();

	void AddRegion(Region*);
	void DelRegion(Region*);
	void RemoveDuplicates();
	Region* GetRegion(int);
	int GetStackSize();
private:
	std::vector<Region*> regionVector;
};