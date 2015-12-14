#include "stdafx.h"
#include <fstream>
#include "PCAUtils.h"
#include "Configure.h"
using namespace std;


void matProject(Mat& eigVec, Mat& mean, Mat& trainSet, int dimAfterPCA, Mat& featureAfterPCA) {
	//int row = trainSet.rows;
	//int col = trainSet.cols;
	//for (int r = 0; r < row; ++r) {
	//	for (int c = 0; c < dimAfterPCA; ++c) {
	//		float sum = 0.0;
	//		for (int i = 0; i < col; ++i) 
	//			sum += eigVec.at<float>(c, i) * (trainSet.at<float>(r, i) - mean.at<float>(0, i));
	//		featureAfterPCA.at<float>(r, c) = sum;
	//	}
	//}
	Mat vec = eigVec(Range(0,dimAfterPCA), Range::all());
	featureAfterPCA = trainSet * vec.t();
}

void vecProject(Mat& eigenVectors, Mat& meanValues, float* src, int srcSize, int dstSize, float* dst) {
	for (int c = 0; c < dstSize; ++c) {
		float sum = 0.0;
		for (int i = 0; i < srcSize; ++i) {
			sum += eigenVectors.at<float>(c, i) * (src[i] - meanValues.at<float>(0, i));
		}
		dst[c] = sum;
	}
}

void pcaTrain(Mat& trainSet, 
			  string meanFile, 
			  string eigenValuesFile, 
			  string eigenVectorsFile, 
			  string featureAfterPcaFile) {
	// PCA, 得到mean, eigenvectors(均值占一行，特征向量按行排列)
	PCA pca(trainSet, Mat(), CV_PCA_DATA_AS_ROW);
	// 计算PCA降维后的特征
	Mat featureAfterPCA(trainSet.rows, featureAfterPCASize, CV_32F);
	matProject(pca.eigenvectors, pca.mean, trainSet, featureAfterPCASize, featureAfterPCA);
	// save files
	// save mean values
	mat2Bin(meanFile.c_str(), pca.mean, 1, surfVladFeatureSize);
	// save eigen values
	ofstream eigenValueStream(eigenValuesFile.c_str());
	for (int i = 0; i < min(surfVladFeatureSize, trainSet.rows); ++i) eigenValueStream << pca.eigenvalues.at<float>(i, 0) << " ";
	eigenValueStream << endl;
	eigenValueStream.close();
	// save eigen vectors
	mat2Bin(eigenVectorsFile.c_str(), pca.eigenvectors, featureAfterPCASize, surfVladFeatureSize);
	// save features after PCA
	mat2Bin(featureAfterPcaFile.c_str(), featureAfterPCA, trainSet.rows, featureAfterPCASize);
}

void pcaProject(float* feature, float* pcaFeature) {
	for (int i = 0; i < featureAfterPCASize; ++i) {
		pcaFeature[i] = 0.0;
		for (int j = 0; j < surfVladFeatureSize; ++j) {
			pcaFeature[i] += pcaMatrix[i][j] * feature[j];
		}
	}
}

void rotateProject(float* pcaFeature, bitset<nbits>& rotationFeature) {
	for (int i = 0; i < featureAfterPCASize; ++i) {
		float val = 0.0;
		for (int j = 0; j < featureAfterPCASize; ++j) {
			val += rotationMatrix[j][i] * pcaFeature[j];
		}
		if (val > 0) rotationFeature.set(i, 1); 
	}
}