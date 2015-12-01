#include "ImgConAna.h"
#include "io.h"
#include "direct.h"

using namespace cv;
using namespace std;

#define DEBUG_MODE 0
#define DIST_CONSTANT 10000
#define PROPOSAL_NUM 2
#define MAX_PERRECT 0.8	//regard the rect as the whole image and delete it
#define MIN_PERRECT 0.15 // small than the max rect's area * MIN_PERRECT should be removed
#define SELECT_NUM 10 //select the front num of source rect 
#define INTERSECT_THRE 0.1 // intersection percent for the last combine of rect
#define INNER_THRESH 0.15 //classify inner thresh to remove the small rect part in one big rect
#define CYCLE_LOOP_THRE 0.7 // remove the middle cycle when multi cycle occur
#define DUPLICATE_THRE 0.8 // remove the duplicate rect at the last step
#define RESULT_NUM 4 // the most num of rect result 
bool SortByArea(const Rect &a,const Rect &b){
	return (a.area()) > (b.area());
}

//bool comparer(ContourList& a,ContourList& b){
//	return (a.area>b.area);
//}


int proposalMat(Mat img,vector<Mat> &proposalRes){

	vector<Rect> res;
	proposal(img,res);
	for(int i = 0;i<res.size();i++){
		proposalRes.push_back(img(res[i]));
	}
	return 1;
}


int proposal(Mat img,vector<Rect> &res){

	if(img.channels()==3){
		cvtColor(img,img,CV_BGR2GRAY);
	}

	//double t = cvGetTickCount();
	
	int width = img.cols,height = img.rows;

	IplImage ipl = IplImage(img);
	IplImage *dst = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,img.channels());
	cvSmooth(&ipl,dst,CV_GAUSSIAN,7,7);
	Mat tmpImg(&ipl);

	Mat threImg;
	threshold(tmpImg,threImg,0,255,THRESH_BINARY|THRESH_OTSU);
	//imwrite("image/threImg.jpg",threImg);

	//Mat element = getStructuringElement(MORPH_RECT,cvSize(3,3));
	//dilate(threImg,threImg,element,cvPoint(-1,-1),1);
	//imwrite("image\\erode.jpg",threImg);
	// two kinds of bg 
	//vector<Rect> proposalRect;
	Edge2Rect(threImg,img,res);
	Rect temp;
	ostuRemoveBorderRect(img,temp);
	//remove duplicate rect

	//flag for add the remove border rect
	bool flag = 1;
	int idx = 0;
	while(idx<res.size()){
		double interSect = InterSectPercent(temp,res[idx]);
		if(interSect>DUPLICATE_THRE){
			if(temp.area()<res[idx].area()){flag = 0;break;}
			else{
				res.erase(res.begin()+idx);
				//flag = 1;
			}
		}
		else{idx++;}
	}
	if(flag){res.push_back(temp);}
	sort(res.begin(),res.end(),SortByArea);

	// remove the too small rect !! bug found at 2015/9/15/1:00
	int axis = 0;
	double maxArea = (double)res[0].area();
	for(axis = 0;axis<res.size();axis++){
		double per = (double)res[axis].area()/(maxArea);
		if(per<MIN_PERRECT){break;}
	}
	if(axis!=res.size()){res.erase(res.begin()+axis,res.end());}


	if(res.size()>RESULT_NUM){res.erase(res.begin()+RESULT_NUM,res.end());}


	// remove the duplicate rect and control the num of rect under 4

	int proNum = res.size();

#if DEBUG_MODE
	remove("RectResult");
	mkdir("RectResult");
	for(int j = 0;j<proNum;j++){
		string savePath = "RectResult";
		char temp[10];
		itoa(j,temp,10);
		savePath = savePath+"/"+temp+".jpg";
		Mat OutTemp = img(res[j]);
		imwrite(savePath.c_str(),OutTemp);
	}
