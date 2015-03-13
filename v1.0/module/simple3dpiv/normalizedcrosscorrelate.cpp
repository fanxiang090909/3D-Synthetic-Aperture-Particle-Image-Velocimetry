#include "normalizedcrosscorrelator.h"
#include <vector>
#include <string>
#include <sstream>
#include <QDebug>

using namespace std;

NormalizedCrossCorrelate::NormalizedCrossCorrelate(Settings *settingsPass, std::list<Grid3DPoint*>* gridPass)
    : PivEngine(settingsPass, gridPass)
{
    // 起始坐标
    _intLengthX = PivEngine::intLengthX();
    _intLengthY = PivEngine::intLengthY();
    _intLengthZ = PivEngine::intLengthZ();

    // 配置参数类
    settings = settingsPass;

    // 重新加载图像的标志
    _lastIntZ = -1;

    // 构造与析构对临界资源有冲突操作
    mutex.lock();
    //prepareNCC(0,0,0);
    prepareNCC();
    mutex.unlock();
}

NormalizedCrossCorrelate::~NormalizedCrossCorrelate()
{
    // 构造与析构对临界资源有冲突操作
    mutex.lock();
    mutex.unlock();

    // 对framePointA和B链表不做回收处理，因为不在此类中分配内存，统一回收


    // 图片内存释放
    for(int l = 0; l < _intLengthZ; l++)
    {
        if (imgA[l])
        {
            cvReleaseImage(&imgA[l]);
        }
        else
            break;
    }
    for(int l = 0; l < candidateLimit; l++)
    {
        if (imgB[l])
        {
            cvReleaseImage(&imgB[l]);
        }
        else
            break;
    }
}

/**
 * 数据准备
 * @param _intAx
 * @param _intAy
 * @param _intAz
 */
void NormalizedCrossCorrelate::prepareNCC()
//void NormalizedCrossCorrelate::prepareNCC(int _intAxPass, int _intAyPass, int _intAzPass)
{

/*    // A帧格子的起始坐标
    this->_intAx = _intAxPass;
    this->_intAy = _intAyPass;
    this->_intAz = _intAzPass;
*/
    // 最大速度限制
    candidateLimit = 20;
    //*(IplImage*) a = ;
    //*(IplImage*) b = ;
    imgA = new IplImage*[_intLengthZ];
    imgB = new IplImage*[candidateLimit*2 + _intLengthZ];
    int i = 0;
    for(i = 0; i < _intLengthZ; i++)
    {
         *(imgA+i) = NULL;
    }
    for(i = 0; i < candidateLimit; i++)
    {
         imgB[i] = NULL;
    }
}

/**
 * 指的第二帧B帧候选匹配窗格的左上角坐标，格子中(x,y,z)分别最小的点坐标
 */
