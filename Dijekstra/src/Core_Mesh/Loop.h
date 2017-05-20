#ifndef _LOOP_H_
#define _LOOP_H_

#include "Mesh.h"
#include "Iterators.h"
#include <vector>

namespace XMeshLib
{
	class Loop
	{
	public:
		Loop(){;}
		~Loop(){;}
		void TraceBoundaryLoop(Halfedge * startHE)
		{
			Halfedge* he = startHE;
			do {
				helist.push_back(he);
				Halfedge* he1= he;
				do {
					he1 = he1->he_next();
					he = he1;
					he1 = he1->he_twin();
				} while (he1!= NULL);
			} while (he!=startHE);
		}
		int lenth(){return helist.size();}
		Halfedge * he(int i){return helist[i];}
		

	public:
		std::vector<Halfedge *> helist;
	};


}


#endif
