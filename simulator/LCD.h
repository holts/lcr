// LCD.h : main header file for the LCD application
//

#if !defined(AFX_LCD_H__26B636B0_8C09_4E90_9741_56A1F7794B6D__INCLUDED_)
#define AFX_LCD_H__26B636B0_8C09_4E90_9741_56A1F7794B6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLCDApp:
// See LCD.cpp for the implementation of this class
//

class CLCDApp : public CWinApp
{
public:
	CLCDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLCDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLCDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCD_H__26B636B0_8C09_4E90_9741_56A1F7794B6D__INCLUDED_)
