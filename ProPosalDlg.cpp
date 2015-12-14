
// ProPosalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProPosal.h"
#include "ProPosalDlg.h"
#include "afxdialogex.h"
#include "Preprocess.h"
#include "CvvImage.h"
#include <string>
#include "afx.h"
#include "stdio.h"
#include "WzdSplash.h"
#include "Test.h"
//#pragma comment(lib,"opencv_highgui248d.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProPosalDlg 对话框



CProPosalDlg::CProPosalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProPosalDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProPosalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboType);
}

BEGIN_MESSAGE_MAP(CProPosalDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenPicture, &CProPosalDlg::Openpicture_Button)
	ON_BN_CLICKED(IDC_ExtractProposal, &CProPosalDlg::Extractproposal_Button)
	ON_BN_CLICKED(IDC_PrePage, &CProPosalDlg::Prepage)
	ON_BN_CLICKED(IDC_NextPage, &CProPosalDlg::Nextpage)
	ON_BN_CLICKED(IDC_BUTTON2, &CProPosalDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CProPosalDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CProPosalDlg::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CProPosalDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON5, &CProPosalDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CProPosalDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CProPosalDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CProPosalDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CProPosalDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CProPosalDlg::OnBnClickedButton10)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProPosalDlg 消息处理程序

BOOL CProPosalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	ratio = 1;// picture 控件的缩放比例设置为 1 
	Pic_Size = 300;
	Pic_Size_2 = 142;
	//组合下拉框初始化
	m_comboType.AddString(_T("DENSESURF"));
	m_comboType.AddString(_T("DENSESURF_PRO_VLAD"));
	//启动画面  2015.12.07
	CWzdSplash wndSplash;                 //创建启动窗口类的实例  
	wndSplash.Create(IDB_BITMAP1);
	wndSplash.CenterWindow();
	wndSplash.UpdateWindow(); // send WM_PAINT

	//记载特征文件
	string labelFile = "F:\\Data\\train_ground_truth.txt";
	string centerFile = "../DenSurfProVlad/center.txt";
	string featureBinFile = "F:\\Data\\featureAfterPCA.bin";
	string imageNameFile = "F:\\Data\\imgName.txt";
	string pcaMatrixFile = "F:\\Data\\pcaMatrix.bin";
	string rotateMatrixFile = "F:\\Data\\rotateMatrix.bin";
	GlobalInit(labelFile,centerFile,pcaMatrixFile,imageNameFile,featureBinFile,rotateMatrixFile);
	/*   
	int GlobalInit(string labelFile,
	string centerFile,
	string pcaMatrixFile,
	string imageNameFile,
	string featureBinFile,
	string rotateMatrixFile 
	)

	*/
	//销毁启动画面
	Sleep(100);
	pic_open = FALSE;  //图片未打开
	pic_search = FALSE;//未搜索
	SetTimer(1,1800,NULL);
	feaflags = DENSESURF;
	wndSplash.DestroyWindow();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProPosalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProPosalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

		//if(pic_open)
		//{
		// cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//将原图长边压缩至图片控件边长Pic_Size

		//    ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("原图片"));        // 调用显示图片函数
		// AfxMessageBox("ll");
		//}
	}
	else
	{
		/*   图片打开，立即不断重绘
		if(pic_open)
		{
		cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//将原图长边压缩至图片控件边长Pic_Size

		ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("原图片"));        // 调用显示图片函数
		//AfxMessageBox("ll");
		cv::Mat ShowImgs2;
		for(int i = 0;i < 9;i++)
		{  
		ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
		// double2 CString
		char buffer[39];
		sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
		//ImgName.push_back(name);
		string str =buffer;

		ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
		}
		}   */
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CProPosalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProPosalDlg::Openpicture_Button()
{
	CFileDialog dlg(
		TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("All Files (*.*) |*.*||"), NULL
		);
	dlg.m_ofn.lpstrTitle = _T("请选择一幅待检索的图像");// 打开文件对话框的标题名

	if (dlg.DoModal() != IDOK)             // 判断是否获得图片
	{
		AfxMessageBox(_T("请选择合适的文件路径"));
		return;
	}

	mPath = dlg.GetPathName();	    // 获取图片路径
	// 路径转化为 char*
	/*
	const size_t strsize = (mPath.GetLength() + 1) * 2;
	char *mPath_str = new char[strsize];
	size_t sz = 0;
	wcstombs_s(&sz, mPath_str, strsize, mPath, _TRUNCATE);
	*/

	sourceImg = cv::imread((LPCSTR)mPath);
	//sourceImg = cv::imread("K:\\anxingle\\anxingle.jpg");
	//sourceImg = cv::Mat(cvLoadImage((LPCSTR)mPath));

	//sourceImg = cv::imread(mPath_str.GetBuffer());
	//mPath(mPath_str)即为这种路径：("E://Programs/Images/Lena.jpg" )  

	if (sourceImg.empty())      // 判断是否成功读取图片,若TheImage==NULL则返回，不执行下面的操作
	{
		AfxMessageBox(_T(mPath));
		AfxMessageBox(_T("读取图片失败"));
		return;
	}

	cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//将原图长边压缩至图片控件边长Pic_Size

	ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("原图片"));        // 调用显示图片函数
	pic_open = TRUE;

}


