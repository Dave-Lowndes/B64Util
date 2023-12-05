// B64Util.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CB64UtilApp:
// See B64Util.cpp for the implementation of this class
//

class CB64UtilApp : public CWinApp
{
public:
	CB64UtilApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CB64UtilApp theApp;