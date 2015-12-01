#ifndef _NEAREST_NEIGHBOR_H_
#define _NEAREST_NEIGHBOR_H_
#include <string>
#include <vector>
#include <unordered_map>
#include "Utils.h"

using namespace std;

void computeKNearestImage(vector<float*>& testFeature, 
						  unordered_map<string, vector<float*> >& trainFeatureMap,
						  int featureSize,
						  int K,
						  vector<QueryRe >& disMap);

void computeKNearestImageAli(float* testFeature, 
							 vector<pair<float*, string> >& trainFeatures, 
							 vector<string>& neighbors);

void computeKNearestImageAli(bitvec& testFeature, 
							 vector<pair<bitvec, string> >& trainFeatures, 
							 vector<string>& neighbors);
#endif