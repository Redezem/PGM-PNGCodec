class Region
{
public:
	Region();
	Region(unsigned char **,int,int,int,int,int);
	~Region();

	void SetImageMap(unsigned char **);
	int ** GetRegionMap();
	void SetPropThres(int);
	int GetTotalSize();
	void SetPropThres(int);
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