#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Halfedge.h"
#include "Point.h"
#include "Trait.h"

namespace XMeshLib
{

class Vertex
{
public:		
	Vertex()
	{ 
		m_halfedge = NULL;
		m_boundary = false;
		m_traits = new TraitList();
	}
	~Vertex()
	{
		if (m_traits)
		{
			m_traits->clear();
			delete m_traits;
		}
	}

	Point & point() { return  m_point; }
	Halfedge * & he(){ return m_halfedge; }

	int & id() { return m_id; }		
	bool & boundary() { return m_boundary; }	//whether this is a boundary vertex

	std::string & PropertyStr() { return m_propertyStr;}
	
    Halfedge *  most_ccw_in_halfedge();
	Halfedge *  most_ccw_out_halfedge();
	Halfedge *  most_clw_in_halfedge();
	Halfedge *  most_clw_out_halfedge();

protected:
	//for half-edge data structure
	Point			m_point;
	Halfedge	*	m_halfedge;

	//additionally
	int				m_id;		
	bool			m_boundary; //whether this is a boundary vertex
	std::string		m_propertyStr;
	TraitList		* m_traits;

public: //operations for traits, you can write traits for other primitives
	TraitList * & traits() { return m_traits; }

};

}

#endif



//void AddTrait(TraitNode * t) 
//{	
//	TraitList * tl = this->traits();
//	if (!tl)
//	{
//		tl = new TraitList;
//		this->traits() = tl;
//	}
//	tl->addTrait(t);
//}
//
//TraitNode * GetTrait(int tind)
//{
//	TraitList * tl = this->traits();
//	if (!tl)
//		return NULL;
//	else
//		return tl->getTrait(tind);
//}
//bool DelTrait(int tind)
//{
//	TraitList * tl = this->traits();
//	if (!tl)
//		return false;
//	else
//		return tl->delTrait(tind);
//}