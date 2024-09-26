
// ScanlineRendering.h : main header file for the ScanlineRendering application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h" // main symbols

// CScanlineRenderingApp:
// See ScanlineRendering.cpp for the implementation of this class
//

class CScanlineRenderingApp : public CWinApp {
public:
	CScanlineRenderingApp() noexcept;

	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CScanlineRenderingApp theApp;
