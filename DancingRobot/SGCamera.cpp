#include "stdafx.h"
#include "SGCamera.h"
#include <gl/gl.h>

using namespace std;



CSGCamera::~CSGCamera()
{
}

void CSGCamera::Render() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	// Set up the camera
	//

	Apply(m_width, m_height);
}