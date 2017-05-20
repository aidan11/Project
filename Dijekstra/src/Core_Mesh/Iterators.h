#ifndef _ITERATORS_H_
#define _ITERATORS_H_

#include "Vertex.h"
#include "Mesh.h"

namespace XMeshLib
{

class VertexOutHalfedgeIterator
{
public:
	VertexOutHalfedgeIterator( Mesh *  Mesh, Vertex *  v )
	{ 
		m_Mesh = Mesh; 
		m_vertex = v; 
		m_halfedge = v->most_clw_out_halfedge();
	}

	~VertexOutHalfedgeIterator(){};
	void operator++()
	{
		if( m_halfedge == m_vertex->most_ccw_out_halfedge() ) 
			m_halfedge = NULL;
		else
			m_halfedge = m_halfedge->ccw_rotate_about_source(); 
	}
	Halfedge * value() { return m_halfedge; }
	bool end(){ return m_halfedge == NULL; }
	Halfedge * operator*() { return value(); }

private:
	Mesh *    m_Mesh;
	Vertex *   m_vertex;
	Halfedge * m_halfedge;		
};

class VertexInHalfedgeIterator
{
public:
	VertexInHalfedgeIterator( Mesh *  Mesh, Vertex * v )
	{ 
		m_Mesh = Mesh; 
		m_vertex = v; 
		m_halfedge = v->most_clw_in_halfedge();
	}
	~VertexInHalfedgeIterator(){;}
	void operator++()
	{
		if( m_halfedge == m_vertex->most_ccw_in_halfedge())
			m_halfedge = NULL; 
		else
			m_halfedge = m_halfedge->ccw_rotate_about_target();
	}
	Halfedge * value() { return m_halfedge; }
	bool end(){ return m_halfedge == NULL; }
	Halfedge * operator*() { return value(); }

private:
	Mesh *    m_Mesh;
	Vertex *   m_vertex;
	Halfedge * m_halfedge;
};

class VertexVertexIterator
{
public:
	VertexVertexIterator( Vertex *  v )
	{ 
		m_vertex = v; 
		m_halfedge = m_vertex->most_clw_out_halfedge();
	}
	~VertexVertexIterator(){};
	void operator++()
	{
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}
		if( m_vertex->boundary() )
		{
			if( m_halfedge == m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}
			Halfedge * he = m_halfedge->ccw_rotate_about_source();
			if( he == NULL )
			{
				m_halfedge = m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}
		return;
	}
	Vertex * value() 
	{ 
		if( !m_vertex->boundary() )
		{
			return m_halfedge->target(); 
		}
		if( m_halfedge != m_vertex->most_ccw_in_halfedge() )
		{
			return m_halfedge->target();
		}
		if( m_halfedge == m_vertex->most_ccw_in_halfedge() )
		{
			return m_halfedge->source();
		}
		return NULL;
	}
	
	Vertex * operator*() { return value(); }
	bool end(){ return m_halfedge == NULL; }
	void reset()	{ m_halfedge = m_vertex->most_clw_out_halfedge(); }
private:
	Vertex *   m_vertex;
	Halfedge * m_halfedge;
};

class VertexEdgeIterator
{
public:
	VertexEdgeIterator( Vertex *  v )
	{ 
		m_vertex = v; 
		m_halfedge = m_vertex->most_clw_out_halfedge();
	};
	~VertexEdgeIterator(){};
	void operator++()
	{
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			Halfedge * he = m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	}
	Edge * value() 
	{ 
		return m_halfedge->edge();
	}

	Edge * operator*() { return value(); }

	bool end(){ return m_halfedge == NULL; }
	void reset()	{ m_halfedge = m_vertex->most_clw_out_halfedge(); }
private:
	Vertex *   m_vertex;
	Halfedge * m_halfedge;
};

class VertexFaceIterator
{
public:
	VertexFaceIterator( Vertex * v )
	{ 
		m_vertex = v; //assign v to m_vertex, which will be used
		m_halfedge = m_vertex->most_clw_out_halfedge(); 
	};

	~VertexFaceIterator(){};

	void operator++()
	{
		if( m_halfedge == m_vertex->most_ccw_out_halfedge() ) 
		{
			m_halfedge = NULL;
			return;
		}
		m_halfedge = m_halfedge->ccw_rotate_about_source();
	};

	Face * value() { return m_halfedge->face(); };
	Face * operator*() { return value(); };
	bool end(){ return m_halfedge == NULL; };
	void reset()	{ m_halfedge = m_vertex->most_clw_out_halfedge(); };

private:
	Vertex *   m_vertex;
	Halfedge * m_halfedge;
};

// f -> halfedge
class FaceHalfedgeIterator
{
public:

