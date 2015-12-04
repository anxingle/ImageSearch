#include "ostuRemoveBorder.h"

using namespace cv;
using namespace std;

int ostuRemoveBorderRect(Mat in,Rect &out){
	Mat input;
	in.copyTo(input);

	if(input.channels()==3){
		cvtColor(input,input,CV_BGR2GRAY);
	}

	Mat BinaryImg;
	threshold(input,BinaryImg,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);

	int width = input.cols,height = input.rows;
	Mat maskIm = Mat::zeros(cvSize(width-2*JUDGE_BORDER,height-2*JUDGE_BORDER),CV_8U);
	copyMakeBorder(maskIm,maskIm,JUDGE_BORDER,JUDGE_BORDER,JUDGE_BORDER,JUDGE_BORDER,BORDER_CONSTANT,1);
	Scalar meanBg = mean(BinaryImg,maskIm);
	double bgValue = meanBg(0);
	bool bg = BG_BLACK;
	if(bgValue>128){
		bg = BG_WHITE;
	}

	//ensure the binary img with white bg
	if(!bg){
		BinaryImg = 255-BinaryImg;
	}
#if ERODE_BINARY
	Mat element = getStructuringElement(MORPH_RECT,cvSize(3,3));
	erode(BinaryImg,BinaryImg,element,cvPoint(-1,-1),1);
#endif
#if DEBUG_MODE
	imwrite("C:\\Users\\qcz\\Desktop\\test_5\\1.jpg",BinaryImg);
#endif
	int top = 0,bottom = height-1;
	int left = 0,right = width-1;

	//find the top
	for (int tb = 0;tb<height;tb++){
		Mat tmp = BinaryImg.row(tb);
		int num = countNonZero(tmp);
		double thre = (double)num/width;
		if(thre<PERTHRESH){
			top = tb;
			break;
		}
	}
	//find the bottom
	for(int bt = height-1;bt>=0;bt--){
		Mat tmp = BinaryImg.row(bt);
		int num = countNonZero(tmp);
		double thre = (double)num/width;
		if(thre<PERTHRESH){
			bottom = bt;
			break;
		}
	}
	//find the left
	for(int lr = 0;lr<width;lr++){
		Mat tmp = BinaryImg.col(lr);
		int num = countNonZero(tmp);
		double thre = (double)num/height;
		if(thre<PERTHRESH){
			left = lr;
			break;
		}
	}
	//find the right
	for(int rl = width-1;rl>=0;rl--){
		Mat tmp = BinaryImg.col(rl);
		int num = countNonZero(tmp);
		double thre = (double)num/height;
		if(thre<PERTHRESH){
			right = rl;
			break;
		}
	}

	top = top-PAD_DIS;top = (top<0)? 0 : top;
	bottom = bottom+PAD_DIS;bottom = (bottom>height-1)? height-1 : bottom;
	left = left-PAD_DIS;left = (left<0)? 0 : left;
	right = right+PAD_DIS;right = (right>width-1)? width-1 :right;
	
	out.x = left;
	out.y = top;
	out.width = right-left+1;
	out.height = bottom-top+1;
	return 1;
	/*Mat outTemp = in(Range(top,bottom+1),Range(left,right+1));
	Mat out(outTemp.size(),in.type());
	outTemp.copyTo(out);
	return out;*/
}