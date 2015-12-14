#ifndef IMG_FEATURE_H_
#define IMG_FEATURE_H_

#include "opencv2/opencv.hpp"
#include "SurfFeature.h"
#include "VLADFeature.h"


class ImgFeature
{
public:
	ImgFeature(std::string imgFilename);
	ImgFeature(const cv::Mat& image);
	~ImgFeature();
	bool computeSurfVladFeature(float** vocabulary, int voc_rows, int voc_cols);
	float* GetFeature();
	void FreeFeature(float* array);
	float **GetSurfFeature();
	int GetSurfRows();
	int GetSurfCols();
	void FreeSurfFeature(float** Array);

private:
	cv::Mat m_image;
	float* surf_vlad_feature;
	float **surf_feature;
	int surf_rows;
	int surf_cols;
};

#endif