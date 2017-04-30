#include "stdafx.h"
#include "SGComposite.h"


CSGComposite::CSGComposite()
{
}


CSGComposite::~CSGComposite()
{
}

void CSGComposite::Render() {
	for (std::vector<CSGPtr<CSGNode> >::iterator child = m_children.begin();
		child != m_children.end();  child++)
	{
		(*child)->Render();
	}
}
