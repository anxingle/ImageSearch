#include "ImgFeature.h"


ImgFeature::ImgFeature(std::string imgFilename)
{
	m_image = cv::imread(imgFilename.c_str());
}

ImgFeature::ImgFeature(const cv::Mat& image)
{
	m_image = image;
}

ImgFeature::~ImgFeature()
{
	
}

bool ImgFeature::computeSurfVladFeature(float** vocabulary, int voc_rows, int voc_cols)
{
	SurfFeature surf;
	surf_feature = surf.GetFeature(m_image);
	surf_rows = surf.GetRows();
	surf_cols = surf.GetCols();
	if (surf_rows == 0) {
		surf_vlad_feature = new float[voc_rows * voc_cols];
		memset(surf_vlad_feature, 0, voc_rows * voc_cols * sizeof(float));
		return false;
	}
	VLADExtractor vlad(vocabulary, voc_rows, voc_cols);
	surf_vlad_feature = vlad.ExtractVLADFeature(surf_feature, surf.GetRows(), surf.GetCols());
	//surf.FreeFeature(surf_feature);
	return true;
}
float* ImgFeature::GetFeature()
{
	return surf_vlad_feature;
}
void ImgFeature::FreeFeature(float* array)
{
	if(array!=NULL)
	{
		delete[] array;
		array = NULL;
	}
}

//-----------surf feature------------
int ImgFeature::GetSurfRows() {
	return surf_rows;
}
int ImgFeature::GetSurfCols() {
	return surf_cols;
}
float **ImgFeature::GetSurfFeature() {
	return surf_feature;
}
void ImgFeature::FreeSurfFeature(float **Array) {
	for(int r = 0; r < surf_rows; r++)
	{
		delete [surf_cols]Array[r];
		Array[r] = NULL;
	}
	delete [surf_rows]Array;
	Array = NULL;
}