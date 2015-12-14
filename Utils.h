#ifndef _UTILS_H_
#define _UTILS_H_
#include <string>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <opencv2/opencv.hpp>
#include "Configure.h"
using namespace std;

typedef std::vector<std::bitset<8> > bitvec;

#define DEBUG 0

void getFiles(std::string path, std::vector<std::string>& files);
std::string intToString(int i);
std::string splitFileName(std::string file);
int computeProposalNums(std::string imageFilePath);
float* newArray(int col);
void freeArray(float* array);
void freeVectorArray(std::vector<float*>& arrays);
float** newMatrix(int row, int col);
void freeMatrix(float** matrix, int row);
void freeFeatureMap(std::unordered_map<std::string, std::vector<float*> >& imageMap);
void freeFeatureVec(std::vector<pair<float*, std::string> >& features);
void featureBinToMat(std::string featureFile, cv::Mat& trainSet, int row, int col);
void loadFeatureBin(std::string trainFile, 
						   std::string featureFile, 
						   int featureSize,
						   std::unordered_map<std::string, std::vector<float*> >& featureMap,
						   bool head);
void loadVocabulary(std::string file, float** matrix, int row, int col);
void loadPCAMatrix(std::string pcaFile, float** matrix, int row_, int col_);
void loadRotationMatrix(string rotateMatrixFile, float** matrix, int row_, int col_);

void loadTrainFeatures(string imageNameFile, string featureBinFile, vector<pair<float*, string> >& trainFeatures);
void loadTrainBitFeatures(string imageNameFile, string featureBinFile, vector<pair<bitset<nbits>, string> >& trainFeatures);

void feature2Bin(float* feature, int featureSize, FILE* fin);
void mat2Bin(const char* filename, const cv::Mat& src, int row, int col);
void bin2Mat(const char* filename, cv::Mat& dst);

#endif