#include "stdafx.h"
#include "SGRotationTranslation.h"
#include <gl/gl.h>


CSGRotationTranslation::CSGRotationTranslation()
{
	m_angle = 0;
	m_ry = 0;
	m_rz = 0;
	m_rx = 1;
	m_tx = 0;
	m_ty = 0;
	m_tz = 0;
	m_angle2 = 0;
	m_ry2 = 0;
	m_rz2 = 0;
	m_rx2 = 1;
}


CSGRotationTranslation::~CSGRotationTranslation()
{
}

void CSGRotationTranslation::Render() {
	glPushMatrix();
	glTranslated(m_tx, m_ty, m_tz);
	if (m_angle2 != 0)
		glRotated(m_angle2, m_rx2, m_ry2, m_rz2);

	if (m_angle != 0)
		glRotated(m_angle, m_rx, m_ry, m_rz);

	for (std::vector<CSGPtr<CSGNode> >::iterator child = m_children.begin();
		child != m_children.end();  child++)
	{
		(*child)->Render();
	}
	glPopMatrix();
}