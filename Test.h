#ifndef _TETS_H_
#define _TEST_H_
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <bitset>
#include "Configure.h"
using namespace std;

typedef vector<bitset<8> > bitvec;

/*
void test(string labelFile, 
		  string queryFile,
		  string centerFile,
		  string type,
		  string featureBinFile, 
		  string imageNameFile,
		  string pcaMatrixFile,
		  string rotateMatrixFile = "");
*/

void testWithEuler(string type,
				   cv::Mat imgSrc,
				   vector<pair<float*, string> >& trainFeatures,
				   unordered_map<string, unordered_set<string> >& categoryLabels);

void testWithHamming(vector<pair<string, string> >& queries, 
				   vector<pair<bitset<nbits>, string> >& trainFeatures,
				   unordered_map<string, unordered_set<string> >& categoryLabels);

#endif