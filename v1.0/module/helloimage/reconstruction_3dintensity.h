#ifndef RECONSTRUCTION_3DINTENSITY_H
#define RECONSTRUCTION_3DINTENSITY_H

#include <vector>
#include <list>

#include <string>

#include "reconstruction_3dintensity.h"
#include "frame_3dpoint.h"

using namespace std;

class Reconstruction3DIntensity
{
protected:
    int height;
    int width;
    int depth;

    Temp3DPoint * tempP;

    std::list<Temp3DPoint*> temp3DPoints;
    std::list<Temp3DPoint*>::iterator piterator;
    std::list<Temp3DPoint*>::iterator insertIter;

    std::vector<string> files;
    string path;

    string resultFilename;

public:
    Reconstruction3DIntensity();
    ~Reconstruction3DIntensity();

    virtual void RawImagesProcess() = 0;

    virtual void loadImageFiles(std::vector<string> *filename, string path);

    virtual std::list<Temp3DPoint*> getCurrent3DPointsList();

    virtual void initail3dPointsResultFile(string resultFile);

    virtual void savePointsToFile(string saveFile);

    virtual long index(int x, int y, int z);

protected:

    virtual int getNextPiteratorLastYpos(Temp3DPoint* &tempP, int tempPointX , int lastYPos, int lastZPos);

    virtual bool insertToList(int x, int y, int z, int val);

    virtual bool deleteRedundant(std::list<Temp3DPoint*>::iterator deletePoint, bool deleteFromList);
};

#endif // RECONSTRUCTION_3DINTENSITY_H
