#ifndef SURF_FEATURE_H_
#define SURF_FEATURE_H_

#include "opencv2/opencv.hpp"
#include"opencv2/nonfree/features2d.hpp"

class SurfFeature
{
public:
	SurfFeature();
	~SurfFeature();
	void computeFeature(IplImage* image_color);
	float* GetFeature();
	float** GetFeature(const cv::Mat& image);
	void FreeFeature(float **Array);
	void FreeFeature(float *Array);
	int GetRows() const;
	int GetCols() const;
private:
	static const int cwlDiameter = 32;  //提取surf特征时的窗口参数
	static const int cwlStep = 32;
	static const int surfFeatureSize = 100 * 64;
	float* featureArray;
	cv::Mat SurfMat;
	float** SurfArray;
	int rows;
	int cols;
	void GetDenseSurf(const cv::Mat& image);    //提取surf特征
	cv::Mat GetDenseSurf2(const cv::Mat& image); 
	void Mat2Array();
	float **newArray();
};
#endif