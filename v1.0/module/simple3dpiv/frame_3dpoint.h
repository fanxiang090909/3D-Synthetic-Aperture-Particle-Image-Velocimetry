#ifndef FRAME_3DPOINT_H
#define FRAME_3DPOINT_H

struct Temp3DPoint
{
    int xpos;
    int ypos;
    int zpos;
    int val;//ª“∂»¥Û–°
    Temp3DPoint * next;
};

struct Grid3DPoint
{
    int xpos;
    int ypos;
    int zpos;
    bool hasCompute;
};

class Frame3DPoint
{
public:
    Frame3DPoint(int x, int y, int z);
    ~Frame3DPoint();

private:
    int xPos;
    int yPos;
    int zPos;
};

#endif // FRAME_3DPOINT_H
