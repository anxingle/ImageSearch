#include "stdafx.h"
#include "Preprocess.h"


//将长边固定为：max_W_H
IplImage* zoomImgByMaxSide(IplImage *src, const int max_W_H)
{
	int width = src->width;
	int height = src->height;
	if (height <= width)
	{
		IplImage* dst = cvCreateImage(cvSize(max_W_H, (int)(height * max_W_H / width)), src->depth, src->nChannels);
		cvResize(src, dst, CV_INTER_LINEAR);
		return dst;
	}
	else
	{
		IplImage* dst = cvCreateImage(cvSize((int)(width * max_W_H / height), max_W_H), src->depth, src->nChannels);
		cvResize(src, dst, CV_INTER_LINEAR);
		return dst;
	}
}
