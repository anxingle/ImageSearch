#ifndef _H_OSTUREMOVEBORDER_H
#define _H_OSTUREMOVEBORDER_H

#include "opencv2/opencv.hpp"

#define BG_BLACK 0
#define BG_WHITE 1
#define ERODE_BINARY 0
#define PAD_DIS 0;
#define JUDGE_BORDER 5
#define DEBUG_MODE 0

#define PERTHRESH 0.96

using namespace cv;


int ostuRemoveBorderRect(Mat in,Rect &out);
#endif