
// CircleDetectionDlg.h : header file
//

#pragma once
#include "OpenCVDemo.h"

// CCircleDetectionDlg dialog
class CCircleDetectionDlg : public CDialogEx
{
// Construction
public:
	CCircleDetectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLEDETECTION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	COpenCVDemo m_CV;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedLaplacian();
	afx_msg void OnBnClickedContour();
	int m_minDistance;
	int m_minRad;
	int m_maxRad;
	int m_minCutoff;
	int m_maxCutoff;
	double m_contrThresh;
	int m_contFilter;
	double m_contRad;
};
