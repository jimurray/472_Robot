#include "stdafx.h"
#include "SGLight.h"
#include <gl/gl.h>

using namespace std;



CSGLight::~CSGLight()
{
}

void CSGLight::Render() {
	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = { m_x, m_y, m_z, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	//
	// Some standard parameters
	//

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Cull backfacing polygons
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}