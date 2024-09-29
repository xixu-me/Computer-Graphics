
// NationalFlag.h : main header file for the NationalFlag application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CNationalFlagApp:
// See NationalFlag.cpp for the implementation of this class
//

class CNationalFlagApp : public CWinApp
{
public:
	CNationalFlagApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNationalFlagApp theApp;