	FaceHalfedgeIterator( Face * f )
	{ 
		m_face = f; 
		m_halfedge = f->he(); 
	};

	~FaceHalfedgeIterator(){};

	void operator++()
	{
		m_halfedge = m_halfedge->he_next();

		if( m_halfedge == m_face->he() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	Halfedge * value() { return m_halfedge; };
	Halfedge * operator*() { return value(); };

	bool end(){ return m_halfedge == NULL; };

private:
	Face *     m_face;
	Halfedge * m_halfedge;
};


// f -> edge
class FaceEdgeIterator
{
public:

	FaceEdgeIterator( Face * f )
	{ 
		m_face = f; 
		m_halfedge = f->he(); 
	};

	~FaceEdgeIterator(){};

	void operator++()
	{
		m_halfedge = m_halfedge->he_next();

		if( m_halfedge == m_face->he() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	Edge * value() { return m_halfedge->edge(); };
	Edge * operator*() { return value(); };

	bool end(){ return m_halfedge == NULL; };

private:
	Face *     m_face;
	Halfedge * m_halfedge;
};


// f -> vertex
class FaceVertexIterator
{
public:

	FaceVertexIterator( Face * f )
	{ 
		m_face = f; 
		m_halfedge = f->he(); 
	};

	~FaceVertexIterator(){};

	void operator++()
	{
		m_halfedge = m_halfedge->he_next();

		if( m_halfedge == m_face->he() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	Vertex * value() { return m_halfedge->target(); };
	Vertex * operator*() { return value(); };

	bool end(){ return m_halfedge == NULL; };

private:
	Face *     m_face;
	Halfedge * m_halfedge;
};


// soild vertices
class MeshVertexIterator
{
public:
	MeshVertexIterator(Mesh * Mesh ):
	  m_Mesh(Mesh)
	{
		m_iter = m_Mesh->m_verts.begin();
	}

	Vertex * value() { return *m_iter; }

	void operator++() 
	{ 
		++m_iter; 
	}

	void operator++(int) 
	{ 
		++m_iter; 
	}
	
	bool end() { return m_iter == m_Mesh->m_verts.end();}

	Vertex * operator*(){ return value(); }

	void reset() { m_iter = m_Mesh->m_verts.begin(); }

private:
	std::list<Vertex *>::iterator m_iter;
	Mesh * m_Mesh;
};

// soild faces
class MeshFaceIterator
{
public:
	MeshFaceIterator(Mesh * cmesh )
	{
		m_Mesh = cmesh;			
		m_iter = m_Mesh->m_faces.begin();
	}

	Face * value() { return *m_iter; }

	void operator++() { ++m_iter;}

	bool end() { return m_iter == m_Mesh->m_faces.end(); }

	Face * operator*(){ return value(); }

	void reset() { m_iter = m_Mesh->m_faces.begin();}

private:
	//int ind;
	Mesh * m_Mesh;
	std::list<Face *>::iterator m_iter;
};

// soild edges
class MeshEdgeIterator
{
public:
	MeshEdgeIterator(Mesh * Mesh )
	{
		m_Mesh = Mesh;			
		m_iter = m_Mesh->m_edges.begin();
	}

	Edge * value() {  return *m_iter; };

	void operator++() { ++m_iter;}

	bool end() { return m_iter == m_Mesh->m_edges.end(); }

	Edge * operator*(){ return value(); }

	void reset() { m_iter = m_Mesh->m_edges.begin();}

private:		
	Mesh * m_Mesh;
	std::list<Edge *>::iterator m_iter;
};

// soild halfedges
class MeshHalfedgeIterator
{
public:
	MeshHalfedgeIterator( Mesh * Mesh )
	{
		m_Mesh = Mesh;			
		m_id = 0;
		m_iter = m_Mesh->m_edges.begin();
	}

	Halfedge * value() {			
		Edge * e = *m_iter; 
		return e->he(m_id); 
	};

	void operator++() 
	{ 
		++m_id;

		switch( m_id )
		{
		case 1:
			{
				Edge * e = *m_iter;
				if( e->he(m_id) == NULL )
				{
					m_id = 0;
					++m_iter;						
				}
			}
			break;
		case 2:
			m_id = 0;
			++m_iter;				
			break;
		}
	};

	bool end() { return (m_iter == m_Mesh->m_edges.end()) ;}

	Halfedge * operator*(){ return value(); };

	void reset() { m_id = 0; m_iter = m_Mesh->m_edges.begin();};

private:		
	Mesh * m_Mesh;
	std::list<Edge *>::iterator m_iter;
	int  m_id;
};

}

#endif