#endif

	//return proposalRect;
	return 1;
}
int Edge2Rect(Mat edge,Mat img,vector<Rect> &res){
	
	int width = edge.cols,height = edge.rows;

	vector<Rect> Clist;

	IplImage * Iedge = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	IplImage tmp = IplImage(edge);
	cvCopy(&tmp,Iedge);

	CvMemStorage *storage = 0;
	storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;
	cvFindContours(Iedge,storage,&contour,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

	for(;contour!=0;contour=contour->h_next){
		CvRect cRect;
		cRect = cvBoundingRect(contour,0);
		Rect rect;
		rect.x = cRect.x;rect.y = cRect.y;
		rect.width = cRect.width;rect.height = cRect.height;
		Clist.push_back(rect);
	}
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&Iedge);


	// another polar
	IplImage* convEdge = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	IplImage* com = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	cvSet(com,cvScalar(255));
	cvSub(com,&tmp,convEdge);

	CvMemStorage* convStorage = 0;
	convStorage = cvCreateMemStorage(0);
	CvSeq* convContour = 0;
	cvFindContours(convEdge,convStorage,&convContour,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);
	for(;convContour!=0;convContour=convContour->h_next){
		CvRect cRect;
		cRect = cvBoundingRect(convContour,0);
		Rect rect;
		rect.x = cRect.x;rect.y = cRect.y;
		rect.width = cRect.width;rect.height = cRect.height;
		Clist.push_back(rect);
	}
	cvReleaseMemStorage(&convStorage);
	cvReleaseImage(&convEdge);
	cvReleaseImage(&com);

	int len = Clist.size();
	sort(Clist.begin(),Clist.end(),SortByArea);

#if DEBUG_MODE
	remove("polarRect");
	mkdir("polarRect");
	for(int j = 0;j<Clist.size();j++){
		string savePath = "polarRect";
		char temp[10];
		itoa(j,temp,10);
		savePath = savePath+"/"+temp+".jpg";
		Mat OutTemp = img(Clist[j]);
		imwrite(savePath.c_str(),OutTemp);
	}
#endif



	vector<Rect> out;
	//int outNum = 10;
	for(int i = 0;i<min(SELECT_NUM,len);i++){
		Rect temp = Clist[i];
		out.push_back(temp);
	}
	//save the source rect
#if DEBUG_MODE
	remove("SourceRect");
	mkdir("SourceRect");
	for(int j = 0;j<out.size();j++){
		string savePath = "SourceRect";
		char temp[10];
		itoa(j,temp,10);
		savePath = savePath+"/"+temp+".jpg";
		Mat OutTemp = img(out[j]);
		imwrite(savePath.c_str(),OutTemp);
	}
#endif
	//combine the rect
	NewCombineRect(out,width,height,res);
	return 1;

}

int NewCombineRect(vector<Rect> input,int width,int height,vector<Rect> &out){
	
	vector<Rect> TempInput(input);
	
	sort(TempInput.begin(),TempInput.end(),SortByArea);
	// remove some max and min rect
	reMinMaxRect(TempInput,width,height);

	combineInterSectRect(TempInput);
	//remove the duplicate rect
	reDupRect(TempInput);

	vector<int > index;
	vector<vector<int > > classify;
	calcIncluRela(index,classify,TempInput);

	//sort the classify 
	for(int i = 0;i<classify.size();i++){sort(classify[i].begin(),classify[i].end());}
	//remove the small rect in one classify

	//for(int i = 0;i<classify.size();i++){
	//	vector<int> &tmp =classify[i];
	//	double innerMaxArea = TempInput[tmp[0]].area();

	//	for(int j = 0;j<tmp.size();j++){
	//		 
	//		double innerArea = TempInput[tmp[j]].area();
	//		if((innerArea/innerMaxArea)<INNER_THRESH){tmp.erase(tmp.begin()+j,tmp.end());break;}
	//		
	//	}
	//}

	/*remove the inner loop when multi cycle occur*/
	//for(int i = 0;i<classify.size();i++){
	//	vector<int> &tmp  = classify[i];
	//	int numMin = 0;
	//	
	//	if(tmp.size()<=2){continue;}
	//	double outArea = TempInput[tmp[0]].area();
	//	for(int j = 1;j<tmp.size();j++){
	//		double per = TempInput[tmp[j]].area()/outArea;
	//		if(per<CYCLE_LOOP_THRE){break;}
	//		outArea = TempInput[tmp[j]].area();
	//		numMin++;
	//	}
	//	if(numMin>=2){tmp.erase(tmp.begin()+1,tmp.begin()+numMin);}
	//}

	// save the include rect to out
	for(int i = 0;i<classify.size();i++){
		for(int j = 0;j<classify[i].size();j++){
			out.push_back(TempInput[classify[i][j]]);
		}
	}
	sort(out.begin(),out.end(),SortByArea);

	if(index.size()==0){
		return 1;
	}
	//calc the rest 
	vector<Rect> restRect;
	for(int i = 0;i<index.size();i++){restRect.push_back(TempInput[index[i]]);}
	sort(restRect.begin(),restRect.end(),SortByArea);

	//combine the rest rect
	combineRestRect(restRect,out,width,height);
	
	return 1;
}

