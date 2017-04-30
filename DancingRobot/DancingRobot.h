
// DancingRobot.h : main header file for the DancingRobot application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDancingRobotApp:
// See DancingRobot.cpp for the implementation of this class
//

class CDancingRobotApp : public CWinAppEx
{
public:
	CDancingRobotApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDancingRobotApp theApp;
