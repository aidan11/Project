#ifndef _HALFEDGE_H_
#define _HALFEDGE_H_

#include "Edge.h"

namespace XMeshLib
{

class Vertex;
class Face;

class Halfedge
{
public:
	Halfedge(){ m_edge = NULL; m_vertex = NULL; m_prev = NULL; m_next = NULL; m_face = NULL;}
	~Halfedge()	{;}
	Edge     * & edge()    { return m_edge;   }
	Vertex   * & vertex()  { return m_vertex; }
	Vertex   * & target()  { return m_vertex; }
	Vertex   * & source()  { return m_prev->vertex();}
	Halfedge * & he_prev() { return m_prev;}
	Halfedge * & he_next() { return m_next;}
	Halfedge * & he_twin()  { return m_edge->twin(this); }
	Face     * & face()    { return m_face;}

	Halfedge * ccw_rotate_about_target();
	Halfedge * clw_rotate_about_target();
	Halfedge * ccw_rotate_about_source();
	Halfedge * clw_rotate_about_source();

protected:
	Edge     *     m_edge;
    Face     *     m_face;
	Vertex   *     m_vertex;		//target vertex
	Halfedge *	   m_prev;
	Halfedge *     m_next;
};
}

#endif 