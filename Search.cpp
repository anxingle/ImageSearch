#include "stdafx.h"
#include <fstream>
#include "Search.h"
#include "ImgConAna.h"
#include "ImgFeature.h"
#include "Configure.h"
#include "NearestNeighbors.h"
#include "Utils.h"

void search(Mat& imgSrc, vector<QueryRe >& disMap,FEAFLAGS flags) {
	if (!disMap.empty()) disMap.clear();

	// 提取proposals
	if(!(QueryPro.size()==0)){QueryPro.clear();}

	//vector<Mat> proposals;

	if(flags==DENSESURF){
		Rect rect;
		ostuRemoveBorderRect(imgSrc,rect);
		Mat tmp = imgSrc(rect);
		QueryPro.push_back(tmp);
	}
	else{
		proposalMat(imgSrc, QueryPro);
	}


	// 提取proposals的特征
	size_t pNums = QueryPro.size();
	vector<float* > testFeatures;
	for (size_t i = 0; i < pNums; ++i) {
		Mat stdImg;
		resize(QueryPro[i], stdImg, Size(240, 260));

		//提取Surf特征
		if(flags==DENSESURF){
			IplImage tmp = IplImage(stdImg);
			SurfFeature feature;
			feature.computeFeature(&tmp);
			float * featureSurf = feature.GetFeature();
			testFeatures.push_back(featureSurf);
		}
		else{
			// 提取Surf&VLAD特征
			ImgFeature feature(stdImg);
			bool extractSucc = feature.computeSurfVladFeature(vocabulary, clusterCount, surfDescriptorSize);
			float* featureArray = feature.GetFeature(); 
			testFeatures.push_back(featureArray);
		}
	}

#if DEBUG
	ofstream testFeatureFile("..\\..\\testFeature.txt");
	testFeatureFile << "15818008.jpg" << endl;
	testFeatureFile << pNums << endl;
	for (int r = 0; r < pNums; ++r) {
		for (int c = 0; c < clusterCount * surfDescriptorSize; ++c) {
			testFeatureFile << testFeatures[r][c] << " ";
		}
		testFeatureFile << endl;
	}
#endif

	// K最近邻检索
	if(flags==DENSESURF){
		computeKNearestImage(testFeatures, trainFeatureDenseSurfMap, denseSurfFeatureSize, nearestK, disMap);
	}
	else{
		computeKNearestImage(testFeatures, trainFeatureProVladMap, surfVladFeatureSize, nearestK, disMap);
	}
	// release
	freeVectorArray(testFeatures);
}