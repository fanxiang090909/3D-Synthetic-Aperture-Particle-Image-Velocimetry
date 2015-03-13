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
#include "reconstruction_3d_from_tomedata.h"
#include "mycvimage_widget.h"

#include <QDebug>
#include <fstream>

// 相当于在进行空域滤波
int Reconstruction3DFromTomeData::filterSpatial = 200;

Reconstruction3DFromTomeData::Reconstruction3DFromTomeData() : QObject(),
                                                Reconstruction3DIntensity()
{
    i = 0;
    j = 0;
    k = 0;//深度
    num = 0;
    max = 0;
    currentX = -1, currentY = -1, currentZ = -1;
    tempPointX = 0;
    currentPointNum = 0;
    sumPointVal = 0;

    files = std::vector<string>();

    img = NULL;
}

Reconstruction3DFromTomeData::~Reconstruction3DFromTomeData()
{
    if (img)
        cvReleaseImage(&img);

    files.clear();
}

void Reconstruction3DFromTomeData::RawImagesProcess()
{

    static bool rowElementEnd = false;
    static int return_nextPIterator = 5;

    static bool initailIter = false;// 该行已经初始化

    string pathfile;

    this->depth = files.size();
    if (this->depth == 0)
    {
        qDebug() << "[process]You have not read image data files!";
        return;
    }

    for (k = 0; k < files.size(); k++)
    {

        pathfile = path + files[k];
        if (img)
            cvReleaseImage(&img);
        img = cvLoadImage(pathfile.data(), 1);//CV_LOAD_IMAGE_GRAYSCALE);

        this->width = img->width;
        this->height = img->height;
        qDebug() << "[height,width,depth]:" << width << ", " << height << ", " << depth;

        for (i = 0; i < img->height; i++)
            for (j = 0; j < img->width; j++)
            {
                s = cvGet2D(img, i, j);

                if (max < s.val[0])
                    max = s.val[0];

                // 如果换行，迭代器置为起始
                if (i != currentY || k != currentZ)
                {
                    initailIter = false;
                }

                if (initailIter == false)
                {
                    insertIter = temp3DPoints.begin();
                    piterator = temp3DPoints.begin();
                    qDebug() <<"in(k,i)("<< k<< ","<<i<< "):"<<(*piterator)->xpos;
                    rowElementEnd = false;
                    initailIter = true;
                    // 初始化tempP
                    if (piterator != temp3DPoints.end())
                        tempP = (*piterator);
                }

                // 在同一行，紧挨着上一列亮像素点，说明还是一个点
                if (s.val[0] > filterSpatial && i == currentY)
                {
                    sumPointVal += s.val[0];
                    currentPointNum++;
                }
                // 不在同一行，强制该行计算完；或在该行遇到不满足filterSpatial的像素点
                else if ((k != currentZ && currentPointNum != 0) || (i != currentY && currentPointNum != 0) ||
                         (i == currentY && currentPointNum != 0 && s.val[0] <= filterSpatial))
                {

                    tempPointX = currentX + 1 - (int)currentPointNum / 2;

                    // 开始迭代器比较
                    if ((*piterator) != NULL)
                    {
                        // 依然是上一行的点
                        if (i != currentY)
                        {
                            // 重新初始化
                            initailIter = false;
                            currentY = i - 1;
                        }

                        if (rowElementEnd == false && piterator != temp3DPoints.end()
                                &&  currentX - (*piterator)->xpos > 4)
                        {
                            // 迭代器piterator后移
                            return_nextPIterator = this->getNextPiteratorLastYpos(tempP, tempPointX, currentY - 1, k - 1);
                            if (return_nextPIterator < 0)
                                rowElementEnd = true;
                            else
                                rowElementEnd = false;
                        }

                        qDebug() << "\t[curr]xpos:" << tempPointX << ", ypos:" << currentY << ", zpos" << k
                                 << ", val:" << sumPointVal / currentPointNum;

                        // (一). yPos相邻, xPos接近,认为是一个点, 暂时加到节点的next中
                        if (rowElementEnd == false && piterator != temp3DPoints.end() &&
                                (tempPointX - tempP->xpos < 4 && tempPointX - tempP->xpos > -4))
                        {
                            if ((tempP->zpos == k && i - tempP->ypos <= 4 && i - tempP->ypos > -4)
                                || (tempP->zpos + 1 == k && i - tempP->ypos <= 4 && i - tempP->ypos > -4))
                            {
                                Temp3DPoint * p = new Temp3DPoint;
                                p->xpos = tempPointX;
                                p->ypos = i;
                                p->zpos = k;
                                p->val = sumPointVal / currentPointNum;
                                p->next = NULL;

                                while (tempP->next != NULL)
                                    tempP = tempP->next;

                                tempP->next = p;
                                num++;
        \
                                qDebug() << "[n]x,y,z(num):" << tempPointX << "," <<
                                            currentY << "," << k << "(" << num << "), val" << sumPointVal / currentPointNum;

                            }
                        }
                        // (二). 若yPos不相邻, x与上一行相同，insert到list中，把上一行节点求平均，消除多余
                        else if (rowElementEnd == false && piterator != temp3DPoints.end()
                                 && ((tempP->zpos == k && tempP->ypos + 1 < currentY)
                                        || tempP->zpos + 1 < k)
                                 && (tempPointX - tempP->xpos < 4 && tempPointX - tempP->xpos > -4))
                        {

                            insertToList(tempPointX, i, k, sumPointVal / currentPointNum);

                            num++;

                            qDebug() << "[i]x,y,z(num):" << tempPointX << ", " << currentY << "(" << num << ")" <<
                                  " s.val[0]: " << sumPointVal / currentPointNum;

                            // 重置迭代器指向的val和ypos, 消除多余
                            deleteRedundant(piterator, false);
                        }
                        // (三). 若xPos与上一行不同，无论yPos,zpos相同与否，插入到list中
                        else
                        {
                            // 依然是上一行的点
                            if (i != currentY || k != currentZ)
                            {
                                // 重新初始化
                                initailIter = false;
                                currentY = i - 1;
                                currentZ = k - 1;
                            }

                            insertToList(tempPointX, i, k, sumPointVal / currentPointNum);

                            num++;

                            qDebug() << "[i]x,y,z(num):" << tempPointX << ", " << i << "," << k
                                     << "(" << num << ")" << " s.val[0]: " << sumPointVal / currentPointNum;
                        }
                    }
                    currentPointNum = 0;
                    sumPointVal = 0;
                }

                // 在同一行，但是没有检测出该行的点，没有点
                else if (i == currentY && currentPointNum == 0 && s.val[0] <= filterSpatial)
                {
                    // 第1行开始迭代器比较
                    if (i > 0 && piterator != temp3DPoints.end())
                    {
                        tempP = NULL;
                        tempP = (*piterator);
                        // 寻找yPos相邻
                        while (tempP->next != NULL)
                            tempP = tempP->next;

                        if (rowElementEnd == false && currentX - tempP->xpos > 4)
                        {
                            return_nextPIterator = this->getNextPiteratorLastYpos(tempP, currentX, i - 1, k - 1);

                            if (return_nextPIterator < 0)
                                rowElementEnd = true;
                            else
                                rowElementEnd = false;
                        }

                        // 与上一行中间相隔的话，消除多余，当前已经没有点了
                        else if (rowElementEnd == false
                                 && ((tempP->zpos == k && tempP->ypos + 1 < currentY) || tempP->zpos < k)
                                && currentX - tempP->xpos <= 4 && currentX - tempP->xpos > -4)
                        {
                            // 重置迭代器指向的val和ypos, 消除多余
                            deleteRedundant(piterator, false);
                        }
                        /*else if (rowElementEnd == false && tempP->zpos < k
                                 && currentX - tempP->xpos <= 4 && currentX - tempP->xpos > -4)
                        {
                            deleteRedundant(piterator, false);
                        }*/
                    }
                }

                currentY = i;
                currentX = j;
                currentZ = k;
            }
        reconstructProgressSignal(k);
    }

    qDebug() << "max = " << max;


    // 图像输出
    /*CvScalar setS;
    std::list<Temp3DPoint*>::iterator iter;

    qDebug() << "size" << temp3DPoints.size();

    for (iter = temp3DPoints.begin(); iter != temp3DPoints.end(); iter++)
    {
        setS.val[0] = 0;
        setS.val[1] = 255;
        setS.val[2] = 0;

        if (0 < (*iter)->ypos && (*iter)->ypos < img->height &&
                0 < (*iter)->xpos && (*iter)->xpos < img->width)
            cvSet2D(img, (*iter)->ypos, (*iter)->xpos, setS);
    }
    cvSaveImage("D:\\first.bmp",img);

    MyCVImageWidget *mw = new MyCVImageWidget(img);
    mw->show();
    */
    return;

}

void Reconstruction3DFromTomeData::loadImageFiles(std::vector<string> *filename, string path)
{

    if (path.length() == 0 || (*filename).size() == 0)
    {
        //qDebug() << "no file in loadImageFiles";
        return;
    }
    this->path = path;

    fstream _file;
    string pathfile;

    for (int nn = 0; nn < (*filename).size(); nn++)
    {
        pathfile = path + (*filename).at(nn);
        _file.open(pathfile.data(), ios::in);
        if(_file)
        {
            //qDebug() << "has-file" << pathfile.data();
            files.push_back((*filename).at(nn));
            _file.close();
            continue;
        }
        else
        {
            //qDebug() << "no-file" << pathfile.data();
            continue;
        }
    }
}

void Reconstruction3DFromTomeData::reconstructProgressSignal(int s)
{
    emit reconstructProgress(s);
}
