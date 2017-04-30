#pragma once
#include "SGNode.h"
#include <vector>
#include "graphics/GrVector.h"
#include "SGTexture.h"
class CSGBox :
	public CSGNode
{
public:
	CSGBox(const double& b_width, const double& b_height, const double& b_length);
	~CSGBox();
	virtual void Render();
	void AddTexture(CSGPtr<CSGTexture>& tex) { m_texture = tex; }
private:
	double m_width;
	double m_height;
	double m_length;
	std::vector<CGrVector>   m_vertices;
	std::vector<CGrVector>   m_tvertices;
	std::vector<CGrVector>   m_normals;
	CSGPtr<CSGTexture>      m_texture;
};

