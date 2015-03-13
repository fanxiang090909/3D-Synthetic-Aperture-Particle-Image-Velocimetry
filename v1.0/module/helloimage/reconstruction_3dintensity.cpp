#include "reconstruction_3dintensity.h"


#include <QFile>
#include <QTextStream>
#include <QString>

#include <QDebug>

Reconstruction3DIntensity::Reconstruction3DIntensity()
{
    height = 0;
    width = 0;
    depth = 0;

    tempP = NULL;

    temp3DPoints = list<Temp3DPoint*>();
    piterator = temp3DPoints.begin();

}

Reconstruction3DIntensity::~Reconstruction3DIntensity()
{
    temp3DPoints.clear();
}

void Reconstruction3DIntensity::loadImageFiles(std::vector<string> *filename, string path)
{
    if (path.length() == 0 || (*filename).size() == 0)
    {
        return;
    }
    this->path = path;
    for (int nn = 0; nn < (*filename).size(); nn++)
    {
            files.push_back((*filename).at(nn));
    }
}

std::list<Temp3DPoint*> Reconstruction3DIntensity::getCurrent3DPointsList()
{
    return temp3DPoints;
}

long Reconstruction3DIntensity::index(int x, int y, int z)
{
    return x * this->height * this->depth + y * this->depth + z;
}

// 有问题，包括调用时lastZPos参数不对
/**
 * @return 返回迭代器状态，及tempP的返回值特征，在哪一个平面上
 *         -1 表示失败，没有找到可以插入节点的地方，（即老版本改函数返回值true，迭代器移到结束位置为真true）
 *         0  表示找到，在当前zpos，即lastZPos+1位置处找到zpos相同的y相邻的点，而lastZPos处没有相邻像素点
 *               （同一幅图像的可插入二维链表节点的点），后续操作，插入节点
 *         1  表示找到，在当前zpos，和lastZPos处都找到相邻像素点，后续操作，插入节点
 *         /----删掉/2  表示找到，在当前zpos没有找到，而在lastZPoos处找到相邻像素点，后续操作，插入节点
 */
int Reconstruction3DIntensity::getNextPiteratorLastYpos(
    Temp3DPoint*& tempP, int tempPointX ,int lastYPos, int lastZPos)
{
    static int times = 0;// 记录迭代次数，是否迭代

    if (temp3DPoints.end() == piterator)
        return -1;

    while ((*piterator) != NULL && piterator != temp3DPoints.end())
    {
        tempP = NULL;

        tempP = (*piterator);
        // 改bug
        if (tempP->ypos == lastYPos + 1 && tempP->zpos == lastZPos + 1)
        {
            times++;
            piterator++;
        }

        // 寻找yPos相邻
        while (tempP != NULL)
        {
            if(tempP->xpos >= tempPointX - 4)
            {
                // 只在当前深度平面上有相邻点，lastZPos深度没有相邻点
                if (tempP->zpos > lastZPos && tempP->ypos >= lastYPos)
                {
                    if (times != 0)
                    {
                        qDebug() << "\t[itTp]xpos:" << tempP->xpos << ", ypos:" << tempP->ypos <<
                                     ", zpos:" << tempP->zpos << ", val:" << tempP->val << ", next:" << tempP->next;
                        times = 0;
                        return 0;
                    }
                }
                else if (tempP->zpos == lastZPos && tempP->ypos >= lastYPos - 4 && tempP->ypos < lastYPos + 4)
                {
                    if (times != 0)
                    {
                        qDebug() << "\t[itTp]xpos:" << tempP->xpos << ", ypos:" << tempP->ypos <<
                                     ", zpos:" << tempP->zpos << ", val:" << tempP->val << ", next:" << tempP->next;
                        times = 0;
                        return 1;
                    }
                }
            }
            tempP = tempP->next;
        }

        times++;
        piterator++;

    }

    //if (tempP != NULL)
    qDebug() << "\t[itTp]ypos:" << lastYPos + 1 << ",zpos:" << lastZPos + 1 <<"end()";
    times = 0;
    // 到结束了
    return -1;

}

bool Reconstruction3DIntensity::insertToList(int x, int y, int z, int val)
{
    Temp3DPoint * p = new Temp3DPoint;
    p->xpos = x;
    p->ypos = y;
    p->zpos = z;
    p->val = val;
    p->next = NULL;

    // 求插入位置
    long tempIndex = this->index(x, y, z); // 待插入点
    long cmpindex = 0;

    Temp3DPoint *tempP = NULL;
    while (insertIter != temp3DPoints.end())
    {
        tempP = (*insertIter);
        // 寻找yPos相邻
        // while (tempP->next != NULL)
        //    tempP = tempP->next;

        cmpindex =  this->index(tempP->xpos, tempP->ypos, tempP->zpos); // 比较点
        if (cmpindex < tempIndex)
            insertIter++;
        else if (cmpindex > tempIndex)
        {
            temp3DPoints.insert(insertIter, p);
            return true;
        }
    }

    // 插入到末尾
    temp3DPoints.insert(insertIter, p);

    return true;
}

