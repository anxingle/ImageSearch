#include <string>
#include <fstream>
#include <sstream>
#include "SurfFeature.h"
using namespace std; //why


SurfFeature::SurfFeature()
{
	
}

SurfFeature::~SurfFeature()
{

}


void SurfFeature::computeFeature(IplImage *image_color)
{
	//转换为灰度图像
	IplImage *GrayImage = cvCreateImage(cvGetSize(image_color),IPL_DEPTH_8U,1);
	if(image_color->nChannels==3){
		cvCvtColor(image_color,GrayImage,CV_BGR2GRAY);
	}
	else{cvCopy(image_color,GrayImage);}

	cv::Mat image(GrayImage);

	//提取特征并转换为一维向量
	cv::Mat surfFeature = GetDenseSurf2(image);
	surfFeature.reshape(0,1);
	rows = surfFeature.rows;
	cols = surfFeature.cols;
	assert(surfFeatureSize == surfFeature.total());
	featureArray = new float[surfFeatureSize];
	memcpy(featureArray, surfFeature.ptr<float>(0), surfFeatureSize*sizeof(float));

	//释放灰度图像
	cvReleaseImage(&GrayImage);
}

float* SurfFeature::GetFeature()
{
	return featureArray;
}

float** SurfFeature::GetFeature(const cv::Mat& image)
{
	GetDenseSurf2(image);
	Mat2Array();
	return SurfArray;
}
int SurfFeature::GetRows() const
{
	return rows;
}
int SurfFeature::GetCols() const
{
	return cols;
}

void SurfFeature::GetDenseSurf(const cv::Mat& image)
{
	/************************** method 1: 分块提取SURF特征 ***************************************/
	int imageRows = image.rows;
	int imageCols = image.cols;
	if( (imageRows < cwlDiameter) || (imageCols < cwlDiameter) )
	{
		return;
	}
	int row_begin = cwlStep;
	int row_end = imageRows - cwlStep + 1;
	int col_begin = cwlStep;
	int col_end = imageCols - cwlStep + 1;
	
	std::vector<cv::KeyPoint> denseKeyPoints;
	
	for(int r = row_begin; r <= row_end; r += cwlStep)
	{
		for(int c = col_begin; c <= col_end; c += cwlStep)
		{
			denseKeyPoints.push_back(cv::KeyPoint(c, r, cwlDiameter, 0));
		}
	}

	/************************* method 2: 关键点提取SURF特征 **********************************************/
	//std::vector<cv::KeyPoint> denseKeyPoints;
	//int minHessian = 400;
	//cv::SurfFeatureDetector detector( minHessian );
	//detector.detect(image, denseKeyPoints);
	//cv::SurfDescriptorExtractor extractor;
	//extractor.compute(image,denseKeyPoints,SurfMat);
}


cv::Mat  SurfFeature::GetDenseSurf2(const cv::Mat& image)
{
	/************************** method 1: 分块提取SURF特征 ***************************************/
	int imageRows = image.rows;
	int imageCols = image.cols;

	//cv::Mat SurfMat;
	std::vector<cv::KeyPoint> denseKeyPoints;
	if( imageRows >= cwlDiameter && imageCols >= cwlDiameter )
	{
		int row1 = cwlDiameter/2;
		int row2 = imageRows - (cwlDiameter/2) + 1;
		int col1 = cwlDiameter/2;
		int col2 = imageCols - (cwlDiameter/2) + 1;

		for( int row = row1; row <= row2; row += cwlStep )
			for( int col = col1; col <= col2; col += cwlStep )
				denseKeyPoints.push_back(cv::KeyPoint(col,row,cwlDiameter,0));

		cv::SurfDescriptorExtractor surfDescriptorExtractor;
		surfDescriptorExtractor.compute(image,denseKeyPoints,SurfMat);
	}
	return SurfMat;

	/************************* method 2: 关键点提取SURF特征 **********************************************/
	//std::vector<cv::KeyPoint> denseKeyPoints;
	//int minHessian = 400;
	//cv::SurfFeatureDetector detector( minHessian );
	//detector.detect(image, denseKeyPoints);
	//cv::SurfDescriptorExtractor extractor;
	//extractor.compute(image,denseKeyPoints,SurfMat);
	//return SurfMat;
}

void SurfFeature::Mat2Array()
{
	//统计非零行
	std::vector<int> nonZeroRow;
	int countNonZero= 0;
	for(int r = 0; r < SurfMat.rows; r++)
	{
		int num_zero = cv::countNonZero(SurfMat.row(r));
		if(num_zero != 0)
		{
			countNonZero++;
			nonZeroRow.push_back(r);
		}
	}

	//决定feature的维数
	rows = countNonZero;
	cols = SurfMat.cols;

	//将矩阵转换为数组
	SurfArray = newArray();
	for(size_t i = 0; i < nonZeroRow.size(); i++)
	{
		memcpy(SurfArray[i], SurfMat.ptr<float>(nonZeroRow[i]), cols*sizeof(float));
	}
}

//给二维数组分配内存
float** SurfFeature::newArray()
{
	float **Array = new float *[rows];
	for(int r = 0; r < rows; r++)
	{
		Array[r] = new float[cols];
		memset(Array[r], 0, cols*sizeof(float));
	}
	return Array;
}

//销毁二维数组
void SurfFeature::FreeFeature(float **Array)
{
	for(int r = 0; r < rows; r++)
	{
		delete [cols]Array[r];
		Array[r] = NULL;
	}
	delete [rows]Array;
	Array = NULL;
}
//销毁一维数组
void SurfFeature::FreeFeature(float  *Array){
	delete [] Array;
	Array = NULL;
}
