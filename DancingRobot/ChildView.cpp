
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "DancingRobot.h"
#include "ChildView.h"
#include "SGPolygon.h"
#include "SGRotationTranslation.h"
#include "SGComposite.h"
#include "SGBox.h"
#include "SGSphere.h"
#include "SGCamera.h"
#include "SGLight.h"
#include <MMSystem.h>
#pragma comment(lib, "winmm")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626

// CChildView

CChildView::CChildView()
	: m_lightSpinAngle(0)
{
	m_spinAngle = 0;
	m_spinTimer = 0;
	CreateSceneGraph();
	timeBeginPeriod(1);
	m_lastTime = 0;
	m_am_dancing = 0;
	m_time_dancing = 0;
	m_am_walking = 0;
	m_time_walking = -0.5;
	m_last_step_dist = 0;
	m_dist_walked = 0;
	m_backstride = 2.517854785222098308;
	m_frontstride = 2.1229051435362702;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, COpenGLWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_ROBOTCONTROLS_DANCE, &CChildView::OnRobotcontrolsDance)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_COMMAND(ID_ROBOTCONTROLS_WALK, &CChildView::OnRobotcontrolsWalk)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}





void CChildView::OnGLDraw(CDC* pDC)
{
	// Set up the camera
	// Determine the screen size so we can determine the aspect ratio
	int width, height;
	GetSize(width, height);
	m_camera_hook->SetRes(width, height);
	
	// Render the scenegraph
	// Animation

	m_robot_hook->SetRotate(m_spinAngle, 0, 1, 0);
	DWORD newTime = timeGetTime();
	double delta = (newTime - m_lastTime)*0.001;
	m_lastTime = newTime;

	if (m_am_dancing) {
		m_time_dancing += delta;
		if (m_time_dancing > 7.25) {
			m_time_dancing -= 7.25;
		}
		UpdateDancingArms();
	}
	if (m_am_walking) {
		m_time_walking += delta;
		if (m_time_walking > 16) {
			m_time_walking -= 16;
		}
		UpdateWalking();
	}
	m_scenegraph->Render();
}


