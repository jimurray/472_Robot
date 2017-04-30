#pragma once
#include "SGNode.h"
#include "graphics/GrTexture.h"
#include <string>
class CSGTexture :
	public CSGNode
{
public:
	CSGTexture(LPCTSTR texname);
	~CSGTexture();
	virtual void Render();
	GLuint Texname();
private:
	CGrTexture m_texture;
};

