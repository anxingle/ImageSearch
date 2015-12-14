#include <opencv2/opencv.hpp>

#include "API_Interface.h"
#include "afxwin.h"
// ProPosalDlg.h : 头文件
//

#pragma once


// CProPosalDlg 对话框
class CProPosalDlg : public CDialogEx
{
// 构造
public:
	CProPosalDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PROPOSAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void Openpicture_Button();
	void ShowImage(IplImage* img, unsigned int ID, unsigned int Edit_ID, CString str);

	
	float ratio;
	CString mPath;
	cv::Mat sourceImg;
	int Pic_Size;
	int Pic_Size_2;
	bool pic_open;//是否有打开的图片
	bool pic_search;//是否按下搜索
	afx_msg void Extractproposal_Button();
	afx_msg void Prepage();
	afx_msg void Nextpage();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CComboBox m_comboType;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
