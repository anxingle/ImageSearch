#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "Test.h"
#include "Configure.h"
#include "Utils.h"
#include "ImgConAna.h"
#include "ImgFeature.h"
#include "LoadFile.h"
#include "NearestNeighbors.h"
#include "PCAUtils.h"

using namespace cv;

void test(string labelFile, 
		  string queryFile, 
		  string centerFile,
		  string type,
		  string featureBinFile, 
		  string imageNameFile,
		  string pcaMatrixFile,
		  string rotateMatrixFile = "") {
	// load ground truth
	unordered_map<string, unordered_set<string> > categoryLabels;
	ifstream labelsInfile(labelFile);
	for (int i = 0; i < categoryNum; ++i){
		string categ;
		int imgNum;
		labelsInfile >> categ;
		labelsInfile >> imgNum;
		for (int j = 0; i < imgNum; ++j) {
			string img;
			labelsInfile >> img;
			categoryLabels[categ].insert(img);
		}
	}
	labelsInfile.close();
	// load queries
	vector<pair<string, string> > queries;
	ifstream queryInfile(queryFile);
	int queryNum;
	queryInfile >> queryNum;
	for (int i = 0; i < queryNum; ++i) {
		string name, categ;
		queryInfile >> name >> categ;
		queries.push_back(make_pair(name, categ));
	}
	queryInfile.close();
	// load vocabulary file
	vocabulary = newMatrix(clusterCount, surfDescriptorSize);
	loadVocabulary(centerFile, vocabulary, clusterCount, surfDescriptorSize);
	// load pca file
	pcaMatrix = newMatrix(featureAfterPCASize, surfVladFeatureSize);
	loadPCAMatrix(pcaMatrixFile, pcaMatrix, featureAfterPCASize, surfVladFeatureSize);
	if (type == 'Euler') {
		// load train images' features and names
		vector<pair<float*, string> > trainFeatures;
		loadTrainFeatures(imageNameFile, featureBinFile, trainFeatures);
		testWithEuler(queries, trainFeatures, categoryLabels);
	}
	if (type == 'Hamming') {
		// rotation matrix
		rotationMatrix = newMatrix(featureAfterPCASize, featureAfterPCASize);
		loadRotationMatrix(rotateMatrixFile, rotationMatrix, featureAfterPCASize, featureAfterPCASize);
		// load train images' bit features and names
		vector<pair<bitvec, string> > trainFeatures;
		loadTrainBitFeatures(imageNameFile, featureBinFile, trainFeatures);
		testWithHamming(queries, trainFeatures, categoryLabels);
	}
}


double computeMAP(unordered_set<string>& labels, vector<string>& neighbors) {
	int k = neighbors.size();
	double ap = 0.0;
	int c = 0;
	for (int i = 0; i < k; ++i) {
		if (labels.count(neighbors[i])) {
			++c;
			ap += c / double(i + 1); 
		}
	}
	ap = ap / k;
	return ap;
}


void testWithEuler(vector<pair<string, string> >& queries, 
				   vector<pair<float*, string> >& trainFeatures,
				   unordered_map<string, unordered_set<string> >& categoryLabels) {
	int n = queries.size();
	ofstream errorLog("log.txt");
	double mAP = 0.0; 
	for (int i = 0; i < n; ++i) {
		clock_t t0 = clock();
		string path = testPath + "\\" + queries[i].first;
		Mat imgSrc = imread(path);
		// 归一化
		Mat stdImg;
		resize(imgSrc, stdImg, Size(320, 320));
		// 提取surf&vlad特征
		ImgFeature feature(stdImg);
		bool extractSucc = feature.computeSurfVladFeature(vocabulary, clusterCount, surfDescriptorSize);
		if (!extractSucc) errorLog << path << ":" << i << " error." << endl;
		float* featureArray = feature.GetFeature();
		float* pcaFeature = new float(featureAfterPCASize);
		pcaProject(featureArray, pcaFeature);
		vector<string> neighbors;
		computeKNearestImageAli(pcaFeature, trainFeatures, neighbors);
		cout << path << " : " << i << " Euler Time is " << double(clock()) / CLOCKS_PER_SEC << " s" << endl; 
		mAP += computeMAP(categoryLabels[queries[i].second], neighbors);
		feature.FreeFeature(featureArray);
		freeArray(pcaFeature);
	}
	mAP = mAP / n;
	cout << "mAP@100 : " << mAP << endl;
	errorLog.close();
}

void testWithHamming(vector<pair<string, string> >& queries, 
				   vector<pair<bitvec, string> >& trainFeatures,
				   unordered_map<string, unordered_set<string> >& categoryLabels) {
	int n = queries.size();
	ofstream errorLog("log.txt");
	double mAP = 0.0; 
	for (int i = 0; i < n; ++i) {
		clock_t t0 = clock();
		string path = testPath + "\\" + queries[i].first;
		Mat imgSrc = imread(path);
		// 归一化
		Mat stdImg;
		resize(imgSrc, stdImg, Size(320, 320));
		// 提取surf&vlad特征
		ImgFeature feature(stdImg);
		bool extractSucc = feature.computeSurfVladFeature(vocabulary, clusterCount, surfDescriptorSize);
		if (!extractSucc) errorLog << path << ":" << i << " error." << endl;
		float* featureArray = feature.GetFeature();
		float* pcaFeature = new float(featureAfterPCASize);
		pcaProject(featureArray, pcaFeature);
		bitvec rotationFeature;
		rotateProject(pcaFeature, rotationFeature);
		vector<string> neighbors;
		computeKNearestImageAli(rotationFeature, trainFeatures, neighbors);
		cout << path << " : " << i << " Hamming Time is " << double(clock()) / CLOCKS_PER_SEC << " s" << endl; 
		mAP += computeMAP(categoryLabels[queries[i].second], neighbors);
		feature.FreeFeature(featureArray);
		freeArray(pcaFeature);
	}
	mAP = mAP / n;
	cout << "mAP@100 : " << mAP << endl;	
	errorLog.close();
}