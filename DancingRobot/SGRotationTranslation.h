#pragma once
#include "SGNode.h"
#include <vector>

class CSGRotationTranslation :
	public CSGNode
{
public:
	CSGRotationTranslation();
	~CSGRotationTranslation();
	virtual void Render();
	void SetRotate(double angle, double rx, double ry, double rz)
		{m_angle = angle; m_rx = rx; m_ry = ry; m_rz = rz;}
	void SetRotate2(double angle, double rx, double ry, double rz)
		{m_angle2 = angle; m_rx2 = rx; m_ry2 = ry; m_rz2 = rz;}
	void SetTranslate(double tx, double ty, double tz)
		{ m_tx = tx; m_ty = ty; m_tz = tz; }
	void AddChild(CSGNode * child) { m_children.push_back(child); }
private:
	double  m_angle, m_rx, m_ry, m_rz;      // Rotation definition
	double  m_angle2, m_rx2, m_ry2, m_rz2;
	double  m_tx, m_ty, m_tz;               // Translation definition
	std::vector<CSGPtr<CSGNode> > m_children;
};

