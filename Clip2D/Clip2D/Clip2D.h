
// Clip2D.h : main header file for the Clip2D application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CClip2DApp:
// See Clip2D.cpp for the implementation of this class
//

class CClip2DApp : public CWinApp
{
public:
	CClip2DApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CClip2DApp theApp;