Rect CombineTwoRect(Rect a,Rect b){
	Rect out;
	int lux = min(a.x,b.x);
	int luy = min(a.y,b.y);
	int rdx = max(a.x+a.width-1,b.x+b.width-1);
	int rdy = max(a.y+a.height-1,b.y+b.height-1);

	out.x = lux,out.y = luy;
	out.width = rdx-lux+1,out.height = rdy-luy+1;
	return out;
}
int distMatrix(vector<Rect> input,int width,int height,Mat &Dist){

	int num = input.size();
	
	Dist = Dist*DIST_CONSTANT;
	// add your code here to combine the Rect
	for(int i = 0;i<num-1;i++){
		for(int j = i+1;j<num;j++){
			//double tempDist = distRect(input[i],input[j],width,height);
			double tempDist = NewDistRect(input[i],input[j]);
			Dist.at<double>(i,j) = tempDist;
		}
	}
	Mat tDist;
	transpose(Dist,tDist);
	Dist = Dist+tDist;
	return 1;
}
bool IsContainer(Rect a,Rect b){
	// a in b
	if(a.x>=b.x&&a.y>=b.y&&(a.y+a.height)<=(b.y+b.height)&&(a.x+a.width)<=(b.x+b.width)){
		return true;
	}
	// b in a 
	if(b.x>=a.x&&b.y>=a.y&&(b.x+b.width)<=(a.x+a.width)&&(b.y+b.height)<=(a.y+a.height)){
		return true;
	}
	return false;

}
bool IsInterSect(Rect a,Rect b){

	if(a.x>b.x+b.width){return false;}
	if(a.y>b.y+b.height){return false;}
	if(b.x>a.x+a.width){return false;}
	if(b.y>a.y+a.height){return false;}

	return true;
}
void saveMat(Mat input,string saveName){
	string saveFold = "image\\";
	string savePath = saveFold+saveName+".xml";
	FileStorage out(savePath,FileStorage::WRITE);
	out<<"out"<<input;
	out.release();

}
double InterSectPercentMin(Rect a,Rect b){
	double widthIn = min(a.x+a.width,b.x+b.width)-max(a.x,b.x);
	double heightIn = min(a.y+a.height,b.y+b.height)-max(a.y,b.y);
	double areaIn = widthIn*heightIn;
	//double interPercent = areaIn/(a.area()+b.area()-areaIn);
	double interPercent = areaIn/(min(a.area(),b.area()));
	return interPercent;
}
double InterSectPercent(Rect a,Rect b){
	double widthIn = min(a.x+a.width,b.x+b.width)-max(a.x,b.x);
	double heightIn = min(a.y+a.height,b.y+b.height)-max(a.y,b.y);
	double areaIn = widthIn*heightIn;
	//double interPercent = areaIn/(a.area()+b.area()-areaIn);
	double interPercent = areaIn/(a.area()+b.area()-areaIn);
	return interPercent;
}
double NewDistRect(Rect a,Rect b){

	//directly use the dist of the centre because of the operation before 
	double xa,ya;int xb,yb;
	xa = a.x+1/2*a.width;
	ya = a.y+1/2*a.height;
	xb = b.x+1/2*b.width;
	yb = b.y+1/2*b.height;
	double Distance = sqrt(pow(xb-xa,2.0)+pow(yb-ya,2.0));
	return Distance;

}
int reDupRect(vector<Rect> &input){
	int num = input.size();
	vector<int> index;
	for(int i = 0;i<num-1;i++){
	
		for(int j = i+1;j<num;j++){
			if(IsInterSect(input[i],input[j])&&InterSectPercent(input[i],input[j])>DUPLICATE_THRE){
				if(input[i].area()>input[j].area()){index.push_back(j);}
				else{index.push_back(i);}
			}
		}
	}
	sort(index.begin(),index.end());
	index.erase(unique(index.begin(),index.end()),index.end());
	for(int i = index.size()-1;i>=0;i--){input.erase(input.begin()+index[i]);}
	return 1;
}
int reMinMaxRect(vector<Rect> &TempInput,int width,int height){

	// delete the rect being almost the whole image
	int num = TempInput.size();
	int axis = 0;
	for(axis = 0;axis<num;axis++){
		double per = (double)TempInput[axis].area()/(width*height);
		if(per<MAX_PERRECT){break;}
	}
	if(axis!=0){
		TempInput.erase(TempInput.begin(),TempInput.begin()+axis);
	}
	num = TempInput.size();
	if(num==0){
		return 1;
	}

	// delete the very small rect
	double maxArea = (double)TempInput[0].area();
	for(axis = 0;axis<TempInput.size();axis++){
		double per = (double)TempInput[axis].area()/(maxArea);
		if(per<MIN_PERRECT){break;}
	}
	if(axis!=TempInput.size()){TempInput.erase(TempInput.begin()+axis,TempInput.end());}
	return 1;
}
int combineRestRect(vector<Rect> &restRect,vector<Rect> &out,int width,int height){
	int restNum = restRect.size();

	if(restNum==2){
		if(IsInterSect(restRect[0],restRect[1])){
			double tp = InterSectPercentMin(restRect[0],restRect[1]);
			if(tp>INTERSECT_THRE){
				restRect[1] = CombineTwoRect(restRect[0],restRect[1]);
				restRect.erase(restRect.begin());
				restNum = 1;
			}
		}
		for(int i = 0;i<restRect.size();i++){
			out.push_back(restRect[i]);
		}
		return 1;
	}

	if(restNum==1){
		for(int i = 0;i<restRect.size();i++){
			out.push_back(restRect[i]);
		}
		return 1;
	}
	Mat dist = Mat::eye(restNum,restNum,CV_64F);

	distMatrix(restRect,width,height,dist);


	do{
		int idx = restNum-1;
		Mat temp = dist.row(idx);
		Point minLoc;
		minMaxLoc(temp,0,0,&minLoc,0);
		int tidx = minLoc.x;
		restRect[idx] = CombineTwoRect(restRect[idx],restRect[tidx]);
		restRect.erase(restRect.begin()+tidx);

		//sort the rect
		sort(restRect.begin(),restRect.end(),SortByArea);


		restNum = restRect.size();
		//calc the dist

		dist = Mat::eye(restNum,restNum,CV_64F);
		distMatrix(restRect,width,height,dist);

		// if leave two rect and the two rect intersect over 0.3 then combine them 
		if(restNum==2){
			if(IsInterSect(restRect[0],restRect[1])){
				double tp = InterSectPercentMin(restRect[0],restRect[1]);
				if(tp>INTERSECT_THRE){
					restRect[1] = CombineTwoRect(restRect[0],restRect[1]);
					restRect.erase(restRect.begin());
					restNum = 1;
				}
			}
		}

	}while(restNum>PROPOSAL_NUM);

	for(int i = 0;i<restRect.size();i++){
		out.push_back(restRect[i]);
	}
}
int calcIncluRela(vector<int > &index,vector<vector<int > > &classify,vector<Rect> &TempInput){

	int num = TempInput.size();

	// create the include relation
	for(int i = 0;i<num;i++){index.push_back(i);}

	int idx = 0;
	while(idx<index.size()){
		vector<int> tempIdx;
		tempIdx.push_back(idx);
		for(int j = idx+1;j<index.size();j++){
			bool tempBin = IsContainer(TempInput[index[idx]],TempInput[index[j]]);
			if(tempBin){tempIdx.push_back(j);}
		}
		if(tempIdx.size()>1){
			vector<int > temp;
			for(int i = 0;i<tempIdx.size();i++){
				temp.push_back(index[tempIdx[i]]);
			}
			classify.push_back(temp);
			for(int i = temp.size()-1;i>=0;i--){index.erase(index.begin()+tempIdx[i]);}
		}else{idx++;}
	}
	return 1;
}
int combineInterSectRect(vector<Rect> &TempInput){
	int num = TempInput.size();
	int idx = 0;
	while(idx<(int)TempInput.size()-1){
		Rect st = TempInput[idx];
		bool flag = 1;
		for(int j = idx+1;j<TempInput.size();j++){
			Rect en = TempInput[j];
			if(IsInterSect(st,en)&&(!IsContainer(st,en))){
				double areaPer = InterSectPercentMin(st,en);
				if(areaPer>INTERSECT_THRE){
					TempInput[j] = CombineTwoRect(st,en);
					TempInput.erase(TempInput.begin()+idx);
					flag = 0;
					break;
				}
			}
		}
		if(flag){idx++;}
	}
	return 1;
}