void CChildView::CreateSceneGraph()
{
	CSGPtr<CSGComposite> root = new CSGComposite();
	CSGPtr<CSGRotationTranslation> robot = new CSGRotationTranslation();
	CSGPtr<CSGTexture> metal = new CSGTexture(L"graphics/tileable-metal-textures-8.bmp");
	CSGPtr<CSGTexture> red_metal = new CSGTexture(L"graphics/red_head.bmp");
	CSGPtr<CSGTexture> yellow_glass = new CSGTexture(L"graphics/yellow_glass.bmp");
	CSGPtr<CSGTexture> blue_metal = new CSGTexture(L"graphics/blue_metal.bmp");
	CSGPtr<CSGTexture> speaker = new CSGTexture(L"graphics/speaker.bmp");
	CSGPtr<CSGLight> light = new CSGLight(20, 20, 20);
	CSGPtr<CSGCamera> camera = new CSGCamera();
	camera->SetEye(-50, 70, 40);
	m_camera_hook = camera;

	root->AddChild(camera);
	root->AddChild(light);
	root->AddChild(robot);

	// Torso Section
	CSGPtr<CSGRotationTranslation> torso = new CSGRotationTranslation(); {
		CSGPtr<CSGBox> chest_box = new CSGBox(3.5, 5, 1.4);
		chest_box->AddTexture(metal);
		torso->AddChild(chest_box);
		torso->SetTranslate(0, 4.5, 0);

		CSGPtr<CSGRotationTranslation> neck_placer = new CSGRotationTranslation();
		CSGPtr<CSGBox> neck = new CSGBox(1., 2, 1.);
		neck->AddTexture(metal);
		neck_placer->AddChild(neck);
		neck_placer->SetTranslate(0, 4.5, 0);
		torso->AddChild(neck_placer);
	}
	robot->AddChild(torso);

	// Head Section
	CSGPtr<CSGRotationTranslation> head = new CSGRotationTranslation; {
		double head_offset = 6.5;
		CSGPtr<CSGRotationTranslation> skull_placer = new CSGRotationTranslation;
		CSGPtr<CSGSphere> skull = new CSGSphere(1);
		skull->AddTexture(red_metal);
		skull_placer->AddChild(skull);
		skull_placer->SetTranslate(0, head_offset, 0);
		head->AddChild(skull_placer);
		torso->AddChild(head);

		CSGPtr<CSGRotationTranslation> leye_placer = new CSGRotationTranslation;
		CSGPtr<CSGBox> left_eye = new CSGBox(0.3, 0.3, .9);
		left_eye->AddTexture(yellow_glass);
		leye_placer->AddChild(left_eye);
		leye_placer->SetTranslate(0.5, head_offset + .3, 0.5);
		head->AddChild(leye_placer);
		CSGPtr<CSGRotationTranslation> reye_placer = new CSGRotationTranslation;
		CSGPtr<CSGBox> right_eye = new CSGBox(0.3, 0.3, .9);
		right_eye->AddTexture(yellow_glass);
		reye_placer->AddChild(right_eye);
		reye_placer->SetTranslate(-0.5, head_offset + .3, 0.5);
		head->AddChild(reye_placer);

		CSGPtr<CSGRotationTranslation> mouth_placer = new CSGRotationTranslation;
		CSGPtr<CSGBox> mouth = new CSGBox(0.8, 0.15, 0.5);
		mouth->AddTexture(speaker);
		mouth_placer->AddChild(mouth);
		mouth_placer->SetTranslate(0, head_offset - .4, 0.72);
		head->AddChild(mouth_placer);
	}
	m_head_hook = head;

	// Arms Section:
	double arm_xoffset = 1.75 +0.25;
	double arm_yoffset = 9.5 -0.25 - 4.5;
	double arm_zoffset = 0;
	//   Left Arm Section:
	CSGPtr<CSGRotationTranslation> larm = new CSGRotationTranslation; {
		// Left Shoulder
		CSGPtr<CSGSphere> lshoulder = new CSGSphere(0.65);
		lshoulder->AddTexture(blue_metal);
		larm->AddChild(lshoulder);
		larm->SetTranslate(arm_xoffset, arm_yoffset, arm_zoffset);
		larm->SetRotate(200, 0, 0, 1);

		// Left Upper Arm
		CSGPtr<CSGBox> lup_arm = new CSGBox(0.75, 3, 0.75);
		lup_arm->AddTexture(metal);
		larm->AddChild(lup_arm);

		// Left Elbow
		CSGPtr<CSGRotationTranslation> lelb_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> lelbow = new CSGSphere(0.60);
		lelbow->AddTexture(blue_metal);
		lelb_placer->SetTranslate(0, 3.2, 0);
		lelb_placer->AddChild(lelbow);
		larm->AddChild(lelb_placer);
		m_lelbow_hook = lelb_placer;
		m_lelbow_hook->SetRotate(20, 1, 0, 0);
		
		// Left Forearm
		CSGPtr<CSGBox> lfo_arm = new CSGBox(0.625, 2.5, 0.625);
		lfo_arm->AddTexture(metal);
		lelb_placer->AddChild(lfo_arm);

		// Left Wrist
		CSGPtr<CSGRotationTranslation> lwri_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> lwrist = new CSGSphere(0.50);
		lwrist->AddTexture(blue_metal);
		lwri_placer->SetTranslate(0, 2.5, 0);
		lwri_placer->AddChild(lwrist);
		lelb_placer->AddChild(lwri_placer);
		m_lwrist_hook = lwri_placer;

		// Left "Hand"
		CSGPtr<CSGBox> lhand = new CSGBox(1, 0.6, 1);
		lhand->AddTexture(red_metal);
		lwri_placer->AddChild(lhand);
		CSGPtr<CSGRotationTranslation> lhlc_placer = new CSGRotationTranslation();
		CSGPtr<CSGBox> lh_lclaw = new CSGBox(0.2, 0.8, 1);
		lh_lclaw->AddTexture(red_metal);
		lhlc_placer->AddChild(lh_lclaw);
		lhlc_placer->SetTranslate(-0.4, 0.6, 0);
		lwri_placer->AddChild(lhlc_placer);
		CSGPtr<CSGRotationTranslation> lhrc_placer = new CSGRotationTranslation();
		CSGPtr<CSGBox> lh_rclaw = new CSGBox(0.2, 0.8, 1);
		lh_rclaw->AddTexture(red_metal);
		lhrc_placer->AddChild(lh_rclaw);
		lhrc_placer->SetTranslate(0.4, 0.6, 0);
		lwri_placer->AddChild(lhrc_placer);
	}
	torso->AddChild(larm);
	m_lshoulder_hook = larm;

	//   Right Arm Section:
	CSGPtr<CSGRotationTranslation> rarm = new CSGRotationTranslation; {
		// Right Shoulder
		CSGPtr<CSGSphere> rshoulder = new CSGSphere(0.65);
		rshoulder->AddTexture(blue_metal);
		rarm->AddChild(rshoulder);
		rarm->SetTranslate(-arm_xoffset, arm_yoffset, arm_zoffset);
		rarm->SetRotate(160, 0, 0, 1);

		// Right Upper Arm
		CSGPtr<CSGBox> rup_arm = new CSGBox(0.75, 3, 0.75);
		rup_arm->AddTexture(metal);
		rarm->AddChild(rup_arm);

		// Right Elbow
		CSGPtr<CSGRotationTranslation> relb_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> relbow = new CSGSphere(0.60);
		relbow->AddTexture(blue_metal);
		relb_placer->SetTranslate(0, 3.2, 0);
		relb_placer->AddChild(relbow);
		rarm->AddChild(relb_placer);
		m_relbow_hook = relb_placer;
		m_relbow_hook->SetRotate(20, 1, 0, 0);

		// Right Forearm
		CSGPtr<CSGBox> rfo_arm = new CSGBox(0.625, 2.5, 0.625);
		rfo_arm->AddTexture(metal);
		relb_placer->AddChild(rfo_arm);

		// Right Wrist
		CSGPtr<CSGRotationTranslation> rwri_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> rwrist = new CSGSphere(0.50);
		rwrist->AddTexture(blue_metal);
		rwri_placer->SetTranslate(0, 2.5, 0);
		rwri_placer->AddChild(rwrist);
		relb_placer->AddChild(rwri_placer);
		m_rwrist_hook = rwri_placer;

		// Right "Hand"
		CSGPtr<CSGBox> rhand = new CSGBox(1, 0.6, 1);
		rhand->AddTexture(red_metal);
		rwri_placer->AddChild(rhand);
		CSGPtr<CSGRotationTranslation> rhlc_placer = new CSGRotationTranslation();
		CSGPtr<CSGBox> rh_lclaw = new CSGBox(0.2, 0.8, 1);
		rh_lclaw->AddTexture(red_metal);
		rhlc_placer->AddChild(rh_lclaw);
		rhlc_placer->SetTranslate(-0.4, 0.6, 0);
		rwri_placer->AddChild(rhlc_placer);
		CSGPtr<CSGRotationTranslation> rhrc_placer = new CSGRotationTranslation();
		CSGPtr<CSGBox> rh_rclaw = new CSGBox(0.2, 0.8, 1);
		rh_rclaw->AddTexture(red_metal);
		rhrc_placer->AddChild(rh_rclaw);
		rhrc_placer->SetTranslate(0.4, 0.6, 0);
		rwri_placer->AddChild(rhrc_placer);
	}
	torso->AddChild(rarm);
	m_rshoulder_hook = rarm;

	// Legs Section
	double leg_xoffset = 1.75 +0.25;
	double leg_yoffset = 4.5 -0.25;
	double leg_zoffset = 0;
	//   Left Leg Section:
	CSGPtr<CSGRotationTranslation> lleg = new CSGRotationTranslation; {
		// Left Shoulder
		CSGPtr<CSGSphere> lhip = new CSGSphere(0.65);
		lhip->AddTexture(blue_metal);
		lleg->AddChild(lhip);
		lleg->SetTranslate(leg_xoffset, leg_yoffset, leg_zoffset);
		lleg->SetRotate(180, 0, 0, 1);

		// Left Upper Leg
		CSGPtr<CSGBox> lup_leg = new CSGBox(0.75, 3, 0.75);
		lup_leg->AddTexture(metal);
		lleg->AddChild(lup_leg);

		// Left Knee
		CSGPtr<CSGRotationTranslation> lknee_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> lknee = new CSGSphere(0.60);
		lknee->AddTexture(blue_metal);
		lknee_placer->SetTranslate(0, 3, 0);
		lknee_placer->AddChild(lknee);
		lleg->AddChild(lknee_placer);
		m_lknee_hook = lknee_placer;
		
		// Left Foreleg
		CSGPtr<CSGBox> lfo_leg = new CSGBox(0.625, 2.5, 0.625);
		lfo_leg->AddTexture(metal);
		lknee_placer->AddChild(lfo_leg);

		// Left Ankle
		CSGPtr<CSGRotationTranslation> lank_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> lankle = new CSGSphere(0.50);
		lankle->AddTexture(blue_metal);
		lank_placer->SetTranslate(0, 2.5, 0);
		lank_placer->AddChild(lankle);
		lknee_placer->AddChild(lank_placer);
		m_lankle_hook = lank_placer;

		// Left "Foot"
		CSGPtr<CSGRotationTranslation> lfoot_placer = new CSGRotationTranslation;
		CSGPtr<CSGBox> lfoot = new CSGBox(1, 0.6, 2);
		lfoot->AddTexture(metal);
		lfoot_placer->AddChild(lfoot);
		lfoot_placer->SetTranslate(0, 0, 0.5);
		lank_placer->AddChild(lfoot_placer);
	}
	robot->AddChild(lleg);
	m_lhip_hook = lleg;

	//   Right Leg Section:
	CSGPtr<CSGRotationTranslation> rleg = new CSGRotationTranslation; {
		// Right Shoulder
		CSGPtr<CSGSphere> rhip = new CSGSphere(0.65);
		rhip->AddTexture(blue_metal);
		rleg->AddChild(rhip);
		rleg->SetTranslate(-leg_xoffset, leg_yoffset, leg_zoffset);
		rleg->SetRotate(180, 0, 0, 1);

		// Right Upper Leg
		CSGPtr<CSGBox> rup_leg = new CSGBox(0.75, 3, 0.75);
		rup_leg->AddTexture(metal);
		rleg->AddChild(rup_leg);

		// Right Knee
		CSGPtr<CSGRotationTranslation> rknee_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> rknee = new CSGSphere(0.60);
		rknee->AddTexture(blue_metal);
		rknee_placer->SetTranslate(0, 3, 0);
		rknee_placer->AddChild(rknee);
		rleg->AddChild(rknee_placer);
		m_rknee_hook = rknee_placer;

		// Right Foreleg
		CSGPtr<CSGBox> rfo_leg = new CSGBox(0.625, 2.5, 0.625);
		rfo_leg->AddTexture(metal);
		rknee_placer->AddChild(rfo_leg);

		// Right Ankle
		CSGPtr<CSGRotationTranslation> rank_placer = new CSGRotationTranslation();
		CSGPtr<CSGSphere> rankle = new CSGSphere(0.50);
		rankle->AddTexture(blue_metal);
		rank_placer->SetTranslate(0, 2.5, 0);
		rank_placer->AddChild(rankle);
		rknee_placer->AddChild(rank_placer);
		m_rankle_hook = rank_placer;

		// Right "Foot"
		CSGPtr<CSGRotationTranslation> rfoot_placer = new CSGRotationTranslation;
		CSGPtr<CSGBox> rfoot = new CSGBox(1, 0.6, 2);
		rfoot->AddTexture(metal);
		rfoot_placer->AddChild(rfoot);
		rfoot_placer->SetTranslate(0, 0, 0.5);
		rank_placer->AddChild(rfoot_placer);
		
	}
	robot->AddChild(rleg);
	m_rhip_hook = rleg;


	m_scenegraph = root;
	m_robot_hook = robot;
}


