#ifndef _H_IMGCONANA_H
#define _H_IMGCONANA_H

#include "opencv2/opencv.hpp"
#include "stdio.h"
#include "math.h"
#include "ostuRemoveBorder.h"


using namespace cv;
using namespace std;

#define AreaThre 0.01
#define REMOVE_BORDER 0

bool SortByArea(const Rect &a,const Rect &b);


bool IsContainer(Rect a,Rect b);
bool IsInterSect(Rect a,Rect b);

double InterSectPercentMin(Rect a,Rect b); // calc the intersection area under the min of two rect for the inner loop last combine 
double InterSectPercent(Rect a,Rect b); // calc the intersection area under the max of two rect for remove the duplicate rect
double NewDistRect(Rect a,Rect b);

int distMatrix(vector<Rect> input,int width,int height,Mat &Dist);

Rect CombineTwoRect(Rect a,Rect b);

//new combine method
int NewCombineRect(vector<Rect> input,int width,int height,vector<Rect> &out);
//get the combine proposal
int proposal(Mat img,vector<Rect> &res);

void saveMat(Mat input,string saveName);

//get rect from binary image
int Edge2Rect(Mat edge,Mat img,vector<Rect> &res);// add the source cut image for debug

int reMinMaxRect(vector<Rect> &TempInput,int width,int height); // remove the max threshold and min threshold rect
int reDupRect(vector<Rect> &input); // remove the duplicate rect 
int combineRestRect(vector<Rect> &restRect,vector<Rect> &out,int width,int height); // combine the rest rect 
int calcIncluRela(vector<int > &index,vector<vector<int > > &classify,vector<Rect> &TempInput); // calc the include relation
int combineInterSectRect(vector<Rect> &TempInput); //combine the intersect rect 

//vector<Mat> proposalMat(Mat img);// interface function for further operation 
int proposalMat(Mat img,vector<Mat> &proposalRes);
#endif