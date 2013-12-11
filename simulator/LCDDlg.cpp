// LCDDlg.cpp : implementation file

#include "stdafx.h"
#include "LCD.h"
#include "LCDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//void drawTest(int type);
void lcdTest();

unsigned char  DDRAM[X_MAX*Y_MAX];
/////////////////////////////////////////////////////////////////////////////
// CLCDDlg dialog

CLCDDlg::CLCDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLCDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLCDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CLCDDlg::~CLCDDlg()
{
	if(m_hmemBmp)
		::DeleteObject(m_hmemBmp);
}


void CLCDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLCDDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLCDDlg, CDialog)
	//{{AFX_MSG_MAP(CLCDDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLCDDlg message handlers

BOOL CLCDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//--------------------------------------------------------------------//
	CWnd::SetWindowPos(NULL,0,0,X_MAX*(DOT_M+1)+16,Y_MAX*(DOT_M+1)+16,SWP_NOMOVE);

	CRect    rectd;
	GetClientRect(&rectd);
	SetWindowPos( NULL,
		(GetSystemMetrics(SM_CXFULLSCREEN) - rectd.Width())/2,
		GetSystemMetrics(SM_CYFULLSCREEN) - rectd.Height(),
		0, 0, SWP_NOSIZE);
	//SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);


	UpdateLayered();

	int  i,j;

	for(i=0;i<Y_MAX;i++)
	{
		for(j=0;j<X_MAX;j++)
		{
			//if(j%4==0)
			DDRAM[i*X_MAX+j] = 0xff;

		}
	}
	CloseHandle(CreateThread( NULL, 0, cpuThread, NULL, 0, &aaThreadID));

    //drawTest(7); //128x64 bmp on screen  
	
	SetTimer(1,8000,NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLCDDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLCDDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



#define   AC_SRC_ALPHA      0x01 
void CLCDDlg::UpdateLayered()
{
	if(m_hWnd == NULL || !::IsWindow(m_hWnd))
		return;

	if(!UpdateLayeredWindow)
		return;

	CClientDC dc(this);
	CDC dcMemory;
/*********************************************************/
	if(dcMemory.CreateCompatibleDC(&dc))
	{
/*********************************************************/
		rc = m_rect;
		//ShowSkinProgress(VolumVal);
		//wDisplayBox(RGB(10,250,10),0,0,rc.Width(),rc.Height());
		//wDisplayBox(RGB(10,250,10),6,6,rc.Width()-12,rc.Height()-12);

/*********************************************************/
		if(m_hmemBmp)
		{
			dcMemory.SelectObject(m_hmemBmp);

			CRect rcWnd;
			GetWindowRect(&rcWnd);
			POINT ptWinPos={rcWnd.left,rcWnd.top};
			SIZE sizeWindow={rcWnd.Width(),rcWnd.Height()};
			POINT ptSrc={0,0};
			
			BLENDFUNCTION Blend={AC_SRC_OVER,0,255,AC_SRC_ALPHA};

			HDC hdcScreen=::GetDC(NULL);
			UpdateLayeredWindow(m_hWnd,hdcScreen,&ptWinPos,&sizeWindow,dcMemory,&ptSrc,0,&Blend,2);
			VERIFY(::ReleaseDC(NULL,hdcScreen)==1);
		}
/*********************************************************/
	}
/*********************************************************/
}

/*===================================================================================================*/
/*===================================================================================================*/

int CLCDDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CDialog::OnCreate(lpCreateStruct)==-1)
		return -1;
	HMODULE hMoudle=::GetModuleHandle(_T("user32.dll"));
	if(hMoudle)
	{		
#ifndef WS_EX_LAYERED
		#define WS_EX_LAYERED           0x00080000
#endif
		SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
		UpdateLayeredWindow=(lpfnUpdateLayeredWindow)GetProcAddress(hMoudle,_T("UpdateLayeredWindow"));
	}
	return 0;
}

/*===================================================================================================*/
/*===================================================================================================*/

void CLCDDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(cx!=0 && cy!=0)
	{
		m_rect.SetRect(0,0,cx,cy);
		if(m_hmemBmp)
			::DeleteObject(m_hmemBmp);
		BITMAPINFO  bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth=cx;
		bmi.bmiHeader.biHeight=cy;
		bmi.bmiHeader.biPlanes=1;
		bmi.bmiHeader.biBitCount=32;
		bmi.bmiHeader.biCompression=BI_RGB;
		
		m_hmemBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&m_pBits, 0, 0);
		if(m_hmemBmp && m_pBits)
		{
			m_dwSize=bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 4;
			m_dwEffWidth = ((((32 * bmi.bmiHeader.biWidth) + 31) / 32) * 4);
		}
		//UpdateLayered();
	}
	CDialog::OnSize(nType,cx,cy);	
}

/*===================================================================================================*/
/*===================================================================================================*/

DWORD WINAPI cpuThread(LPVOID lpParam)
{
	CLCDDlg * pWnd = (CLCDDlg *)AfxGetMainWnd();

	do
	{
		pWnd->wShowDDRAM();
		pWnd->UpdateLayered();
		::Sleep(50);// ÐÝÃß
	}while(1);

	return 0;
}

/*===================================================================================================*/
/*===================================================================================================*/

void CLCDDlg::wDisplayBox(COLORREF color,long xs,long ys,long xww,long yhh)
{
	long xx ,yy;
	BYTE* dst;
	long xHight,xWidth;

	xHight = rc.Height();
	xWidth = rc.Width();
	//Ìî³äÊý¾Ý

	for(yy=0;yy<yhh;yy++)
	{
		dst=m_pBits + (xHight-yy-ys-1)*m_dwEffWidth + xs*4;

		for(xx=0;xx<xww;xx++)
		{
			*dst++=(BYTE)(GetBValue(color));
			*dst++=(BYTE)(GetGValue(color));
			*dst++=(BYTE)(GetRValue(color));
			*dst++=255;
		}
	}
}

void CLCDDlg::wDisplayDot(long xs,long ys)
{
	wDisplayBox(RGB(10,10,10),8+xs*(DOT_M+1),8+ys*(DOT_M+1),DOT_M,DOT_M);

}
/*===================================================================================================*/
/*===================================================================================================*/

void CLCDDlg::wShowDDRAM(void)
{
	int  i,j;

	wDisplayBox(RGB(15,175,75),0,0,rc.Width(),rc.Height());
	wDisplayBox(RGB(10,250,10),7,7,rc.Width()-16,rc.Height()-16);

	for(i=0;i<Y_MAX;i++)
	{
		for(j=0;j<X_MAX;j++)
		{
			if(  0xff == DDRAM[i*X_MAX+j]  )
				wDisplayDot(j,i);
		}
	}
}



BOOL CLCDDlg::PreTranslateMessage(MSG* pMsg) 
{
	if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE))
	{
		return 1;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CLCDDlg::OnTimer(UINT nIDEvent) 
{  
        lcdTest();

	/*
        drawTest(0); //simple text    
        drawTest(1); //simple drawing

        drawTest(6); //simple progress bar

        drawTest(4); //menus with invert

        drawTest(7); //128x64 bmp on screen   
        drawTest(8); //put a bmp on screen (atmel picture is 108x35)    
        drawTest(9);
        */
  
	CDialog::OnTimer(nIDEvent);
}
