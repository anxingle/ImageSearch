#include <iostream>
#include <fstream>
#include <cassert>
#include "Configure.h"
#include "LoadFile.h"
#include "Search.h"
#include "Utils.h"
#include "Train.h"
#include "Learn.h"
#include "Test.h"

using namespace std;
using namespace cv;

int main(){
	//string ImgFold = "D:\\ImageSearch\\train";
	//string CenFile = "../DenSurfProVlad/center.txt";
	//feaflags=DENSESURF_PRO_VLAD;

	////cout<<"start learn the codebook of vlad .."<<endl;
	////learnVocabulary(ImgFold,CenFile,feaflags);

	//string vocPath = CenFile;
	//string imgPathFile = "../DenSurfProVlad/imgPath.txt";
	//string featureBinFile = "../DenSurfProVlad/feaProVlad.bin";

	////cout<<"start get the train feature of  surf+vlad+proposal images .."<<endl;
	////train(vocPath,ImgFold,imgPathFile,featureBinFile,feaflags);


	//feaflags = DENSESURF;
	//string imgPathFileNew = "../DenSurf/imgPath.txt";
	//string featureFileNew = "../DenSurf/feaDenseSurf.bin";

	////cout<<"start get the train feature of dense surf images .."<<endl;
	////train(vocPath,ImgFold,imgPathFileNew,featureFileNew,feaflags);
	//load(CenFile,imgPathFile,featureBinFile,imgPathFileNew,featureFileNew);

	//string testImgPath = "../images/11844681.jpg";
	//Mat img = imread(testImgPath.c_str());
	//vector<QueryRe > out;
	//search(img,out,feaflags);


	//************************************ learn ****************************************//
	//***********************************************************************************//
	//string imagePath = "K:\\qcz\\AliSearch\\train_image\\Competition";
	//// string samplingFile = "K:\\qcz\AliSearch\\train_label\\train_label\\sampling100K.txt";
	//string samplingFile = "sampling100K.txt";
	//string cenFile = "../DenSurfProVlad/center.txt";
	//feaflags=DENSESURF_PRO_VLAD;
	//learnVocabulary(imagePath, samplingFile, cenFile, feaflags);

	//************************************ train *******************************************//
	//Mat A = Mat::eye(10, 10, CV_32S);
	//Mat B = A(Range::all(), Range(0, 11));
	//Mat C = B(Range(5, 9), Range::all());

	//string vocabularyFile = "../DenSurfProVlad/center.txt";
	//string imagePath = "K:\\qcz\\AliSearch\\train_image\\Competition";
	//// string imagePath = "K:\\qcz\\AliSearch\\test_images";
	//string imagePathFile = "../DenSurfProVlad/imgPath.txt";
	//string featureBinFile = "../DenSurfProVlad/surfVladFeature.bin";
	//feaflags=DENSESURF_PRO_VLAD;
	//train(vocabularyFile, imagePath, imagePathFile, featureBinFile, feaflags);
	string labelFile = "F:\\Data\\train_ground_truth.txt";
	string queryFile = "F:\\Data\\query_file.txt";
	string centerFile = "../DenSurfProVlad/center.txt";
	string type = "Euler";
	//string type = "Hamming";
	string featureBinFile = "F:\\Data\\featureAfterPCA.bin";
	//string featureBinFile = "F:\\Data\\featureBits.bin";
	string imageNameFile = "F:\\Data\\imgName.txt";
	string pcaMatrixFile = "F:\\Data\\pcaMatrix.bin";
	string rotateMatrixFile = "F:\\Data\\rotateMatrix.bin";
	test(labelFile, queryFile, centerFile, type, featureBinFile, imageNameFile, pcaMatrixFile);
	//test(labelFile, queryFile, centerFile, type, featureBinFile, imageNameFile, pcaMatrixFile, rotateMatrixFile);
	system("pause");
	return 0;
}