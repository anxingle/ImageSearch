#ifndef _H_API_INTERFACE_H
#define _H_API_INTERFACE_H

#include "Configure.h"
#include "ImgConAna.h"
#include "ImgFeature.h"
#include "Learn.h"
#include "LoadFile.h"
#include "NearestNeighbors.h"
#include "ostuRemoveBorder.h"
#include "Search.h"
#include "SurfFeature.h"
#include "Train.h"
#include "Utils.h"
#include "VLADFeature.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void GlobalInit(string labelFile,
			   string centerFile,
			   string pacMatrixFile,
			   string imageNameFile,
			   string featureBinFile,
			   string rotaMatrixFile 
			   );	//加载相关数据库--全局变量
int Retrieval(Mat img);	//计算图片的检索结果--存放于全局变量QueryRe
int ShowToMFC();	//根据翻页需要来得到显示的图片和相应的 proposal && 相似度

#endif