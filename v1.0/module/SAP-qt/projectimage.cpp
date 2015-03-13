#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "ProjectImage.h"
using namespace std;

/*#pragma comment(lib,"cxcore200.lib")
#pragma comment(lib,"cv200.lib")
#pragma comment(lib,"highgui200.lib")
*/

//CvMat* C非参考摄像机内参矩阵,以此类推
void ProjectImage(IplImage* Image, CvMat* C, CvMat* R, CvMat* T, CvMat* Refer_C,
                                        CvMat* Refer_R, CvMat* Refer_T,
                            CvMat* PlaneNormalVector, int Imagetype)
{
    //计算关于图像和参考图像之间关于空间中某个平面的单应矩阵

    /******************************************************/
    /*  这部分为计算参考图像与其他图像关于场景中一个平面的单应矩阵    */
    /******************************************************/

    CvMat* Product[8];
    for(int i=0;i<8;i++)
            Product[i]=cvCreateMat(3,3,CV_64FC1);

    CvMat* InvofRefer_R=cvCreateMat(3,3,CV_64FC1);

    cvT(Refer_R,InvofRefer_R);

    //Product[0] = R*Rf^T
    cvMatMul(R,InvofRefer_R,Product[0]);

    CvMat* TPlaneNormalVector=cvCreateMat(1,3,CV_64FC1);
    cvT(PlaneNormalVector,TPlaneNormalVector);

    //Product[1] = Tf*N^T
    cvMatMul(Refer_T,TPlaneNormalVector,Product[1]);

    CvMat* I=cvCreateMat(3,3,CV_64FC1);
    CvScalar ratio;
    ratio=cvRealScalar(1);

    //初始化单位阵
    cvSetIdentity(I,ratio);

    cvSub(I,Product[1],Product[2],0);
    //Product[2] = I - Tf*N^T
    //Product[3] = (R * Rf^T) * (I - Tf*N^T)
    cvMatMul(Product[0],Product[2],Product[3]);
    //Product[4] = T * N^T
    cvMatMul(T,TPlaneNormalVector,Product[4]);
    //Product[5] = (R * Rf^T) * (I - Tf*N^T) + T * N^T
    cvAdd(Product[3],Product[4],Product[5],0);
    //Product[6] = C * [(R * Rf^T) * (I - Tf*N^T) + T * N^T]
    cvMatMul(C,Product[5],Product[6]);

    CvMat* InvofRefer_C=cvCreateMat(3,3,CV_64FC1);

    cvInv(Refer_C,InvofRefer_C,CV_LU);
    //Product[7] = C * [(R * Rf^T) * (I - Tf*N^T) + T * N^T] *Cf^-1
    cvMatMul(Product[6],InvofRefer_C,Product[7]);

    /*将计算的单应矩阵赋值给数组homography[3][3]*/

    double homography[3][3],*Homographydata;
    for(int i=0;i<3;i++)
    {
            for(int j=0;j<3;j++)
                    homography[i][j]=0;
    }

    Homographydata=Product[7]->data.db;

    //将计算得到的单应矩阵赋值个homography[][];
    for(int i=0;i<3;i++)
    {
         for(int j=0;j<3;j++)
              homography[i][j]=Homographydata[i*3+j];
    };

    //用于存储经过变换的图像
    IplImage* ProjectedImage=cvCreateImage(cvSize(Image->width,Image->height),IPL_DEPTH_8U,3);
    cvZero(ProjectedImage);
    double point[3];
    for(int i=0;i<3;i++)
            point[i]=0;
    CvScalar s;

    for(int height=0;height<Image->height;height++)  //计算变换后图像的像素点在变换前图像中的位置并赋值
        {
            for(int width=0;width<Image->width;width++)
            {
                for(int k=0;k<3;k++)
                        point[k]=homography[k][0]*width+homography[k][1]*height+homography[k][2];
                point[0]=point[0]/point[2];
                point[1]=point[1]/point[2];        //齐次坐标转化为非齐次坐标
                int a,b;
                a=point[0];
                b=point[1];                       //像素点坐标取整，从double转为int

                if(a>=0&&a<Image->width&&b>=0&&b<Image->height)
                {
                        s=cvGet2D(Image,b,a);
                        cvSet2D(ProjectedImage,height,width,s);
                }
            };
        };

    //将变换后的图像复制到原图像的存储空间中
    cvCopy(ProjectedImage,Image,0);

    //以下几行用于测试帧变换是否正确
    //显示变换后的图像
    //cvNamedWindow( "ProjectImage",CV_WINDOW_AUTOSIZE);
    //cvShowImage( "ProjectImage", Image);
    //cvResizeWindow("ProjectImage",Image->width,Image->height);
    //cvWaitKey(0);
    //cvDestroyWindow( "ProjectImage" );

    cvReleaseImage(&ProjectedImage);
    cvReleaseMat(&InvofRefer_R);
    cvReleaseMat(&TPlaneNormalVector);
    cvReleaseMat(&I);
    cvReleaseMat(&InvofRefer_C);

    for(int i=0;i<8;i++)
        cvReleaseMat(&Product[i]);

}
