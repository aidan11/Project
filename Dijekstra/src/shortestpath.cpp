#include "shortestpath.h"

using namespace XMeshLib;


void shortestpath::delver(std::vector <Vertex*> &lst, Vertex* V)
{
	if (lst.size() == 0)
	{
		std::cout << "the list is empty.";
		return;
	}
	for (int i = 0; i<lst.size(); i++)
	{
		if (lst[i]->point()[0] == V->point()[0] && lst[i]->point()[1] == V->point()[1] && lst[i]->point()[2] == V->point()[2])
			lst.erase(lst.begin() + i);
	}

}

double WW(Vertex* V, Vertex* W)
{
	Point p1 = V->point();
	Point p2 = W->point();
	return (p1 - p2).norm();
}

void shortestpath::SetEdgesWeight(void)
{
	std::cout << "Computing lengths of edges...";
	for (MeshEdgeIterator vit(cmesh); !vit.end(); ++vit)
	{
		Edge *E = *vit;
		Halfedge *he1 = E->he(0);
		Point p1 = he1->source()->point();
		Point p2 = he1->target()->point();

		double length = (p1 - p2).norm();

		edgeweight[E] = length;

	}
	std::cout << "Done!" << std::endl;
}

//this function returns calculates the geodesic distance between a source point and every other point
void shortestpath::getgeodesicfunction(void)
{
	//SetEdgesWeight();
	std::cout << "Storing the distance function with respect the input vertex..";
	std::map<Vertex*, bool> visited;
	for (MeshVertexIterator it(cmesh); !it.end(); ++it)	{
		Vertex * v = *it;
		visited[v] = 0;
		D[v] = 1000000;
	}

	Vertex *v = source;
	D[v] = 0;
	visited[v] = 1;
	prev[v] = v;
	std::vector<Vertex*> cont;
	cont.push_back(v);
	while (!cont.empty())
	{
		Vertex *pick = cont[0];
		int j = 0;
		for (int i = 0; i<cont.size(); i++)
		{
			if (D[cont[i]]<D[pick])
			{
				pick = cont[i];
				j = i;
			}
		}
		visited[pick] = 1;
		cont.erase(cont.begin() + j);
		for (VertexVertexIterator hit(pick); !hit.end(); ++hit)
		{

			Vertex *mv = *hit;
			Edge *E = cmesh->vertexEdge(pick, mv);
			if (edgeweight[E] + D[pick]<D[mv])
			{
				D[mv] = edgeweight[E] + D[pick];
				prev[mv] = pick;
				if (visited[mv] == 0)
					cont.push_back(mv);
			}
		}

	}
	std::cout << "Done!" << std::endl;
}

//stores all the geodesic edges between source and another vertex endh
std::vector<Edge*> shortestpath::geodesic(Vertex* endh)
{
	std::vector<Edge*> out;
	Vertex *end = endh;
	Vertex *onestepbefore = prev[end];
	out.push_back(cmesh->vertexEdge(end, onestepbefore));
	while (end != source)
	{
		onestepbefore = prev[onestepbefore];
		end = prev[end];
		out.push_back(cmesh->vertexEdge(end, onestepbefore));
	}

	return out;
}

//store all the vertices on geodesic path between source and another vertex "endh" 
std::vector<Vertex*> shortestpath::geodesicpoints(Vertex* endh)
{
	std::vector<Vertex*> out;
	Vertex *end = endh;
	out.push_back(end);
	while (end != source)
	{
		end = prev[end];
		out.push_back(end);
	}

	return out;
}


//get the geodesic path that connects between 2 vertices
std::vector<Vertex*> shortestpath::geodesicpath(Vertex* begin, Vertex* end)
{
	SetEdgesWeight();
	int n = cmesh->numVertices();
	std::map<Vertex*, double> Dis;
	std::map<Vertex*, Vertex*> previous;

	std::vector<Vertex*>initialcontainer;
	for (MeshVertexIterator it(cmesh); !it.end(); ++it)	{
		Vertex * v = *it;
		initialcontainer.push_back(v);
		D[v] = 1000000;
	}

	Vertex *v = begin;
	Dis[v] = 0;
	previous[v] = v;
	while (initialcontainer.size() != 0)
	{
		Vertex *pick = initialcontainer[0];
		for (int i = 0; i<initialcontainer.size(); i++)
		{
			if (Dis[initialcontainer[i]]<Dis[pick])
				pick = initialcontainer[i];
		}
		delver(initialcontainer, pick);

		//check if we are visiting a vertex
		for (VertexVertexIterator hit(pick); !hit.end(); ++hit)
		{
			Vertex *mv = *hit;
			Edge *E = cmesh->vertexEdge(pick, mv);
			//edgeweight[E]=WW(pick,mv)
			if (edgeweight[E] + Dis[pick]<Dis[mv])
			{
				Dis[v] = edgeweight[E] + Dis[pick];
				previous[mv] = pick;
			}
		}

	}

	std::vector<Vertex*> out;
	out.push_back(end);
	while (end != source)
	{
		end = prev[end];
		out.push_back(end);
	}

	return out;

}