bool Reconstruction3DIntensity::deleteRedundant(std::list<Temp3DPoint*>::iterator deletePoint, bool deleteFromList)
{
    static int deletenum = 0;
    Temp3DPoint * newValP = new Temp3DPoint;
    Temp3DPoint * resetValP = (*deletePoint);
    Temp3DPoint * deleteP;
    int sumVal = resetValP->val;
    int sumYPos = resetValP->ypos;
    int sumXPos = resetValP->xpos;
    int sumZPos = resetValP->zpos;

    int samePynum = 1;
    while (resetValP->next != NULL)
    {
        resetValP = resetValP->next;
        sumVal += resetValP->val;
        sumYPos += resetValP->ypos;
        sumXPos += resetValP->xpos;
        sumZPos += resetValP->zpos;
        samePynum++;
    }
    if (samePynum > 1)
    {
        newValP->val = sumVal / samePynum;
        newValP->ypos = sumYPos / samePynum;
        newValP->xpos = sumXPos / samePynum;
        newValP->zpos = sumZPos / samePynum;
        newValP->next = NULL;
    }
    else
    {
        return false;
    }

    // 删除原节点
    for (int d = 0; d < samePynum - 1; d++)
    {
        deleteP = (*deletePoint)->next;
        (*deletePoint)->next = deleteP->next;
        delete deleteP;
        deletenum++;
        qDebug() << "\t[delete:num]" << deletenum;
    }

    //std::list<Temp3DPoint*>::iterator iter = deletePoint;

    qDebug() << "\t[remove:x,y.z]" << (*deletePoint)->xpos << ","
             << (*deletePoint)->ypos << "," << (*deletePoint)->zpos;

    // 要马上自增 防止迭代器失效，如果自增到链表尾，还要返回
    temp3DPoints.remove(*(deletePoint++));

    // 在Z深度时，确定为一个点，直接从list中删除，写入文件
    if (deleteFromList == true)
    {
        static int num = 0;
        // 处理文件
        //QFile file(QString(("D:\\"+"3dPoints_1.txt").data()));
        QFile file("D:\\3dPoints_1.txt");

        // 追加写入点坐标信息
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
            return true;

        QTextStream out(&file);

        out << "num:" << num << ":(x,y,z)=("
            << newValP->xpos << "," << newValP->ypos << "," << newValP->zpos << "),\tindex:"
                << index(newValP->xpos,newValP->ypos,newValP->zpos) << "\n";

        file.close();
        delete newValP;

        if (deletePoint == temp3DPoints.begin())
            return true;
        else
        {
            deletePoint--;
            if (deletePoint == temp3DPoints.begin())
                // 如果到最开始，在开始处插入
                return true;
        }
        return true;
    }

    // 没有z深度的删除节点，不写入文件

    if (deletePoint == temp3DPoints.begin())
    {
        // 如果到最开始，在开始处插入
        temp3DPoints.insert(temp3DPoints.begin(), newValP);
        qDebug() << "\t[reinsert:x,y.z]" << newValP->xpos << ","
                 << newValP->ypos << "," << newValP->zpos;
        return true;

    }
    else
    {
        deletePoint--;
        if (deletePoint == temp3DPoints.begin())
        {
            // 如果到最开始，在开始处插入
            temp3DPoints.insert(temp3DPoints.begin(), newValP);
            qDebug() << "\t[reinsert:x,y.z]" << newValP->xpos << ","
                     << newValP->ypos << "," << newValP->zpos;
            return true;

        }
        else
        {
            long tempIndex = this->index(newValP->xpos, newValP->ypos, newValP->zpos); // 待插入点
            long cmpindex;

            // 重新插入节点
            while (deletePoint != temp3DPoints.begin())
            {
                cmpindex =  this->index((*deletePoint)->xpos, (*deletePoint)->ypos, (*deletePoint)->zpos); // 比较点

                if (cmpindex > tempIndex)
                    deletePoint--;
                else
                {
                    //deletePoint--;
                    deletePoint++;
                    temp3DPoints.insert(deletePoint, newValP);
                    qDebug() << "\t[reinsert:x,y.z]" << newValP->xpos << ","
                             << newValP->ypos << "," << newValP->zpos;
                    return true;
                }
            }
        }
    }

    return false;
}

void Reconstruction3DIntensity::initail3dPointsResultFile(string resultFile)
{
    QFile file(QString(resultFile.data()));

    file.remove();
}

void Reconstruction3DIntensity::savePointsToFile(string saveFile)
{
    int num = 0;
    if (temp3DPoints.size() == 0)
    {
        qDebug() << "no points to save";
        return;
    }

    /*QString filename = QFileDialog::getSaveFileName(
       this,
       QObject::tr("导出TXT文件表格"),
       QDir::currentPath(),
       "Document files (*.txt);;All files(*.*)");
    if (filename.isNull()) {

        // 用户取消选择
        QMessageBox::information(this, QObject::tr("导出TXT文件表格"),
                                 QObject::tr("没有指定要保存的文件目标"), QMessageBox::Ok | QMessageBox::Cancel);
    } else {
        //用户选择了文件
        QMessageBox::information(this, QObject::tr("导出TXT文件表格"),
                                 QObject::tr("保存文件成功"), QMessageBox::Ok | QMessageBox::Cancel);
*/
    // 处理文件
    QFile file(QString(saveFile.data()));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    // 保存格式
    std::list<Temp3DPoint*>::iterator iter;

    //iter = temp3DPoints.begin();
    //temp3DPoints.remove((*iter));

    qDebug() << "size" << temp3DPoints.size();

    for (iter = temp3DPoints.begin(); iter != temp3DPoints.end(); iter++)
    {
        num++;
        out << "num:" << num << ":(x,y,z)=(" << (*iter)->xpos << "," << (*iter)->ypos << "," << (*iter)->zpos << "),\tindex:"
            << index((*iter)->xpos,(*iter)->ypos,(*iter)->zpos) << "\n";
    }
    file.close();

}

