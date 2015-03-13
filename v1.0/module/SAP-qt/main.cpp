#include <QtCore/QCoreApplication>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "projectimage.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <QDebug>

int cameranumber = 4;
int cameraArraySize = 4;

using namespace std;

void SAP(IplImage* PIC[], IplImage* new_pic, int reference_camera, int WorldorCamera,double distance)  //对于已经过投影的图像进行累加和均值处理
{
    int pic_num=cameranumber;                //定义合成图像的帧数=摄像机数目
    int pic_w=PIC[0]->width;                 //定义图像宽高
    int pic_h=PIC[0]->height;
    CvScalar pixel;

    CvMat *ImagePixel_total[3];//存储像素值(3个通道)总和[752][576]
    for(int k=0;k<3;k++)
            ImagePixel_total[k]=cvCreateMat(pic_h,pic_w,CV_64FC1);
    CvMat *ImagePixel_num=cvCreateMat(pic_h,pic_w,CV_32SC1);//存储叠加像素个数
    for(int k=0;k<3;k++) //初始化清零
         cvZero(ImagePixel_total[k]);
    cvZero(ImagePixel_num);
    CvMemStorage* memstorage = cvCreateMemStorage(0);

    //加载摄像机参数文件以供读取
    CvFileStorage* Read_Param = cvOpenFileStorage("calibration_result_x.xml", memstorage, CV_STORAGE_READ );

    CvMat* IntrinsicMatrix=cvCreateMat(3,3,CV_64FC1);//定义非参考摄像机内参
    CvMat* RotationMatrix=cvCreateMat( 3, 3 ,CV_64FC1);   //定义非参考摄像机旋转矩阵
    CvMat* TranslationMatrix=cvCreateMat( 3, 1 ,CV_64FC1);    //定义非参考摄像机平移向量
    CvMat* Refer_Intrinsic=cvCreateMat(3,3,CV_64FC1);//定义参考摄像机内参
    CvMat* Refer_Rotation=cvCreateMat(3,3,CV_64FC1);//定义摄像机外参
    CvMat* Refer_Translation=cvCreateMat(3,1,CV_64FC1);//定义摄像机的平移向量
    char num[3];
    num[2]='\0';
    sprintf(num,"%d",reference_camera); //将参考相机号转为字符串型，以下几行代码为读取参考相机参数

    string paramname="";
    string paramname_I,paramname_R,paramname_T;

    paramname="cam";
    //paramname = "camera_";
    paramname=paramname.append(num);
    //paramname=paramname+"_";
    paramname_I=paramname+"intrinsic_matrix";
    paramname_R=paramname+"rotation_matrix";
    paramname_T=paramname+"translation_vectors";
    //paramname_T = paramname+"trasnlation_vector";

    char *paramname1=(char *)paramname_I.c_str();
    Refer_Intrinsic=(CvMat*)cvReadByName(Read_Param, NULL, paramname1, NULL);
    paramname1=(char*)paramname_R.c_str();
    Refer_Rotation=(CvMat*)cvReadByName(Read_Param, NULL, paramname1, NULL);
    paramname1=(char*)paramname_T.c_str();
    Refer_Translation=(CvMat*)cvReadByName(Read_Param, NULL, paramname1, NULL);
    CvMat* PlaneNormalVector=cvCreateMat(3,1,CV_64FC1);//定义世界坐标系下的聚焦平面的法向量
    double P[3][1]={0,0,-1.0/3.4184e+003};				//聚焦平面法向量
    P[2][0]=P[2][0]*3418.4/distance;
    cvInitMatHeader(PlaneNormalVector,3,1,CV_64FC1,P);//初始化聚焦平面法向量
    CvMat* PlaneNormalVectorC=cvCreateMat(3,1,CV_64FC1);//定义参考摄像机坐标系下聚焦平面的法向量
    if(WorldorCamera==0)//根据用户不同的选择转换坐标系
    {
        cvMatMul(Refer_Rotation,PlaneNormalVector,PlaneNormalVectorC);
            cvCopy(PlaneNormalVectorC,PlaneNormalVector,0);
    }
    for(int l=0;l<pic_num;l++)
    {
        //ProjectImage(IplImage* Image,CvMat* C,CvMat* R,CvMat* T,CvMat* Refer_C,CvMat* Refer_R, CvMat* Refer_T, CvMat* PlaneNormalVector,int WorldorCamera,int Imagetype);//CvMat* C非参考摄像机内参矩阵,以此类推,对灰度图象做投影变换
        sprintf(num,"%d",l+1);
        paramname="cam";
        //paramname = "camera_";
        paramname=paramname.append(num);
        //paramname=paramname+"_";
        paramname_I=paramname+"intrinsic_matrix";
        paramname_R=paramname+"rotation_matrix";
        paramname_T=paramname+"translation_vectors";
        //paramname_T = paramname+"trasnlation_vector";
/*
        paramname = "camera_";
        paramname=paramname.append(num);
        paramname=paramname+"_";
        paramname_I=paramname+"intrinsic_matrix";
        paramname_R=paramname+"rotation_matrix";
        //paramname_T=paramname+"translation_vectors";
        paramname_T = paramname+"trasnlation_vector";
*/
        if((l+1)!=reference_camera)//若图像为非参考图像，则对其做投影变换
        {
            paramname1=(char *)paramname_I.c_str();
            IntrinsicMatrix=(CvMat*)cvReadByName(Read_Param, NULL, paramname1, NULL);
            paramname1=(char*)paramname_R.c_str();
            RotationMatrix=(CvMat*)cvReadByName( Read_Param, NULL, paramname1, NULL);
            paramname1=(char *)paramname_T.c_str();
            TranslationMatrix=(CvMat*)cvReadByName( Read_Param, NULL, paramname1, NULL);//读取"calibration_result.xml"中的R,T外参数
                //对图像进行投影
            ProjectImage(PIC[l],IntrinsicMatrix,RotationMatrix,TranslationMatrix,Refer_Intrinsic,Refer_Rotation,Refer_Translation,PlaneNormalVector,GRAYIMAGE);
        }
    }

    for(int l=0;l<pic_num;l++)
    {
        for(int i=0;i<pic_h;i++)
        {
            for(int j=0;j<pic_w;j++)
            {

                pixel=cvGet2D(PIC[l],i,j);	//像素的叠加
                for(int k=0;k<3;k++)
                {
                    double mm=CV_MAT_ELEM(*ImagePixel_total[k],double,i,j);
                    mm+=pixel.val[k];
                    CV_MAT_ELEM(*ImagePixel_total[k],double,i,j)=mm;
                }
                if((pixel.val[0]||pixel.val[1]||pixel.val[2])!=0)//叠加数量统计
                {
                    int nn=CV_MAT_ELEM(*ImagePixel_num,int,i,j);
                    nn++;
                    CV_MAT_ELEM(*ImagePixel_num,int,i,j)=nn;
                }
            }
        }
    }
    for(int i=0;i<pic_h;i++)//对SAP图像进行赋值
    {
        for(int j=0;j<pic_w;j++)
        {
            if(CV_MAT_ELEM(*ImagePixel_num,int,i,j)!=0)
            {
                    for(int k=0;k<3;k++)
                       pixel.val[k]=CV_MAT_ELEM(*ImagePixel_total[k],double,i,j)/CV_MAT_ELEM(*ImagePixel_num,int,i,j);
                cvSet2D(new_pic,i,j,pixel);
            }
        }

    }
    for(int k=0;k<3;k++)
    cvReleaseMat(&ImagePixel_total[k]);
    cvReleaseMat(&ImagePixel_num);
    cvReleaseFileStorage(&Read_Param);
    cvReleaseMemStorage(&memstorage);
    cvReleaseMat(&Refer_Intrinsic);
    cvReleaseMat(&Refer_Rotation);
    cvReleaseMat(&Refer_Translation);
    cvReleaseMat(&IntrinsicMatrix);
    cvReleaseMat(&RotationMatrix);
    cvReleaseMat(&TranslationMatrix);
    cvReleaseMat(&PlaneNormalVector);
    cvReleaseMat(&PlaneNormalVectorC);
}

