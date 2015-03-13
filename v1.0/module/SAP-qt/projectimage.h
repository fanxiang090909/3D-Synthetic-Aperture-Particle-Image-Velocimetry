#ifndef PROJECTIMAGE_H
#define PROJECTIMAGE_H

#include "projectimage.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
using namespace std;

/*#pragma comment(lib,"cxcore200.lib")
#pragma comment(lib,"cv200.lib")
#pragma comment(lib,"highgui200.lib")
*/

#define COLORIMAGE 1
#define GRAYIMAGE 0

void ProjectImage(IplImage* Image,CvMat* C,CvMat* R,CvMat* T,CvMat* Refer_C,CvMat* Refer_R, CvMat* Refer_T, CvMat* PlaneNormalVector,int Imagetype);//CvMat* C非参考摄像机内参矩阵,以此类推


#endif // PROJECTIMAGE_H