void CProPosalDlg::ShowImage(IplImage* img,unsigned int ID,unsigned int Edit_ID,CString str)
{
	if (!img)      // 判断是否成功读取图片,若TheImage==NULL则返回，不执行下面的操作
	{
		AfxMessageBox(_T("读取图片失败"));
		return;
	}
	CDC* pDC = GetDlgItem(ID)->GetDC();   // 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();			 // 获取 HDC(设备句柄) 来进行绘图操作	

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int ID_size = rect.bottom>rect.right ? rect.right : rect.bottom;//检查Picture控件的尺寸
	//int b=rect.right;
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));//加一个对话框颜色填充，不然多张图片会有地方一直出现
	int iw = img->width;
	int ih = img->height;
	if (iw >= ih)	
		ratio = ((float)iw) / ID_size;
	else			
		ratio = ((float)ih) / ID_size;
	SetRect(rect, 0, 0, floor((float)iw / ratio), floor((float)ih / ratio));//改变rect大小		


	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);	// 将图片绘制到显示控件的指定区域内
	if(feaflags == DENSESURF_PRO_VLAD)
	{
		if ( Edit_ID == IDC_EDIT1 || Edit_ID == IDC_EDIT4 || Edit_ID == IDC_EDIT7)
			GetDlgItem(Edit_ID)->SetWindowText(str);
		else
		{
			if(Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
				GetDlgItem(Edit_ID)->SetWindowTextA("              ");
		}
	}
	else
	{
		if(Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
			GetDlgItem(Edit_ID)->SetWindowText(str);
	}
	ReleaseDC(pDC);

}



// 检索图片
void CProPosalDlg::Extractproposal_Button()
{

	pageIdx = 1;
	/*
	testWithEuler(string type,
	cv::Mat imgSrc,
	vector<pair<float*, string> >& trainFeatures,
	unordered_map<string, unordered_set<string> >& categoryLabels)
	*/
	neighbors.clear();
	if(!pic_open)
	{
		AfxMessageBox("请打开图片！");
		return ;
	}
	//vector<string>(neighbors).swap(neighbors);
	testWithEuler("abc ",sourceImg,trainFeatures,categoryLabels);
	//AfxMessageBox("debug1");
	//Retrieval(sourceImg);
	//vector<cv::Mat>ShowImgs2;
	cv::Mat ShowImgs2;
	if(!neighbors.size())
	{
		AfxMessageBox("显示图片不够！");
	}
	else
	{  	
		if(feaflags == DENSESURF_PRO_VLAD)
		{  
			/*    debug for the ShowImgs2[i];
			for(int i = 0;i < 9;)
			{ 

			ShowImgs2[i] = cv::imread(trainPath+"\\"+neighbors[i]);
			ShowImgs2[i+1] = cv::imread(trainPath+"\\"+neighbors[i+1]);
			ShowImgs2[i+2] = cv::imread(trainPath+"\\"+neighbors[i+2]);

			//neighbors[i]
			// double2 CString
			char buffer[39];
			char buffer2[39];
			char buffer3[39];
			sprintf_s(buffer,"%s",neighbors[i]);
			sprintf_s(buffer2,"%s",neighbors[i+1]);
			sprintf_s(buffer3,"%s",neighbors[i+2]);

			string str =buffer;
			string str2 = buffer2;
			string str3 = buffer3;
			ShowImage(&IplImage(ShowImgs2[i]),IDC_T1+i,IDC_EDIT1+i,str.c_str());  //str.Format（＂%f＂,i）; 

			ShowImage(&IplImage(ShowImgs2[i+1]),IDC_T1+i+1,IDC_EDIT1+i+1,str2.c_str());
			ShowImage(&IplImage(ShowImgs2[i+2]),IDC_T1+i+2,IDC_EDIT1+i+2,str3.c_str());
			i = i+3;
			*/
			//}
		}
		else
		{
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i]);

				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i].c_str());

				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
				pic_search = TRUE;

			}
		}
	}
}


//上一页
void CProPosalDlg::Prepage()
{
	//AfxMessageBox(_T("这个是上一页的按钮"));
	cv::Mat ShowImgs2;
	if(!neighbors.size())
	{
		AfxMessageBox("显示图片不够！");
		return;
	}
	if(pageIdx == 1)
	{
		AfxMessageBox(_T("已经是第一页了"));
		return;
	}
	else
	{
		pageIdx--;
		//ShowToMFC();
		if(feaflags == DENSESURF_PRO_VLAD)
		{/*
		 for(int i = 0;i < 9;)
		 {  
		 // double2 CString
		 char buffer[39];
		 if(i == 0)
		 sprintf_s(buffer,"%f %s",SimRe[0],ImgName[0].c_str());
		 else 
		 {
		 if(i == 3)
		 sprintf_s(buffer,"%f %s",SimRe[1],ImgName[1].c_str());
		 else
		 sprintf_s(buffer,"%f %s",SimRe[2],ImgName[2].c_str());
		 }
		 //ImgName.push_back(name);
		 string str =buffer;
		 ShowImage(&IplImage(ShowImgs[i]),IDC_T1+i,IDC_EDIT1+i,str.c_str());  //str.Format（＂%f＂,i）; 

		 ShowImage(&IplImage(ShowImgs[i+1]),IDC_T1+i+1,IDC_EDIT1+i+1,str.c_str());
		 ShowImage(&IplImage(ShowImgs[i+2]),IDC_T1+i+2,IDC_EDIT1+i+2,str.c_str());
		 i = i+3;
		 } */
		}
		else
		{
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
			}
		}
	}
}



