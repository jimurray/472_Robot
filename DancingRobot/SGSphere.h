#pragma once
#include "SGNode.h"
#include "SGTexture.h"
class CSGSphere :
	public CSGNode
{
public:
	CSGSphere(const double& s_radius);
	~CSGSphere();
	virtual void Render();
	void AddTexture(CSGPtr<CSGTexture>& tex) { m_texture = tex; }
private:
	double m_radius;
	void CSGSphere::SphereFace(int p_recurse, double p_radius, double *a,
		double *b, double *c);
	CSGPtr<CSGTexture>      m_texture;
};

