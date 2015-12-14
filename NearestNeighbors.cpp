#include "stdafx.h"
#include "NearestNeighbors.h"
#include <map>
#include <algorithm>
#include <functional>

bool compDouble (pair<string, double>& p1, pair<string, double>& p2) {
	return p1.second < p2.second;
}

bool compInt(pair<string, int>& p1, pair<string, int>& p2) {
	return p1.second < p2.second;
}

//计算两个向量的欧式距离
double vecDistance(float *array1, float *array2, int len)
{
	double result = 0.0;
	for(int i = 0; i < len; i++)
	{
		result += (array1[i] - array2[i]) * (array1[i] - array2[i]);
	}
	return sqrt(result);
}

double computeDistance(vector<float*>& testFeature, vector<float*>& trainFeature, int featureSize,int &IdxQuery,int &IdxDB) {
	/* method 1: 取最小距离*/
	
	double minDis = numeric_limits<double>::max();
	for (int i = 0; i < testFeature.size(); ++i) {
		for (int j = 0; j < trainFeature.size(); ++j) {
			double dis = vecDistance(testFeature[i], trainFeature[j], featureSize);
			if (dis < minDis) {
				IdxQuery = i;IdxDB = j;
				minDis = dis;
			}
		}
	}
	return minDis;
	

	/*method 2: 最小距离取平均值*/
	//double minDisSum = 0.0;
	//for (int i = 0; i < testFeature.size(); ++i) {
	//	double minDis = numeric_limits<double>::max();
	//	for (int j = 0; j < trainFeature.size(); ++j) {
	//		double dis = vecDistance(testFeature[i], trainFeature[j], featureSize);
	//		if (dis < minDis) minDis = dis;
	//	}
	//	minDisSum += minDis;
	//}
	//return minDisSum / testFeature.size();
}

void insertDis(string path, double dis,int IdxQuery,int IdxDB, vector<QueryRe >& disMap) {

	QueryRe tmp;
	tmp.PathDis = make_pair(path,dis);
	tmp.IdxDB = IdxDB;
	tmp.IdxQuery = IdxQuery;

	disMap.push_back(tmp);
	int n = disMap.size();
	int cur = n - 1;
	for (int i = n - 2; i >= 0; --i) {
		if (disMap[i].PathDis.second > dis) {
			swap(disMap[i], disMap[cur]);
			cur = i;
		} else {
			 break;
		}
	}
}

void computeKNearestImage(vector<float*>& testFeature, 
						  unordered_map<string, vector<float*> >& trainFeatureMap,
						  int featureSize,
						  int K,
						  vector<QueryRe >& disMap) {
	for (auto& trainImage : trainFeatureMap) {
		string trainImagePath = trainImage.first;
		vector<float* > trainFeature = trainImage.second;
		int IdxQuery = 0,IdxDB = 0;
		double dis = computeDistance(testFeature, trainFeature, featureSize,IdxQuery,IdxDB);

		if (disMap.size() < K) {
			insertDis(trainImagePath, dis,IdxQuery,IdxDB, disMap);
		} else {
			if (disMap[K - 1].PathDis.second > dis) {
				disMap.pop_back();
				insertDis(trainImagePath, dis,IdxQuery,IdxDB, disMap);
			}
		}
	}
}

void insertDoubleDis(double dis, vector<pair<string, double> > &res) {
	int n = res.size();
	int pos = n - 1;
	for (int i = n - 2; i >= 0; --i) {
		if (dis < res[i].second) {
			swap(res[i], res[pos--]);
		} else {
			break;
		}
	}
}

void insertIntDis(int dis, vector<pair<string, int> >& res) {
	int n = res.size();
	int pos = n - 1;
	for (int i = n - 2; i >= 0; --i) {
		if (dis < res[i].second) {
			swap(res[i], res[pos--]);
		} else {
			break;
		}
	}
}

void computeKNearestImageAli(float* testFeature, 
							 vector<pair<float*, string> >& trainFeatures, 
							 vector<string>& neighbors) {
	vector<pair<string, double> > res;
	int n = trainFeatures.size();
	for (int i = 0; i < n; ++i) {
		double dis = vecDistance(testFeature, trainFeatures[i].first, featureAfterPCASize);
		//if (res.size() < nearestK) {
		//	res.push_back(make_pair(trainFeatures[i].second, dis));
		//} else {
		//	sort(res.begin(), res.end(), compDouble);
		//	if (dis < res[nearestK - 1].second) {
		//		res[nearestK - 1] = make_pair(trainFeatures[i].second, dis);
		//	}
		//}
		if (res.size() < nearestK) {
			res.push_back(make_pair(trainFeatures[i].second, dis));
			insertDoubleDis(dis, res);
		} else {
			if (dis < res[nearestK - 1].second) {
				res[nearestK - 1] = make_pair(trainFeatures[i].second, dis);
				insertDoubleDis(dis, res);
			}
		}
	}
	assert(res.size() == nearestK);
	for (int i = 0; i < nearestK; ++i) neighbors.push_back(res[i].first);
}

int hammingDistance(bitset<nbits>& bv1, bitset<nbits>& bv2) {
	return (bv1 ^ bv2).flip().count();
}
void computeKNearestImageAli(bitset<nbits>& testFeature, 
							 vector<pair<bitset<nbits>, string> >& trainFeatures, 
							 vector<string>& neighbors) {
	vector<pair<string, int> > res;
	int n = trainFeatures.size();
	for (int i = 0; i < n; ++i) {
		int dis = hammingDistance(testFeature, trainFeatures[i].first);
		if (res.size() < nearestK) {
			res.push_back(make_pair(trainFeatures[i].second, dis));
			insertIntDis(dis, res);
		} else {
			if (dis < res[nearestK - 1].second) {
				res[nearestK - 1] = make_pair(trainFeatures[i].second, dis);
				insertIntDis(dis, res);
			}
		}

	}
	assert(res.size() == nearestK);
	for (int i = 0; i < nearestK; ++i) neighbors.push_back(res[i].first);
}