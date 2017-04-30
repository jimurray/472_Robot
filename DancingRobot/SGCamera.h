#pragma once
#include "SGNode.h"
#include "graphics\GrCamera.h"
class CSGCamera :
	public CSGNode,
	public CGrCamera
{
public:
	CSGCamera() : CGrCamera() {};
	~CSGCamera();
	virtual void Render();
	void SetRes(int& wid, int& hei) { m_width = wid, m_height = hei; }
private:
	int m_width, m_height;
};

