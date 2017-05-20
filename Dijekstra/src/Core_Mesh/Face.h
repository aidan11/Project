#ifndef _FACE_H_
#define _FACE_H_

namespace XMeshLib
{

class Halfedge;

class Face
{
public:
	Face(){ m_halfedge = NULL;}
	~Face(){;}
	Halfedge    * & he() { return m_halfedge; }
	int		      & id()       { return m_id; }

	std::string & PropertyStr() { return m_propertyStr;}

protected:
	//for half-edge data structure	
	Halfedge	*	m_halfedge;
	
	//additionally
	int				m_id;
	std::string		m_propertyStr;
};

}

#endif