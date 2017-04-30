#include "stdafx.h"
#include "SGTexture.h"
using namespace std;

CSGTexture::CSGTexture(LPCTSTR texname)
{
	m_texture.LoadFile(texname);
}


CSGTexture::~CSGTexture()
{
}

void CSGTexture::Render() {

}

GLuint CSGTexture::Texname() {
	return m_texture.TexName();
}