//bool NormalizedCrossCorrelate::crossCorrelate(int _intBx, int _intBy, int _intBz)
bool NormalizedCrossCorrelate::crossCorrelate(int _intAx, int _intAy, int _intAz, PivPointData* pointDataPass)
{

    // 记录最大位置
    int _maxBx = -1;
    int _maxBy = -1;
    int _maxBz = -1;
    double _maxNCCCoeffient = 0;
    double _currentNCCCoeffient = 0;

    // 实际允许候选区域窗格对角线端点
    actualx_start = 0;
    actualy_start = 0;
    actualz_start = 0;
    actualx_end = this->settings->getROIWidth();
    actualy_end = this->settings->getROIHeight();
    actualz_end = this->settings->getROIDepth();
    // 一端
    if (_intAx - candidateLimit >= 0)
        actualx_start = _intAx - candidateLimit;
    else
        actualx_start = 0;
    if (_intAy - candidateLimit >= 0)
        actualy_start = _intAy - candidateLimit;
    else
        actualy_start = 0;
    if (_intAz - candidateLimit >= 0)
        actualz_start = _intAz - candidateLimit;
    else
        actualz_start = 0;
    // 另一端
    if (_intAx + _intLengthX + candidateLimit <= this->settings->getROIWidth())
        actualx_end = _intAx + candidateLimit;
    else
        actualx_end = this->settings->getROIWidth() - _intLengthX;
    if (_intAy + _intLengthY + candidateLimit <= this->settings->getROIHeight())
        actualy_end = _intAy + candidateLimit;
    else
        actualy_end = this->settings->getROIHeight() - _intLengthY;
    if (_intAz + _intLengthZ + candidateLimit <= this->settings->getROIDepth())
        actualz_end = _intAz + candidateLimit;
    else
        actualz_end = this->settings->getROIDepth() - _intLengthZ;

    qDebug() << "begin" << _intAx << "," << _intAy << "," << _intAz;

    if (_intAz != _lastIntZ)
    {
        // 图片内存释放
        for(int l = 0; l < _intLengthZ; l++)
        {
            if (imgA[l])
            {
                cvReleaseImage(&imgA[l]);
            }
            else
                break;
        }
        for(int l = 0; l < candidateLimit; l++)
        {
            if (imgB[l])
            {
                cvReleaseImage(&imgB[l]);
            }
            else
                break;
        }

        // 图片加载第一帧
        string pathfileA;
        for (int l = 0; l < _intLengthZ; l++)
        {
            pathfileA = settings->pathA + settings->filesA[_intAz + l];
            imgA[l] = cvLoadImage(pathfileA.data(), 1);//CV_LOAD_IMAGE_GRAYSCALE);
            if (!imgA[l])
            {
                qDebug() << "img of frame A error!" << pathfileA.data();
                return false;
            }
            else
                qDebug() << "success load A image" << pathfileA.data();
        }
        // 图片加载第二帧
        string pathfileB;
        for (int l = 0; l < (actualz_end + _intLengthZ - actualz_start + 1); l++)
        {
            pathfileB = settings->pathB + settings->filesB[actualz_start + l];
            imgB[l] = cvLoadImage(pathfileB.data(), 1);//CV_LOAD_IMAGE_GRAYSCALE);
            if (!imgB[l])
            {
                qDebug() << "img of frame B error!";
                return false;
            }
            else
                qDebug() << "success load B image" << pathfileB.data();
        }

        _lastIntZ = _intAz;
    }

    for (ck = actualz_start; ck < actualz_end; ck++)
        for (cj = actualy_start; cj < actualy_end; cj++)
            for (ci = actualx_start; ci < actualx_end; ci++)
                {
                    _currentNCCCoeffient = NccCore(_intAx,_intAy,_intAz,ci,cj,ck);
                    if (_currentNCCCoeffient > _maxNCCCoeffient)
                    {
                        _maxNCCCoeffient = _currentNCCCoeffient;
                        _maxBx = ci;
                        _maxBy = cj;
                        _maxBz = ck;
                    }
                }

    // 改写PivPointData,delta t在哪里？？？
    double deltat = 1;
    pointDataPass->u = (_maxBx - _intAx) / deltat;
    pointDataPass->v = (_maxBy - _intAy) / deltat;
    pointDataPass->w = (_maxBz - _intAz) / deltat;
    pointDataPass->ncc = _maxNCCCoeffient;

    qDebug()  << "vectory:" << "A(" <<
                 _intAx << "," << _intAy << "," << _intAz << ") - B(" <<
                 _maxBx << "," << _maxBy << "," << _maxBz << ") = vec(" <<
                 _maxBx - _intAx << ", " << _maxBy - _intAy << "," << _maxBz - _intAz << "), NCC" << _maxNCCCoeffient ;


    return true;
}

