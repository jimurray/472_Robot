#pragma once
class CSGNode
{
public:
	CSGNode();
	virtual ~CSGNode();
	void IncRef() { m_refs++; }
	void DecRef() { m_refs--;  if (m_refs == 0) { delete this; } }
	int  RefCnt() const { return m_refs; }
	virtual void Render() = 0;
private:
	int m_refs;
};


// class CSGPtr
// Class that is a pointer to a scene graph node

template <class T> class CSGPtr
{
public:
	CSGPtr() { m_ptr = NULL; }
	CSGPtr(T *p_ptr) { m_ptr = p_ptr;  if (m_ptr) m_ptr->IncRef(); }
	CSGPtr(const CSGPtr &p_ptr) { m_ptr = p_ptr.m_ptr;  if (m_ptr) m_ptr->IncRef(); }
	~CSGPtr() { Clear(); }

	void Clear() { if (m_ptr) { m_ptr->DecRef(); m_ptr = NULL; } }
	T *operator=(T *t) { if (t) t->IncRef();  Clear();  m_ptr = t;  return m_ptr; }
	T *operator=(CSGPtr &t) { if (t.m_ptr) t.m_ptr->IncRef();  Clear();  m_ptr = t.m_ptr;  return m_ptr; }
	operator T *() const { return m_ptr; }
	T *operator->() const { return m_ptr; }

private:
	T *m_ptr;
};
