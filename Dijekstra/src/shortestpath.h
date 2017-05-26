#ifndef SHORTESTPATH_H_
#define SHORTESTPATH_H_

#include "Mesh.h"
#include "Iterators.h"
#include <vector>
#include <queue>
#include <map>
#include <cstdlib>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <hash_map>
#include <fstream>
#include "zerocochain.h"
//#include "scalarfunction.h"
//#include "perturbzerocochain.h"

//Dijkstra implementation for distance function from a source vertex 

namespace XMeshLib
{

	class shortestpath :public ZeroCochain
	{
	public:
		shortestpath()
		{ 
			
			
		}
		shortestpath(Mesh *mesh)
		{ 
			mesh=cmesh;
			
		}

		shortestpath(Mesh *mesh,Vertex* V)
		{ 
			mesh=cmesh;
			source=V;
		}

		~shortestpath()
		{ 
		}
	private:
		//each vertex has a vertex that is previous to it as far as the distance from a source is concerned
		std::map<Vertex*,Vertex*> prev;

		//edge weight is basically the length of the edge
		std::map<Edge*,double> edgeweight;

		//delete vertex from a list of vertices
		void delver(std::vector <Vertex*> &lst, Vertex* V );
	public:
		//getting the geodisic edges sequence from the source point to the input vertex
		std::vector<Edge*> geodesic(Vertex*);
		//getting the geodisic vertices sequence from the source point to the input vertex
		std::vector<Vertex*> geodesicpoints(Vertex*);
		
		//get the geodesic path that connects between 2 vertices
		std::vector<Vertex*> geodesicpath(Vertex*,Vertex*);

		//Store the edges lengths
		void SetEdgesWeight(void);
		//Store the distance function from the source vertex "source"
		void getgeodesicfunction(void);

		//D is the distance function
		std::map<Vertex*,double> D;
		Vertex *source;


	};
}



#endif