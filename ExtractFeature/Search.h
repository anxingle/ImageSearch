#ifndef _SEARCH_H_
#define _SEARCH_H_
#include <string>
#include <vector>
#include "Utils.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void search(Mat& imgSrc, vector<QueryRe >& disMap,FEAFLAGS flags); 

#endif