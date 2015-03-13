/**
 * 类Reconstruction3DFromTomeData
 * 继承自public Reconstruction3DIntensity
 *
 * Tomo-PIV和SAPIV图像的3维重建方法
 * 外部接口必须调用步骤
 * a) 创建实例： Reconstruction3DFromTomeData * recon = new Reconstruction3DFromTomeData();
 * b) 加载图像： recon->loadImageFiles(files, path);
 * c) 初始化结果点文件：
 *             recon->initail3dPointsResultFile("D:\\resultpoints.txt");
 * d) 迭代计算三维点，创建二维链表计算，同步保存已计算点(因为可在二维链表中删除)
 *             recon->RawImagesProcess();
 * e) 保存链表剩余点至结果文件
 *             recon->savePointsToFile("D:\\points.txt");
 * @author 范翔
 * @version 1.0.0
 * @date 2013-05-21
 */

#ifndef RECONSTRUCTION_3D_FROM_TOMEDATA_H
#define RECONSTRUCTION_3D_FROM_TOMEDATA_H

#include "reconstruction_3dintensity.h"
#include <QObject>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <string>

using namespace std;

class Reconstruction3DFromTomeData : public QObject,
        public Reconstruction3DIntensity
{

    Q_OBJECT
private:

    // 相当于在进行空域滤波
    static int filterSpatial;
    CvScalar s;
    int i;
    int j;
    int k;//深度
    int num;
    int max;
    int currentX, currentY, currentZ;
    int tempPointX;
    int currentPointNum;
    int sumPointVal;


    IplImage *img;

public:
    Reconstruction3DFromTomeData();

    ~Reconstruction3DFromTomeData();

    void RawImagesProcess();

    void loadImageFiles(std::vector<string> *filename, string path);
private:
    void reconstructProgressSignal(int s);

signals:
    void reconstructProgress(int);
};

#endif // RECONSTRUCTION_3D_FROM_TOMEDATA_H