//下一页
void CProPosalDlg::Nextpage()
{
	//AfxMessageBox(_T("这个是下一页的按钮"));

	if(pageIdx <= 10)
		pageIdx++;
	else 
	pageIdx = 11;
	if(pageIdx == 11)
	{
		AfxMessageBox("图片显示结束！");
		//pageIdx = 11;
		return ;
	}
	cv::Mat ShowImgs2;
	if(neighbors.size() == 0 ||!neighbors.size())
	{
		AfxMessageBox("显示图片不够！");
		return;
	}

	if(pageIdx <= 10)
	{

		/*
		if(feaflags == DENSESURF_PRO_VLAD)
		{  
			for(int i = 0;i < 9;)
			{  
			// double2 CString
			char buffer[39];
			if(i == 0)
			sprintf_s(buffer,"%f %s",SimRe[0],ImgName[0].c_str());
			else 
			{
			if(i == 3)
			sprintf_s(buffer,"%f %s",SimRe[1],ImgName[1].c_str());
			else
			sprintf_s(buffer,"%f %s",SimRe[2],ImgName[2].c_str());
			}
			//ImgName.push_back(name);
			string str =buffer;
			ShowImage(&IplImage(ShowImgs[i]),IDC_T1+i,IDC_EDIT1+i,str.c_str());  //str.Format（＂%f＂,i）; 

			ShowImage(&IplImage(ShowImgs[i+1]),IDC_T1+i+1,IDC_EDIT1+i+1,str.c_str());
			ShowImage(&IplImage(ShowImgs[i+2]),IDC_T1+i+2,IDC_EDIT1+i+2,str.c_str());
			i = i+3;
			}  
		} */

			
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
			}
		
	}
}

//第一个放大按钮
void CProPosalDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowImage(&IplImage(ShowImgs[0]),IDC_MAIN2,IDC_EDIT1+10,_T("第一个放大按钮"));
}

//第四个放大按钮
void CProPosalDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowImage(&IplImage(ShowImgs[3]),IDC_MAIN2,IDC_EDIT1+10,_T("第4个放大按钮"));
}


//第七个放大按钮
void CProPosalDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowImage(&IplImage(ShowImgs[6]),IDC_MAIN2,IDC_EDIT1+10,_T("第7个放大按钮"));
}


void CProPosalDlg::OnCbnSelchangeCombo1()
{
	pageIdx = 1;
	int nSel;
	nSel = m_comboType.GetCurSel();
	if(nSel == 0)
		feaflags = DENSESURF;
	//AfxMessageBox("第一个");
	else
		feaflags = DENSESURF_PRO_VLAD;
}

//第二个
void CProPosalDlg::OnBnClickedButton5()
{
	ShowImage(&IplImage(ShowImgs[1]),IDC_MAIN2,IDC_EDIT1+10,_T("第2个放大按钮"));
}
//第五个
void CProPosalDlg::OnBnClickedButton6()
{
	ShowImage(&IplImage(ShowImgs[4]),IDC_MAIN2,IDC_EDIT1+10,_T("第5 个放大按钮"));
}

//第八个
void CProPosalDlg::OnBnClickedButton7()
{
	ShowImage(&IplImage(ShowImgs[7]),IDC_MAIN2,IDC_EDIT1+10,_T("第8个放大按钮"));
}

//第三个
void CProPosalDlg::OnBnClickedButton8()
{
	ShowImage(&IplImage(ShowImgs[2]),IDC_MAIN2,IDC_EDIT1+10,_T("第三个放大按钮"));
}

//六
void CProPosalDlg::OnBnClickedButton9()
{
	ShowImage(&IplImage(ShowImgs[5]),IDC_MAIN2,IDC_EDIT1+10,_T("第6个放大按钮"));
}

//九
void CProPosalDlg::OnBnClickedButton10()
{
	ShowImage(&IplImage(ShowImgs[8]),IDC_MAIN2,IDC_EDIT1+10,_T("第9个放大按钮"));
}


void CProPosalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(pic_open)
	{
		cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//将原图长边压缩至图片控件边长Pic_Size

		ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("原图片"));        // 调用显示图片函数
		//AfxMessageBox("ll");
		if(pic_search){
			cv::Mat ShowImgs2;
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
			}
		}
	}  
	CDialogEx::OnTimer(nIDEvent);
}
