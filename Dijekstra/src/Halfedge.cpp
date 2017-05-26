#include "Halfedge.h"

using namespace XMeshLib;

Halfedge * Halfedge::ccw_rotate_about_target()
{
	Halfedge * he_dual = he_twin();
	if (he_dual) return he_dual->he_prev();
	else return NULL;
}

Halfedge * Halfedge::clw_rotate_about_target()
{
	return he_next()->he_twin();
}

Halfedge * Halfedge::ccw_rotate_about_source()
{
	return he_prev()->he_twin();
}

Halfedge * Halfedge::clw_rotate_about_source()
{
	Halfedge * he = he_twin();
	if (he) return he->he_next();
	else return NULL;
}