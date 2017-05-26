#ifndef _EDGE_H_
#define _EDGE_H_

#include <cassert>
#include <string>

namespace XMeshLib
{

class Halfedge;

class Edge
{
public:
	Edge(){m_halfedge[0]=NULL;m_halfedge[1]=NULL;}
	Edge(Halfedge * he0, Halfedge * he1){m_halfedge[0]=he0;m_halfedge[1]=he1;}
	~Edge(){;}

	Halfedge * & he (int i) { return m_halfedge[i];}
	
	bool boundary() { return (!m_halfedge[0] || !m_halfedge[1]); }
		
	std::string & PropertyStr() { return m_propertyStr;}
	
	Halfedge * & twin( Halfedge * he ) { 
		if (he==m_halfedge[0])
			return m_halfedge[1];
		else 
		{
			assert(he==m_halfedge[1]);
			return m_halfedge[0];
		}
	}
	
	bool operator== (Edge & e) const
    {
		if (m_halfedge[0] == e.he(0) && m_halfedge[1] == e.he(1))
			return true;
		else if (m_halfedge[0] == e.he(1) && m_halfedge[1] == e.he(0))
			return true;
		return false;
    }

protected:	
	//for halfedge data structure
	Halfedge	*	m_halfedge[2];

	//additionally
	std::string		m_propertyStr;
};

}

#endif
