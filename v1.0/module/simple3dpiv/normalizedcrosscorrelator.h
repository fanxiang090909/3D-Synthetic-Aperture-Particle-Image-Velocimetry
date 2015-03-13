#ifndef NORMALIZEDCROSSCORRELATE_H
#define NORMALIZEDCROSSCORRELATE_H

#include <list>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "frame_3dpoint.h"
#include "pivengine.h"
#include "settings.h"

class NormalizedCrossCorrelate : public PivEngine
{
public:
    NormalizedCrossCorrelate(Settings * settingsPass, std::list<Grid3DPoint*>* gridPass);

    virtual ~NormalizedCrossCorrelate();

protected:
    //bool crossCorrelate(int topLeftRow, int topLeftColumn);
    bool crossCorrelate(int _intAx, int _intAy, int _intAz, PivPointData* pointdata);

    void prepareNCC();
    //void prepareNCC(int _intAxPass, int _intAyPass, int _intAzPass);

    //void NCCCore();
    double NccCore(int _intAxPass, int _intAyPass, int _intAzPass,
                 int _intBxPass, int _intByPass, int _intBzPass);

private:
    // 选取单个窗格的长宽高
    int _intLengthX, _intLengthY, _intLengthZ;

    QMutex mutex;

    // A帧格子的起始坐标_intAx, _intAy, _intAz继承自PivEngine
    // 不需要了，从原始数据中算互相关常数
    //std::list<Frame3DPoint&> * framePoints_A;
    //std::list<Frame3DPoint&> * framePoints_B;

    //IplImage *imgA;
    //IplImage *imgB;

    IplImage **imgA;
    IplImage **imgB;

    CvScalar sA;
    CvScalar sB;

    int i;
    int j;
    int k;//深度

    // 候选区域遍历临时变量
    int ci;
    int cj;
    int ck;

    // 实际允许候选区域窗格对角线端点
    int actualx_start;
    int actualy_start;
    int actualz_start;
    int actualx_end;
    int actualy_end;
    int actualz_end;

    // 重新加载图像的标志
    int _lastIntZ;

    // 候选区域依据--最大速度
    int candidateLimit;

    // 配置参数类
    Settings * settings;
};

#endif // NORMALIZEDCROSSCORRELATE_H
