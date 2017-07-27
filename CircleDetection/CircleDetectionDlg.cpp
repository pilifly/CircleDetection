
// CircleDetectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CircleDetection.h"
#include "CircleDetectionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCircleDetectionDlg dialog



CCircleDetectionDlg::CCircleDetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CIRCLEDETECTION_DIALOG, pParent)
	, m_minDistance(50)
	, m_minRad(2)
	, m_maxRad(4)
	, m_minCutoff(8)
	, m_maxCutoff(16)
	, m_contrThresh(100.0)
	, m_contFilter(32)
	, m_contRad(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCircleDetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MINDIST, m_minDistance);
	DDX_Text(pDX, IDC_MINRAD, m_minRad);
	DDX_Text(pDX, IDC_MAXRAD, m_maxRad);
	DDX_Text(pDX, IDC_CUTOFF1, m_minCutoff);
	DDX_Text(pDX, IDC_CUTOFF2, m_maxCutoff);
	DDX_Text(pDX, IDC_CONTTHRESH, m_contrThresh);
	DDX_Text(pDX, IDC_CONTFILTER, m_contFilter);
	DDX_Text(pDX, IDC_CONTRAD, m_contRad);
}

BEGIN_MESSAGE_MAP(CCircleDetectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPENFILE, &CCircleDetectionDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(ID_APPLY, &CCircleDetectionDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_APPLY2, &CCircleDetectionDlg::OnBnClickedLaplacian)
	ON_BN_CLICKED(ID_CONTOUR, &CCircleDetectionDlg::OnBnClickedContour)
END_MESSAGE_MAP()


// CCircleDetectionDlg message handlers

BOOL CCircleDetectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCircleDetectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCircleDetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCircleDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCircleDetectionDlg::OnBnClickedOpenfile()
{
	m_CV.ReadFileAndDetect();
}


void CCircleDetectionDlg::OnBnClickedApply()
{
	UpdateData(true);
	m_CV.UpdateCirclesDetection(m_minDistance, m_minRad, m_maxRad, m_minCutoff, m_maxCutoff);
	//m_CV.DetectCircles();
}


void CCircleDetectionDlg::OnBnClickedLaplacian()
{
	m_CV.ApplyLaplacian();
}


void CCircleDetectionDlg::OnBnClickedContour()
{
	UpdateData(true);
	m_CV.DetectContours(m_contrThresh, m_contFilter, m_contRad);
}
