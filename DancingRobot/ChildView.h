
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "graphics/OpenGLWnd.h"
#include "SGNode.h"
#include "graphics/GrCamera.h"
#include "SGRotationTranslation.h"
#include "SGCamera.h"


// CChildView window

class CChildView : public COpenGLWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnGLDraw(CDC* pDC);
private:
	void CreateSceneGraph();
	void UpdateDancingArms();
	void UpdateWalking();
	CSGPtr<CSGNode> m_scenegraph;
	int m_lightSpinAngle;
	int m_spinAngle;
	int m_spinTimer;
	CSGPtr<CSGCamera> m_camera_hook;
	CSGPtr<CSGRotationTranslation> m_robot_hook;
	CSGPtr<CSGRotationTranslation> m_head_hook;
	CSGPtr<CSGRotationTranslation> m_lshoulder_hook;
	CSGPtr<CSGRotationTranslation> m_rshoulder_hook;
	CSGPtr<CSGRotationTranslation> m_lelbow_hook;
	CSGPtr<CSGRotationTranslation> m_relbow_hook;
	CSGPtr<CSGRotationTranslation> m_lwrist_hook;
	CSGPtr<CSGRotationTranslation> m_rwrist_hook;
	CSGPtr<CSGRotationTranslation> m_lhip_hook;
	CSGPtr<CSGRotationTranslation> m_rhip_hook;
	CSGPtr<CSGRotationTranslation> m_lknee_hook;
	CSGPtr<CSGRotationTranslation> m_rknee_hook;
	CSGPtr<CSGRotationTranslation> m_lankle_hook;
	CSGPtr<CSGRotationTranslation> m_rankle_hook;
	DWORD m_lastTime;
	double m_time_dancing;
	bool m_am_dancing;
	double m_time_walking;
	bool m_am_walking;
	double m_dist_walked;
	double m_last_step_dist;
	double m_backstride;
	double m_frontstride;

public:
	afx_msg void OnRobotcontrolsDance();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRobotcontrolsWalk();
};

