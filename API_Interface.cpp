#include "stdafx.h"
#include "API_Interface.h"
#include "Configure.h" 
#include <string> 
#include <fstream>
using namespace std;


void  GlobalInit(string labelFile,
			   string centerFile,
			   string pcaMatrixFile,
			   string imageNameFile,
			   string featureBinFile,
			   string rotateMatrixFile 
			   ){

	vocabulary = newMatrix(clusterCount, surfDescriptorSize);
	pcaMatrix = newMatrix(featureAfterPCASize, surfVladFeatureSize);	
	rotationMatrix = newMatrix(featureAfterPCASize, featureAfterPCASize);
	
	ifstream labelsInfile(labelFile);
	for (int i = 0; i < categoryNum; ++i){
		string categ;
		int imgNum;
		labelsInfile >> categ;
		labelsInfile >> imgNum;
		for (int j = 0; j < imgNum; ++j) {
			string img;
			labelsInfile >> img;
			categoryLabels[categ].insert(img);
		}
	}
	labelsInfile.close();

	loadVocabulary(centerFile, vocabulary, clusterCount, surfDescriptorSize);

	loadPCAMatrix(pcaMatrixFile, pcaMatrix, featureAfterPCASize, surfVladFeatureSize);

	loadTrainFeatures(imageNameFile, featureBinFile, trainFeatures);

	loadRotationMatrix(rotateMatrixFile, rotationMatrix, featureAfterPCASize, featureAfterPCASize);
	return ; 
}

int Retrieval(Mat img){

	if(!(IRResult.size()==0)){IRResult.clear();}

	search(img,IRResult,feaflags);

	return 1;

}

int ShowToMFC(){

	//初始化
	if(!(SimRe.size()==0)){SimRe.clear();}
	if(!(ShowImgs.size()==0)){ShowImgs.clear();}

	//DENSESURF结果
	if(feaflags==DENSESURF){
		if((pageIdx*ImgNumPerPage*3)>IRResult.size()){
			cout<<"显示图片不够"<<endl;
			return -1;
		}
		int start = (pageIdx-1)*ImgNumPerPage*3;
		for(int i = start;i<(start+ImgNumPerPage*3);i++){
			double sim = IRResult[i].PathDis.second;
			string path = IRResult[i].PathDis.first;
			string name = splitFileName(path);
			Mat tmp = imread(path.c_str());
			SimRe.push_back(sim);
			ShowImgs.push_back(tmp);
			ImgName.push_back(name);
		}
	}
	if(feaflags==DENSESURF_PRO_VLAD){
		if((pageIdx*ImgNumPerPage)>IRResult.size()){
			cout<<"显示图片不够"<<endl;
			return -1;
		}
		int start = (pageIdx-1)*ImgNumPerPage;
		for(int i = start;i<(start+ImgNumPerPage);i++){
			double sim = IRResult[i].PathDis.second;
			SimRe.push_back(sim);

			string path = IRResult[i].PathDis.first;
			string name = splitFileName(path);
			Mat tmp = imread(path.c_str());
			ShowImgs.push_back(tmp);
			vector<Mat> proTmp;
			proposalMat(tmp,proTmp);

			//保存最相似的proposal
			int qIdx = IRResult[i].IdxQuery;
			int DBIdx = IRResult[i].IdxDB;
			ShowImgs.push_back(QueryPro[qIdx]);
			ShowImgs.push_back(proTmp[DBIdx]);
			ImgName.push_back(name);
		}
	}
	return 1;
}