#ifndef _VLADFEATURE_H
#define _VLADFEATURE_H


class VLADExtractor
{
public:
	VLADExtractor(float** vocabulary, int voc_rows, int voc_cols);
	~VLADExtractor();
	float* ExtractVLADFeature(float** feature, int feature_rows, int feature_cols);
	void FreeVLADFeature(float* array);
	int GetDimension(){return m_nDimension;}
	int GetClusterCenters(){return m_nClusterCenters;}
	int GetFeatureDim(){return m_nFeatureDim;}
private:
	int m_nDimension;              //特征向量维数
	int m_nClusterCenters;  	  //码本个数
	float** m_vocabulary;		//码本
	float** vlad_feature;       //存储计算后的vlad特征
	float* Array_feature;       //将vlad_feature转换为一维数组赋给Array_feature
	int m_nFeatureDim;          //Array_feature的维数，m_nClusterCenters*m_nDimension


	//计算vlad特征
	void computeFeature(float** feature, int feature_rows);
	//计算码本m_vocabulary中距离向量feature_i最近的聚类中心
	int getNearestCenterLabel(float* feature_i);
	//累加feature和聚类中心i的差值
	void accumulateVlad(float* feature, int center_i);
	//计算两个向量的欧式距离,其长度为m_nDimension
	double euclidean(float *array1, float *array2);
	//将vlad_feature转换为一维向量Array_feature
	void toArray();
	//L2-norm
	void L2_Norm();
	//SSR_norm
	void SSR_Norm(float alpha);
	//uSSR-norm
	void uSSR_Norm(float alpha);

	//给二维数组分配内存
	float **newArray(int row, int col);
	//销毁二维数组
	void deleteArray(float **Array, int row, int col);

};



#endif