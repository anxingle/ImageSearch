#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <opencv2/opencv.hpp>
using namespace cv;

typedef struct{
	std::pair<std::string,double> PathDis;
	int IdxQuery;
	int IdxDB;
}QueryRe;

typedef enum{
	DENSESURF = 0,
	DENSESURF_PRO_VLAD,
}FEAFLAGS;

#define DEBUG 0


extern const std::string trainPath; // 图片库地址
extern const std::string testPath;

extern const int surfDescriptorSize; // surf特征维度
extern const int surfBlockSize; // 每张图片的surf特征块数
extern const int denseSurfFeatureSize; // 以dense surf方式抽取，每张图片的特征维度
extern const int clusterCount; // 聚类出来的码本个数
extern const int surfVladFeatureSize;  // 以dense surf + VLAD方式抽取，每张图片的特征纬度
extern const int featureAfterPCASize; // PCA降维后的特征维度
extern const int nearestK; // 返回的最近邻图片的个数
extern const int categoryNum; // 阿里巴巴图片检索数据的小类别数
const int nbits = 256; // 编码的位数

extern FEAFLAGS feaflags;


extern std::vector<string> neighbors;  //搜索结果*****************！！！！！！！！！
extern std::vector<QueryRe > IRResult;	//保存检索结果
extern std::vector<cv::Mat > QueryPro;	//保存query的proposal结果

extern std::vector<double > SimRe;	//保存与检索结果之间的距离
extern std::vector<cv::Mat > ShowImgs;//保存检索得到的图片
extern std::vector<std::string > ImgName; //保存检索得到的图片的路径

extern int pageIdx;	//MFC中翻页的页数索引
extern const int ImgNumPerPage;	//每页显示结果图数目


extern std::unordered_map<string, std::unordered_set<string> > categoryLabels;
extern std::vector<std::pair<float*, string> > trainFeatures;
extern float** vocabulary; // 保存码本
extern float** pcaMatrix; // PCA矩阵
extern float** rotationMatrix; // ITQ旋转矩阵
//	DenseSurf + VLAD + Proposal
extern std::unordered_map<std::string, std::vector<float*> > trainFeatureProVladMap; // 保存库图片及每张图片对应的特征

//	DenseSurf
extern std::unordered_map<std::string, std::vector<float*> > trainFeatureDenseSurfMap; // 保存库图片及每张图片对应的DenseSurf特征
#endif