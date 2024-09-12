
// StraightLine.h : main header file for the StraightLine application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CStraightLineApp:
// See StraightLine.cpp for the implementation of this class
//

class CStraightLineApp : public CWinApp
{
public:
	CStraightLineApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CStraightLineApp theApp;
