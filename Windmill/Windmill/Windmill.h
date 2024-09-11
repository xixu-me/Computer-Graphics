
// Windmill.h : main header file for the Windmill application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWindmillApp:
// See Windmill.cpp for the implementation of this class
//

class CWindmillApp : public CWinApp
{
public:
	CWindmillApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWindmillApp theApp;