//void NormalizedCrossCorrelate::NCCCore()
double NormalizedCrossCorrelate::NccCore(int _intAx, int _intAy, int _intAz,
                                         int _intBx, int _intBy, int _intBz)
{

    // 互相关计算
    // Qncc = ∑(framea * frameb) / sqrt(∑framea^2 * ∑frameb^2)

    double upValSum = 0;
    double downAValSum = 0;
    double downBValSum = 0;

    for (k = 0; k < _intLengthZ; k++)
    {
        for (j = 0; j < _intLengthY; j++)
            for (i = 0; i < _intLengthX; i++)
            {
                sA = cvGet2D(imgA[k], _intAy + j, _intAx + i);
                sB = cvGet2D(imgB[k + _intBz - actualz_start], _intBy + j, _intBx + i);
                // 求分子
                upValSum += sA.val[0] * sB.val[0];
                // 求分母
                downAValSum += sA.val[0] * sA.val[0];
                downBValSum += sB.val[0] * sB.val[0];
            }
    }

    double nccCoefficient = upValSum / sqrt(downAValSum * downBValSum);

//    int d = actualz_start;
//    qDebug() << "\timgA index:" << _intAz << "-" << _intAz + _intLengthZ
//             << "imgB index:" << _intBz - actualz_start << "-" << _intBz - actualz_start + _intLengthZ;
    qDebug() << "NCC:" << nccCoefficient << "A(" <<
                _intAx << "," << _intAy << "," << _intAz << ") B(" <<
                _intBx << "," << _intBy << "," << _intBz << ")";

    return nccCoefficient;
}

/***************************************************
NCC = ∑(current_patch_nomalized * mean_patch_nomalized) / sqrt(∑current_patch_nomalized^2 * ∑mean_patch_nomalized^2)
肖照林  18:47:37
double CDepthFrame::CCNomalizedCrossCorrelation(CSFCIPatch mean_patch,CSFCIPatch reference_patch,ColorSpace color_space)
{
        //NCC互相关系数法判断各patch与mean_patch的相似性//
        // 公式 NCC = ∑(current_patch_nomalized * mean_patch_nomalized) / sqrt(∑current_patch_nomalized^2 * ∑mean_patch_nomalized^2)
        //其物理意义相当于计算了两个向量的夹角
        CvScalar mean_patch_pixel_average, mean_patch_sqrted_sum;
        CSFCIPatch mean_patch_nomalized, mean_patch_nomalized_sqrted;
        double result = 0.0;
        CvScalar current_patch_average, temp, temp1;
        CSFCIPatch current_patch_nomalized,current_patch_nomalized_sqrted;
        if (mean_patch.GetPatchValid()&&reference_patch.GetPatchValid())		//判断patch无效则不进行计算
        {
                mean_patch_nomalized = mean_patch;
                current_patch_nomalized = reference_patch;
                mean_patch_nomalized.ChangeColorSpaceTo(spaceGRAY);
                current_patch_nomalized.ChangeColorSpaceTo(spaceGRAY);			//灰度空间进行
                mean_patch_pixel_average = mean_patch_nomalized.GetPixelAverage();	//求取mean_patch的像素均值
                current_patch_average = current_patch_nomalized.GetPixelAverage();
                mean_patch_nomalized.Add(-mean_patch_pixel_average.val[0]);		//归一化操作,去直流分量
                current_patch_nomalized.Add(-current_patch_average.val[0]);
                mean_patch_nomalized_sqrted = mean_patch_nomalized * mean_patch_nomalized;	//平方操作
                current_patch_nomalized_sqrted = current_patch_nomalized * current_patch_nomalized;
                mean_patch_sqrted_sum = mean_patch_nomalized_sqrted.GetPixelSum();	//求和
                current_patch_nomalized = current_patch_nomalized * mean_patch_nomalized;
                temp = current_patch_nomalized.GetPixelSum();
                temp1 = current_patch_nomalized_sqrted.GetPixelSum();
                result = sqrt(temp1.val[0] * mean_patch_sqrted_sum.val[0]);
                result = temp.val[0] / result;
                return result;
        }
        else
        {
                return result;
        }
}
****************************************************/
