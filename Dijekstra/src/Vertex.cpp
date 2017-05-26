#include "Vertex.h"

using namespace XMeshLib;

Halfedge *  Vertex::most_ccw_in_halfedge()  
{ 
	if( !m_boundary )
		return m_halfedge; //for interior vertex, randomly pick one, any halfedge can be most ccw

	Halfedge * he = m_halfedge->ccw_rotate_about_target();
	
	while( he )
	{
		m_halfedge = he;
		he = m_halfedge->ccw_rotate_about_target();
	}

	return m_halfedge;
}
	
Halfedge *  Vertex::most_clw_in_halfedge()  
{ 
	if( !m_boundary )
	{
		return most_ccw_in_halfedge()->ccw_rotate_about_target(); 
	}

	Halfedge * he = m_halfedge->clw_rotate_about_target();
	
	while( he )
	{
		m_halfedge = he;
		he = m_halfedge->clw_rotate_about_target();
	}

	return m_halfedge;
}

Halfedge *  Vertex::most_ccw_out_halfedge()  
{ 
	if( !m_boundary )
	{
		return most_ccw_in_halfedge()->he_twin(); 
	}

	Halfedge * he = m_halfedge->he_next();
	Halfedge * ne = he->ccw_rotate_about_source();

	while( ne )
	{
		he = ne;
		ne = he->ccw_rotate_about_source();
	}

	return he;
}
	
Halfedge *  Vertex::most_clw_out_halfedge()  
{ 
	if( !m_boundary )
	{
		Halfedge * he = most_ccw_out_halfedge();
		return he->ccw_rotate_about_source(); 
	}

	Halfedge * he = m_halfedge->he_next();
	Halfedge * ne = he->clw_rotate_about_source();
	
	while( ne )
	{
		he = ne;
		ne = he->clw_rotate_about_source();
	}

	return he;
}