void CChildView::OnRobotcontrolsDance()
{
	if (!m_am_dancing){
		m_am_dancing = 1;
		DWORD newTime = timeGetTime();
		m_lastTime = newTime;
		if (m_spinTimer == 0) {
			// Create the timer
			m_spinTimer = SetTimer(1, 30, NULL);
		}
	} else {
		// Destroy the timer
		if (!m_am_dancing && !m_am_walking) {
			KillTimer(m_spinTimer);
			m_spinTimer = 0;
		}
		m_am_dancing = 0;
	}
}


void CChildView::OnRobotcontrolsWalk()
{
	if (!m_am_walking) {
		m_am_walking = 1;
		DWORD newTime = timeGetTime();
		m_lastTime = newTime;
		if (m_spinTimer == 0) {
			// Create the timer
			m_spinTimer = SetTimer(1, 30, NULL);
		}
	}
	else {
		m_am_walking = 0;
		if (!m_am_dancing && !m_am_walking) {
			KillTimer(m_spinTimer);
			m_spinTimer = 0;
		}
	}
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	//m_spinAngle += 5;
	Invalidate();

	COpenGLWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGLWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lastTime = timeGetTime();

	return 0;
}


void CChildView::UpdateDancingArms() {
	// default to "Y"
	double time_into;
	if (m_time_dancing < 0.5) {
		time_into = m_time_dancing;
		m_lshoulder_hook->SetRotate(200 + 135 / 0.5*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(160 - 135 / 0.5*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(20 - 20 / 0.5*time_into, 1, 0, 0);
		m_relbow_hook->SetRotate(20 - 20 / 0.5*time_into, 1, 0, 0);
	}
	// Hold in "Y"
	else if (m_time_dancing >= 0.5 && m_time_dancing < 0.75) {
		m_lshoulder_hook->SetRotate(335, 0, 0, 1);
		m_rshoulder_hook->SetRotate(25, 0, 0, 1);
		m_lelbow_hook->SetRotate(0, 1, 0, 0);
		m_relbow_hook->SetRotate(0, 1, 0, 0);
	}
	// "Y" to "M"
	else if (m_time_dancing >= 0.75 && m_time_dancing < 1.0) {
		time_into = m_time_dancing - 0.75;
		m_lelbow_hook->SetRotate(-115 / 0.25*(time_into), 0, 0, 1);
		m_relbow_hook->SetRotate(115 / 0.25*(time_into), 0, 0, 1);
	}
	// Hold in "M"
	else if (m_time_dancing >= 1.0 && m_time_dancing < 1.25) {
		m_lshoulder_hook->SetRotate(335, 0, 0, 1);
		m_rshoulder_hook->SetRotate(25, 0, 0, 1);
		m_lelbow_hook->SetRotate(-115, 0, 0, 1);
		m_relbow_hook->SetRotate(115, 0, 0, 1);
	}
	// "M" to "C"
	else if (m_time_dancing >= 1.25 && m_time_dancing < 1.5) {
		time_into = m_time_dancing - 1.25;
		m_lelbow_hook->SetRotate(-115 + 40 / 0.25*(time_into), 0, 0, 1);
		m_lshoulder_hook->SetRotate(335 + 5 / 0.25*(time_into), 0, 0, 1);
		m_rshoulder_hook->SetRotate(25 + 150 / 0.25*(time_into), 0, 0, 1);
		m_rshoulder_hook->SetRotate2(-30 / 0.25*(time_into), 1, 0, 0);
		m_relbow_hook->SetRotate(115 - 40 / 0.25*(time_into), 0, 0, 1);
	}
	// Hold "C"
	else if (m_time_dancing >= 1.5 && m_time_dancing < 1.75) {
		m_lshoulder_hook->SetRotate(340, 0, 0, 1);
		m_rshoulder_hook->SetRotate(175, 0, 0, 1);
		m_rshoulder_hook->SetRotate2(-30, 1, 0, 0);
		m_lelbow_hook->SetRotate(-75, 0, 0, 1);
		m_relbow_hook->SetRotate(75, 0, 0, 1);
	}
	// "C" to "A"
	else if (m_time_dancing >= 1.75 && m_time_dancing < 2.0) {
		time_into = m_time_dancing - 1.75;
		m_lshoulder_hook->SetRotate(340 + 35 / 0.25*(time_into), 0, 0, 1);
		m_lelbow_hook->SetRotate(-75 + 75 / 0.25*(time_into), 0, 0, 1);
		m_lwrist_hook->SetRotate(-15 / 0.25*(time_into), 0, 0, 1);
		m_rshoulder_hook->SetRotate(175 - 190 / 0.25*(time_into), 0, 0, 1);
		m_rshoulder_hook->SetRotate2(-30 + 30 / 0.25*(time_into), 1, 0, 0);
		m_relbow_hook->SetRotate(75 - 75 / 0.25*(time_into), 0, 0, 1);
		m_rwrist_hook->SetRotate(15 / 0.25*(time_into), 0, 0, 1);
	}
	// Hold "A"
	else if (m_time_dancing >= 2.0 && m_time_dancing < 2.25) {
		m_lshoulder_hook->SetRotate(15, 0, 0, 1);
		m_lelbow_hook->SetRotate(0, 0, 0, 1);
		m_lwrist_hook->SetRotate(-15, 0, 0, 1);
		m_rshoulder_hook->SetRotate(-15, 0, 0, 1);
		m_rshoulder_hook->SetRotate2(0, 0, 0, 1);
		m_relbow_hook->SetRotate(0, 0, 0, 1);
		m_rwrist_hook->SetRotate(15, 0, 0, 1);
	}
	// "A" to "Egyptian" #1
	else if (m_time_dancing >= 2.25 && m_time_dancing < 2.5) {
		time_into = m_time_dancing - 2.25;
		m_lshoulder_hook->SetRotate(15 - 190 / 0.25*(time_into), 0, 0, 1);
		m_lelbow_hook->SetRotate(85 / 0.25*(time_into), 0, 0, 1);
		m_lwrist_hook->SetRotate(-15 + 15 / 0.25*(time_into), 0, 0, 1);
		m_rshoulder_hook->SetRotate(-15 + 20 / 0.25*(time_into), 0, 0, 1);
		m_relbow_hook->SetRotate(85/0.25*time_into, 0, 0, 1);
		m_rwrist_hook->SetRotate(15 - 15 / 0.25*(time_into), 0, 0, 1);
	}
	// "Egyptian" arms out
	else if (m_time_dancing >= 2.5 && m_time_dancing < 2.7) {
		time_into = m_time_dancing - 2.5;
		m_lshoulder_hook->SetRotate(-175 + 25/0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(85 - 25/0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(5+25/0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(85-25/0.2*time_into, 0, 0, 1);
	}
	// "Egyptian" arms in
	else if (m_time_dancing >= 2.7 && m_time_dancing < 2.9) {
		time_into = m_time_dancing - 2.7;
		m_lshoulder_hook->SetRotate(-150 - 25/0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(60 + 25/0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(30 - 25/0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(60 + 25/0.2*time_into, 0, 0, 1);
	}
	// "Egyptian" arms out
	else if (m_time_dancing >= 2.9 && m_time_dancing < 3.1) {
		time_into = m_time_dancing - 2.9;
		m_lshoulder_hook->SetRotate(-175 + 25/0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(85 - 25/0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(5+25/0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(85-25/0.2*time_into, 0, 0, 1);
	}
	// "Egyptian" arms in
	else if (m_time_dancing >= 3.1 && m_time_dancing < 3.3) {
		time_into = m_time_dancing - 3.1;
		m_lshoulder_hook->SetRotate(-150 - 25/0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(60 + 25/0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(30 - 25/0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(60 + 25/0.2*time_into, 0, 0, 1);
	}
	// Reposition arms for other "Egyptian"
	else if (m_time_dancing >= 3.3 && m_time_dancing < 3.55) {
		time_into = m_time_dancing - 3.3;
		m_lshoulder_hook->SetRotate(-175 + 170/0.25*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(85 - 170/0.25*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(5 + 170/0.25*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(85 - 170/0.25*time_into, 0, 0, 1);
	}
	// "Egyptian" arms out 2,1
	else if (m_time_dancing >= 3.55 && m_time_dancing < 3.75) {
		time_into = m_time_dancing - 3.55;
		m_lshoulder_hook->SetRotate(-5 - 25 / 0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(-85 + 25 / 0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(175 - 25 / 0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(-85 + 25 / 0.2*time_into, 0, 0, 1);
	}
	// "Egyptian" arms in 2,1
	else if (m_time_dancing >= 3.75 && m_time_dancing < 3.95) {
		time_into = m_time_dancing - 3.75;
		m_lshoulder_hook->SetRotate(-30 + 25 / 0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(-60 - 25 / 0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(150 + 25 / 0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(-60 - 25 / 0.2*time_into, 0, 0, 1);
	}
	// "Egyptian" arms out 2,2
	else if (m_time_dancing >= 3.95 && m_time_dancing < 4.15) {
		time_into = m_time_dancing - 3.95;
		m_lshoulder_hook->SetRotate(-5 - 25 / 0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(-85 + 25 / 0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(175 - 25 / 0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(-85 + 25 / 0.2*time_into, 0, 0, 1);
	}
	// "Egyptian" arms in 2,2
	else if (m_time_dancing >= 4.15 && m_time_dancing < 4.35) {
		time_into = m_time_dancing - 4.15;
		m_lshoulder_hook->SetRotate(-30 + 25 / 0.2*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(-60 - 25 / 0.2*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(150 + 25 / 0.2*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(-60 - 25 / 0.2*time_into, 0, 0, 1);
	}
	// Move to arm helicopter pose
	else if (m_time_dancing >= 4.35 && m_time_dancing < 4.6) {
		time_into = m_time_dancing - 4.35;
		m_lshoulder_hook->SetRotate(-5 - 120 / 0.25*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(-85 + 10 / 0.25*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(175 - 145 / 0.25*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(-85 - 5 / 0.25*time_into, 0, 0, 1);
	}
	// Hold arm helicopter pose
	else if (m_time_dancing >= 4.6 && m_time_dancing < 4.75) {
		m_lshoulder_hook->SetRotate(-125, 0, 0, 1);
		m_lelbow_hook->SetRotate(-75, 0, 0, 1);
		m_rshoulder_hook->SetRotate(30, 0, 0, 1);
		m_relbow_hook->SetRotate(-90, 0, 0, 1);
	}
	// Arm helicopter one way
	else if (m_time_dancing >= 4.75 && m_time_dancing < 5.75) {
		time_into = m_time_dancing - 4.75;
		m_relbow_hook->SetRotate2(720*time_into, 0, 1, 0);
	}
	// Arm helicopter other way
	else if (m_time_dancing >= 5.75 && m_time_dancing < 6.75) {
		time_into = m_time_dancing - 5.75;
		m_relbow_hook->SetRotate2(-720 * time_into, 0, 1, 0);
	}
	// Arms back to default
	else if (m_time_dancing >= 6.75 && m_time_dancing <= 7.0) {
		time_into = m_time_dancing - 6.75;
		m_lshoulder_hook->SetRotate(-125 - 35 / 0.25*time_into, 0, 0, 1);
		m_lelbow_hook->SetRotate(20 / 0.25*time_into, 1, 0, 0);
		m_lelbow_hook->SetRotate2(-75 + 75 / 0.25*time_into, 0, 0, 1);
		m_rshoulder_hook->SetRotate(30 + 130 / 0.25*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate2(-90 + 90 / 0.25*time_into, 0, 0, 1);
		m_relbow_hook->SetRotate(0+20 / 0.25*time_into, 1, 0, 0);
	}
	else {
		m_lshoulder_hook->SetRotate(-160, 0, 0, 1);
		m_lelbow_hook->SetRotate(20, 1, 0, 0);
		m_lelbow_hook->SetRotate2(0, 0, 0, 1);
		m_rshoulder_hook->SetRotate(160, 0, 0, 1);
		m_relbow_hook->SetRotate(20, 1, 0, 0);
		m_relbow_hook->SetRotate2(0, 0, 0, 1);
	}
}


void CChildView::UpdateWalking(){
	double time_into;
	double lhip_ang, lknee_ang, lank_ang;
	double rhip_ang, rknee_ang, rank_ang;
	double adj_time, walk_time;
	double temp_dist=0;
	double curr_height;
	// Start pos to starting to walk
	// Left foot straight, right up
	if (m_time_walking >= -0.5 && m_time_walking <= 0){
		time_into = m_time_walking + 0.5;
		lhip_ang = 0;
		lknee_ang = 0;
		lank_ang = lhip_ang - lknee_ang;
		rhip_ang = -10 / 0.5*time_into;
		rknee_ang = -35 / 0.5*time_into;
		rank_ang = rhip_ang - rknee_ang;
		m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
		m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
		m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
		m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
		m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
		m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
	}
	// Walk Forward.
	else if ((m_time_walking > 0 && m_time_walking <= 4) || 
		    (m_time_walking > 12 && m_time_walking <= 16)){
		adj_time = m_time_walking;
		// shift adj_time to range from 0 to 8
		if (adj_time > 12) {
			adj_time -= 12;
		} else {
			adj_time += 4;
		}
		m_dist_walked = floor(adj_time - 4) * (m_frontstride + m_backstride);
		if (fmod(adj_time, 1) > 0.5) {
			m_dist_walked += m_backstride;
		}
		temp_dist = m_dist_walked;
		walk_time = fmod(adj_time, 2);
		// Left foot back, right forward
		if (walk_time > 0 && walk_time <= 0.5) {
			time_into = walk_time;
			lhip_ang = 25 / 0.5*time_into;
			lknee_ang = 5 / 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = -10 - 15/ 0.5*time_into;
			rknee_ang = -35 + 30 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = sin(lhip_ang*PI / 180) * 3 + sin((lhip_ang + lknee_ang)*PI / 180)*2.5 +m_dist_walked;
			curr_height = cos(lhip_ang*PI / 180) * 3 + cos((lhip_ang - lknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
		// Right foot straight, left up
		else if (walk_time > 0.5 && walk_time <= 1) {
			time_into = walk_time - 0.5;
			lhip_ang = 25 - 35 / 0.5*time_into;
			lknee_ang = 5 - 40/ 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = -25 + 25 / 0.5*time_into;
			rknee_ang = -5 + 5 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = m_frontstride - (sin(abs(rhip_ang)*PI / 180) * 3 + sin(abs(rhip_ang - rknee_ang)*PI / 180)*2.5) + m_dist_walked;
			curr_height = cos(rhip_ang*PI / 180) * 3 + cos((rhip_ang - rknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
		// Right foot back, left forward
		else if (walk_time > 1 && walk_time <= 1.5) {
			time_into = walk_time - 1;
			lhip_ang = -10 - 15 / 0.5*time_into;
			lknee_ang = -35 + 30 / 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = 25 / 0.5*time_into;
			rknee_ang = 5 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = sin(rhip_ang*PI / 180) * 3 + sin((rhip_ang + rknee_ang)*PI / 180)*2.5 + m_dist_walked;
			curr_height = cos(rhip_ang*PI / 180) * 3 + cos((rhip_ang - rknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
		// Left foot straight, right up
		else if (walk_time > 1.5 && walk_time <= 2) {
			time_into = walk_time - 1.5;
			lhip_ang = -25 + 25 / 0.5*time_into;
			lknee_ang = -5 + 5 / 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = 25 - 35 / 0.5*time_into;
			rknee_ang = 5 - 40 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = m_frontstride - (sin(abs(lhip_ang)*PI / 180) * 3 + sin(abs(lhip_ang - lknee_ang)*PI / 180)*2.5) + m_dist_walked;
			curr_height = cos(lhip_ang*PI / 180) * 3 + cos((lhip_ang - lknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
	}
	// Walk Backwards
	else if (m_time_walking > 4 && m_time_walking <= 12){
		adj_time = m_time_walking;
		// shift adj_time to range from 0 to 8
		adj_time -= 4;
		m_dist_walked = 4*(m_frontstride + m_backstride) -
						floor(adj_time) * (m_frontstride + m_backstride);
		if (fmod(adj_time, 1) > 0.5) {
			m_dist_walked -= m_frontstride;
		}
		temp_dist = m_dist_walked;
		walk_time = fmod(adj_time, 2);

		// Left foot straight, right up
		if (walk_time > 0 && walk_time <= 0.5) {
			time_into = 0.5 - walk_time;
			lhip_ang = -25 + 25 / 0.5*time_into;
			lknee_ang = -5 + 5 / 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = 25 - 35 / 0.5*time_into;
			rknee_ang = 5 - 40 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = m_backstride - (sin(abs(lhip_ang)*PI / 180) * 3 + sin(abs(lhip_ang - lknee_ang)*PI / 180)*2.5) + m_dist_walked;
			curr_height = cos(lhip_ang*PI / 180) * 3 + cos((lhip_ang - lknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
		// Right foot back, left forward
		else if (walk_time > 0.5 && walk_time <= 1) {
			time_into = 1 - walk_time;
			lhip_ang = -10 - 15 / 0.5*time_into;
			lknee_ang = -35 + 30 / 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = 25 / 0.5*time_into;
			rknee_ang = 5 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = sin(rhip_ang*PI / 180) * 3 + sin((rhip_ang + rknee_ang)*PI / 180)*2.5 + m_dist_walked;
			curr_height = cos(rhip_ang*PI / 180) * 3 + cos((rhip_ang - rknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
		// Right foot straight, left up
		else if (walk_time > 1 && walk_time <= 1.5) {
			time_into = 1.5 - walk_time;
			lhip_ang = 25 - 35 / 0.5*time_into;
			lknee_ang = 5 - 40/ 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = -25 + 25 / 0.5*time_into;
			rknee_ang = -5 + 5 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = m_backstride - (sin(abs(rhip_ang)*PI / 180) * 3 + sin(abs(rhip_ang - rknee_ang)*PI / 180)*2.5) + m_dist_walked;
			curr_height = cos(rhip_ang*PI / 180) * 3 + cos((rhip_ang - rknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
		// Left foot back, right forward
		else if (walk_time > 1.5 && walk_time <= 2) {
			time_into = 2 - walk_time;
			lhip_ang = 25 / 0.5*time_into;
			lknee_ang = 5 / 0.5*time_into;
			lank_ang = lhip_ang - lknee_ang;
			rhip_ang = -10 - 15/ 0.5*time_into;
			rknee_ang = -35 + 30 / 0.5*time_into;
			rank_ang = rhip_ang - rknee_ang;
			m_lhip_hook->SetRotate2(lhip_ang, 1, 0, 0);
			m_lknee_hook->SetRotate(lknee_ang, 1, 0, 0);
			m_lankle_hook->SetRotate(lank_ang, 1, 0, 0);
			m_rhip_hook->SetRotate2(rhip_ang, 1, 0, 0);
			m_rknee_hook->SetRotate(rknee_ang, 1, 0, 0);
			m_rankle_hook->SetRotate(rank_ang, 1, 0, 0);
			temp_dist = sin(lhip_ang*PI / 180) * 3 + sin((lhip_ang + lknee_ang)*PI / 180)*2.5 +m_dist_walked;
			curr_height = cos(lhip_ang*PI / 180) * 3 + cos((lhip_ang - lknee_ang)*PI / 180)*2.5 + 0.6 - 6.1;
			m_robot_hook->SetTranslate(0, curr_height, temp_dist);
		}
	}
}