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

class Frame3DPoint
{
public:
    Frame3DPoint();
    ~Frame3DPoint();

private:
    int Pos;
};

#endif // FRAME_3DPOINT_H
