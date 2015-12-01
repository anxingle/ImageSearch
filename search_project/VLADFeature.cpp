#include <cmath>
#include <cassert>
#include <iostream>
#include "VLADFeature.h"


VLADExtractor::VLADExtractor(float** vocabulary, int voc_rows, int voc_cols)
{
	// 64 feature vector dim
	m_nDimension = voc_cols;
	// 256 vocabulary num
	m_nClusterCenters = voc_rows;
	m_vocabulary = vocabulary;

	m_nFeatureDim = m_nClusterCenters * m_nDimension;
	
}

VLADExtractor::~VLADExtractor()
{
	
}

float* VLADExtractor::ExtractVLADFeature(float** feature, int feature_rows, int feature_cols)
{
	// m_nDimension特征向量的维数
	assert( feature_cols == m_nDimension );
	vlad_feature = newArray(m_nClusterCenters, m_nDimension);
	computeFeature(feature, feature_rows);
	toArray();
	SSR_Norm(0.3f);
	//L2_Norm();
	deleteArray(vlad_feature, m_nClusterCenters, m_nDimension);
	return Array_feature;

}

void VLADExtractor::FreeVLADFeature(float* array)
{
	if(array!=NULL)
	{
		delete[] array;
		array = NULL;
	}
}

//计算vlad特征
void VLADExtractor::computeFeature(float** feature, int feature_rows)
{
	for(int row = 0; row < feature_rows; row++)
	{
		int nearesetCenter = getNearestCenterLabel(feature[row]);
		accumulateVlad(feature[row], nearesetCenter);
	}
}

//计算码本m_vocabulary中距离向量feature_i最近的聚类中心
int VLADExtractor::getNearestCenterLabel(float* feature_i)
{
	int nearestCenter = 0;
	double min_distance = euclidean(feature_i, m_vocabulary[0]);
	for(int center_i = 1; center_i < m_nClusterCenters; center_i++)
	{
		double distance = euclidean(feature_i, m_vocabulary[center_i]);
		if(distance < min_distance)
		{
			min_distance = distance;
			nearestCenter = center_i;
		}
	}
	return nearestCenter;
}

//累加feature_i和聚类中心i的差值
void VLADExtractor::accumulateVlad(float* feature, int center_i)
{
    float* vlad_feature_i = vlad_feature[center_i];
	const float* vocabulary_i = m_vocabulary[center_i];
	for(int col = 0; col < m_nDimension; col++)
	{
		vlad_feature_i[col] += feature[col] - vocabulary_i[col];
	}

}

//计算两个向量的欧式距离,其长度为m_nDimension
double VLADExtractor::euclidean(float *array1, float *array2)
{
	double result = 0.0;
	for(int i = 0; i < m_nDimension; i++)
	{
		result += (array1[i] - array2[i]) * (array1[i] - array2[i]);
	}
	return result;
}

//将vlad_feature转换为一维向量Array_feature
void VLADExtractor::toArray()
{
	Array_feature = new float[m_nFeatureDim];
	int i = 0;
	for(int row = 0; row < m_nClusterCenters; row++)
	{
		for(int col = 0; col < m_nDimension; col++)
		{
			Array_feature[i] = vlad_feature[row][col];
			i++;
		}
	}
}

//L2-norm
void VLADExtractor::L2_Norm()
{
	double sum = 0.0;
	for(int i = 0; i < m_nFeatureDim; i++)
	{
		sum += Array_feature[i]*Array_feature[i];
	}
	sum = sqrt(sum);
	for(int i = 0; i < m_nFeatureDim; i++)
	{
		Array_feature[i] = static_cast<float>(Array_feature[i]/sum);
	}

}
//SSR_norm
void VLADExtractor::SSR_Norm(float alpha)
{
	for(int i = 0; i < m_nFeatureDim; i++)
	{
		float data = Array_feature[i];
		int sign =  data > 0 ? 1 : -1;
		Array_feature[i] = sign*pow(sign*data,alpha);
	}
	L2_Norm();
}
//uSSR-norm
void VLADExtractor::uSSR_Norm(float alpha)
{
	for(int i = 0; i < m_nFeatureDim; i++)
	{
		float data = Array_feature[i];
		int sign =  data > 0 ? 1 : -1;
		Array_feature[i] = pow(sign*data,alpha);
	}
	L2_Norm();
}

//给二维数组分配内存
float** VLADExtractor::newArray(int row, int col)
{
	float **Array = new float *[row];
	for(int r = 0; r < row; r++)
	{
		Array[r] = new float[col];
		memset(Array[r], 0, col*sizeof(float));
	}
	return Array;
}

//销毁二维数组
void VLADExtractor::deleteArray(float **Array, int row, int col)
{
	for(int r = 0; r < row; r++)
	{
		delete [col]Array[r];
		Array[r] = NULL;
	}
	delete [row]Array;
	Array = NULL;
}