int main( int argc,char** argv)
{
    QCoreApplication a(argc, argv);

    ifstream fpic("pic_p.txt");
    if(fpic.is_open())
        qDebug() << "find file!";
    else
        qDebug() << "can't find file!";
    string picname;
    int Reference_cam = 1;
    int imagenumber = 4;
    double distance = 3148;
    int WorldorCamera = 0;
    /*cout<<"please input the number of cameras for SAP video:\n";
    cin>>cameranumber;
    cout<<"please input the number of images for SAP video:\n";
    cin>>imagenumber;		//视频中图像的帧数
    cout<<"please input the Reference camera for viewpoint selection[1-64]:\n";
    cin>>Reference_cam;   //参考相机
    cout<<"choose the coordination, 0or1, correspond to world and camera coordination:\n";
    cin>>WorldorCamera;
    cout<<"please input the distance between the focal plane and the reference camera(mm)(default 3418):\n";
    cin>>distance;*/
    CvVideoWriter * writer = 0;
    int isColor = 1;
    int fps = 20; // or 30
    int frameW = 752; //  camera CCD resolution
    int frameH = 576;
    //writer=cvCreateVideoWriter("out.avi",-1,fps,cvSize(frameW,frameH),isColor); //写入彩色帧?与下面程序不符,可能有疏漏
    /*　　CV_FOURCC(’P’,’I’,’M’,’1’) = MPEG-1 codec
      CV_FOURCC(’M’,’J’,’P’,’G’) = motion-jpeg codec (does not work well)
      CV_FOURCC(’M’, ’P’, ’4’, ’2’) = MPEG-4.2 codec
      CV_FOURCC(’D’, ’I’, ’V’, ’3’) = MPEG-4.3 codec
      CV_FOURCC(’D’, ’I’, ’V’, ’X’) = MPEG-4 codec
      CV_FOURCC(’U’, ’2’, ’6’, ’3’) = H263 codec
      CV_FOURCC(’I’, ’2’, ’6’, ’3’) = H263I codec
      CV_FOURCC(’F’, ’L’, ’V’, ’1’) = FLV1 codec     */
    //IplImage* img1=cvCreateImage(cvSize(752,576),IPL_DEPTH_8U,1);

    //测试帧是否读完
    int test=1;
    while(test<=imagenumber)
    {
        std::getline(fpic,picname);
        qDebug() << "1"<<picname.c_str() << "2";
        int i;
        ifstream fdirect("direct_p.txt");
        if(fdirect.is_open())
            qDebug() << "find file!";
        else
            qDebug() << "can't find file!";

        string directname;
        IplImage* new_pic1 = cvCreateImage(cvSize(frameW,frameH),IPL_DEPTH_8U,3);
        IplImage *pImg[cameraArraySize];
        for(i=0;i<cameraArraySize;i++)
        {
             pImg[i]=NULL;
        }
        //pImg=new IplImage*[cameranumber];  //动态分配一个图像数组
        i=0;  //用于检测摄像机数是否超出读取范围
        while(i<cameranumber)//读取同帧图像
        {
            std::getline(fdirect,directname);
            qDebug() << "getimage:"<<directname.c_str();

            string filename;
            filename=directname+picname;
            char *filename1=(char*)filename.c_str();
            pImg[i]=cvLoadImage(filename1,1);
            //读取3通道图象

            //以下几行代码为测试帧是否正确读取
            //cvNamedWindow("Original",CV_WINDOW_AUTOSIZE);
            //cvShowImage("Original",pImg[i]);
            //cvWaitKey(0);
            //cvDestroyWindow("Original");

            i++;
        }

        //对图像进行合成孔径处理
        SAP(pImg,new_pic1,Reference_cam,WorldorCamera,distance);

/*      cvNamedWindow( "SAP_PIC",CV_WINDOW_AUTOSIZE);*/
/*      */
        // 显示合成图像
        cvShowImage( "SA_PIV", new_pic1);
        // 写入视频
        //cvWriteFrame(writer,new_pic1); //
        // 保存文件
        cvSaveImage(("D:\\" + picname).c_str(),new_pic1); //

// 		cvWaitKey(0);
// 		cvDestroyWindow( "SAP_PIC" );

        cvReleaseImage( &new_pic1 );
        //释放图像存储空间
        for(i=0;i<cameraArraySize;i++)
        {
            if (pImg[i])
            {
                cvReleaseImage(&pImg[i]);
            }
            else
                break;
        }
        test++;
    }
    //cvReleaseVideoWriter(&writer); //

    //printf("tttttttttt88888888888888888888888888888888888888888888888888888888888\n");

    cout<<"SAP处理完成";
    cvWaitKey('q');

    return a.exec();

}
