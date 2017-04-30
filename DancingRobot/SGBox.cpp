#include "stdafx.h"
#include "SGBox.h"
#include <gl/gl.h>
using namespace std;


CSGBox::CSGBox(const double& b_width, const double& b_height, const double& b_length)
{
	m_width = b_width;
	m_height = b_height;
	m_length = b_length;
	m_texture = NULL;

	CGrVector v0(m_width / 2, 0, m_length / 2);
	CGrVector v1(-m_width / 2, 0, m_length / 2);
	CGrVector v2(-m_width / 2, 0, -m_length / 2);
	CGrVector v3(m_width / 2, 0, -m_length / 2);
	CGrVector v4(m_width / 2, m_height, m_length / 2);
	CGrVector v5(-m_width / 2, m_height, m_length / 2);
	CGrVector v6(-m_width / 2, m_height, -m_length / 2);
	CGrVector v7(m_width / 2, m_height, -m_length / 2);

	m_normals.push_back(CGrVector(0, -1, 0));
	m_vertices.push_back(v0);
	m_vertices.push_back(v1);
	m_vertices.push_back(v2);
	m_vertices.push_back(v3);
	
	m_normals.push_back(CGrVector(-1, 0, 0));
	m_vertices.push_back(v1);
	m_vertices.push_back(v5);
	m_vertices.push_back(v6);
	m_vertices.push_back(v2);

	m_normals.push_back(CGrVector(0, 0, -1));
	m_vertices.push_back(v2);
	m_vertices.push_back(v6);
	m_vertices.push_back(v7);
	m_vertices.push_back(v3);

	m_normals.push_back(CGrVector(0, 1, 0));
	m_vertices.push_back(v4);
	m_vertices.push_back(v7);
	m_vertices.push_back(v6);
	m_vertices.push_back(v5);

	m_normals.push_back(CGrVector(1, 0 , 0));
	m_vertices.push_back(v0);
	m_vertices.push_back(v3);
	m_vertices.push_back(v7);
	m_vertices.push_back(v4);

	m_normals.push_back(CGrVector(0, 0, 1));
	m_vertices.push_back(v0);
	m_vertices.push_back(v4);
	m_vertices.push_back(v5);
	m_vertices.push_back(v1);

	CGrVector t0(0,0);
	CGrVector t1(1, 0);
	CGrVector t2(1, 1);
	CGrVector t3(0, 1);
	for (int i = 0; i < 6; i++) {
		m_tvertices.push_back(t0);
		m_tvertices.push_back(t1);
		m_tvertices.push_back(t2);
		m_tvertices.push_back(t3);
	}
}


CSGBox::~CSGBox()
{
}


void CSGBox::Render() {
	// Create an iterator for each of the three arrays
	vector<CGrVector>::iterator v = m_vertices.begin();
	vector<CGrVector>::iterator n = m_normals.begin();
	vector<CGrVector>::iterator t = m_tvertices.begin();

	
	if (m_texture) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, m_texture->Texname());
	}
	glBegin(GL_QUADS);
	int vert_count = 0;
	for (; v != m_vertices.end(); v++)
	{
		// If we have an available normal, use it:
		if ((m_vertices.size() == m_normals.size() || vert_count % 4 == 0) && n != m_normals.end())
		{
			glNormal3dv(*n);
			n++;
		}

		// If we have an available texture coordinate, use it:
		if (t != m_tvertices.end())
		{
			glTexCoord2dv(*t);
			t++;
		}

		glVertex3dv(*v);
		vert_count++;
		
	}
	glEnd();
	if (m_texture) {
		glDisable(GL_TEXTURE_2D);
	}
	
}