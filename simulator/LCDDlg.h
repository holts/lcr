// LCDDlg.h : header file
//

#if !defined(AFX_LCDDLG_H__2839E6E4_64CA_4E9B_89B1_8C1DC2BCC743__INCLUDED_)
#define AFX_LCDDLG_H__2839E6E4_64CA_4E9B_89B1_8C1DC2BCC743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define     X_MAX       220
#define     Y_MAX       176

#define     DOT_M       2

/////////////////////////////////////////////////////////////////////////////
// CLCDDlg dialog
DWORD WINAPI cpuThread(LPVOID lpParam);


class CLCDDlg : public CDialog
{
// Construction
public:
	CLCDDlg(CWnd* pParent = NULL);	// standard constructor
	~CLCDDlg();
// Dialog Data
	//{{AFX_DATA(CLCDDlg)
	enum { IDD = IDD_LCD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLCDDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void  wDisplayDot(long xs,long ys);
	void  wShowDDRAM(void);
	void  UpdateLayered();
	void  wDisplayBox(COLORREF clr1,long xs,long ys,long xww,long yhh);



protected:
	
	typedef BOOL (WINAPI* lpfnUpdateLayeredWindow)(HWND hwnd,HDC hdcDst,POINT *pptDst,SIZE *psize,HDC hdcSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,DWORD dwFlags);
	lpfnUpdateLayeredWindow UpdateLayeredWindow;

	HBITMAP m_hmemBmp;
	BYTE*   m_pBits;
	DWORD   m_dwSize;
	DWORD   m_dwEffWidth;
	CRect   m_rect;
	CRect   rc;


	DWORD  aaThreadID;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLCDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCDDLG_H__2839E6E4_64CA_4E9B_89B1_8C1DC2BCC743__INCLUDED_)
