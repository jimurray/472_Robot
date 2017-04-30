#pragma once
#include "SGNode.h"
class CSGLight :
	public CSGNode
{
public:
	CSGLight(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
	~CSGLight();
	virtual void Render();
private:
	double m_x;
	double m_y;
	double m_z;
};

