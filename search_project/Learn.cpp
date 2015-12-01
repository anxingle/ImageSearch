#include <vector>
#include <time.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <assert.h>
#include "Learn.h"
#include "Utils.h"
#include "ImgConAna.h"
#include "SurfFeature.h"
#include "Configure.h"
using namespace cv;

void learnVocabulary(string imagePath, string sampleFile, string centerFile, FEAFLAGS flags) {
	vector<string> files;
	/*getFiles(imagePath, files);*/
	ifstream sampling(sampleFile);
	assert(sampling);
	string name;
	while (true) {
		sampling >> name;
		if (name == "end") break;
		string file = imagePath + "\\" + name;
		files.push_back(file);
	}
	cout << "Files read end." << endl;
	sampling.close();

	clock_t start;
	start = clock();
	
	string featureBinFile = "samplingFeature.bin";
	FILE* fin;
	fin = fopen(featureBinFile.c_str(), "wb");

	int featureMatRows = 0;
	vector<float* > featureArray;
	size_t imgNums = files.size();
	for (size_t n = 0; n < imgNums; ++n) {
		Mat imgSrc = imread(files[n]);
		vector<Mat> proposals;
		proposals.push_back(imgSrc);
		//proposalMat(imgSrc,proposals);
		size_t pNums = proposals.size();
		featureMatRows += pNums;
		for (size_t i = 0; i < pNums; ++i) {
			IplImage img = IplImage(proposals[i]);
			// 归一化图片
			IplImage* stdImg= cvCreateImage( cvSize(320,320), IPL_DEPTH_8U, img.nChannels);
			cvResize(&img, stdImg, CV_INTER_LINEAR);
			// 提取Surf特征
			SurfFeature sfeature;
			sfeature.computeFeature(stdImg);
			float* feature = sfeature.GetFeature();
			featureArray.push_back(feature);
			cvReleaseImage(&stdImg);
		}
		cout << files[n] << ":" << n << " end." << endl;
	}
	Mat featureMat(featureMatRows * surfBlockSize, surfDescriptorSize, CV_32F);
	for (int r = 0; r < featureMatRows; ++r) {
		float* feature = featureArray[r];
		for (int b = 0; b < surfBlockSize; ++b) {
			float* localFeat = new float[surfDescriptorSize];
			for (int c = 0; c < surfDescriptorSize; ++c) {
				featureMat.at<float>(r * surfBlockSize + b, c) = feature[b * surfDescriptorSize + c];
				localFeat[c] = feature[b * surfDescriptorSize + c];
			}
			feature2Bin(localFeat, surfDescriptorSize, fin);
			delete [] localFeat;
		}
	}
	fclose(fin);
	// 释放Surf特征
	for (int i = 0 ; i < featureMatRows; ++i) freeArray(featureArray[i]);
	
	// K-means聚类码本
	Mat centers(clusterCount, 1, featureMat.type());
	Mat labels;
	double cretia = kmeans(featureMat, clusterCount, labels, TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 100, 0.01), 3, KMEANS_PP_CENTERS, centers);
	cout << "紧凑度： " << cretia << endl;
	ofstream outfile(centerFile.c_str());
	outfile.precision(8);
	for (int r = 0; r < clusterCount; ++r) {
		for (int c = 0; c < surfDescriptorSize; ++c) {
			outfile << centers.at<float>(r, c) << " ";
		}
		outfile << endl;
	}
	outfile.close();
	clock_t end = clock();
	cout << "Total Time: " << (float)(end - start) / CLOCKS_PER_SEC << endl;

}

