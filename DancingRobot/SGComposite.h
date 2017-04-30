#pragma once
#include "SGNode.h"
#include <vector>
class CSGComposite :
	public CSGNode
{
public:
	CSGComposite();
	~CSGComposite();
	virtual void Render();
	void AddChild(CSGNode * child) { m_children.push_back(child); }
private:
	std::vector<CSGPtr<CSGNode> > m_children;
};

