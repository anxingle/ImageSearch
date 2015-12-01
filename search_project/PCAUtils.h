#ifndef _PCA_UTILS_H_
#define _PCA_UTILS_H_
#include <string>
#include <opencv2/opencv.hpp>
using namespace cv;

void matProject(Mat& eigVec, Mat& mean, Mat& trainSet, int dimAfterPCA, Mat& featureAfterPCA);

void vecProject(Mat& eigenVectors, Mat& meanValues, float* src, int srcSize, int dstSize, float* dst);

void pcaTrain(Mat& trainSet, std::string meanFile, std::string eigenValuesFile, std::string eigenVectorFile, std::string featureAfterPcaFile);

void pcaProject(float* feature, float* pcaFeature);

void rotateProject(float* pcaFeature, bitvec rotationFeature);

#endif