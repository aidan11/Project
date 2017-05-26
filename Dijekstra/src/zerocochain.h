#ifndef ZEROCOCHAIN_H_
#define ZEROCOCHAIN_H_

#include "Mesh.h"
#include "Iterators.h"
#include <vector>
#include <queue>
#include <map>
#include <cstdlib>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <hash_map>
#include <fstream>



//this is a class for scalar map on a mesh. It can do all arithmatics on these scalar map
namespace XMeshLib
{
	class ZeroCochain
	{
	public:
		ZeroCochain()
		{ 
		}

		ZeroCochain(Mesh *inmesh )
		{ 
			cmesh=inmesh;
		}

		~ZeroCochain()
		{ 
		}
	public:
		Mesh *cmesh;
		std::map<Vertex*,double> VerticesMap;

	public:
		double & operator[]( Vertex * vert )
		{
			return VerticesMap[vert];
		}

		//set ZeroCochain 

		void SetZeroCochain(std::map<Vertex*,double> zoc)
		{
			VerticesMap=zoc;
		}


		//a*f where a is a real number and f is a zerochain
		ZeroCochain operator*( double scaler )
		{
			ZeroCochain out;
			out.cmesh=cmesh;
			out.VerticesMap=VerticesMap;


			for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
			{
				Vertex *v=it->first;
				out.VerticesMap[v]=scaler*VerticesMap[v];

			}

			return out;
		}
		//f1*f2 where f1 and f2 are zerochains
		ZeroCochain  operator*( ZeroCochain chain )
		{
			ZeroCochain out;
			out.cmesh=cmesh;
			out.VerticesMap=VerticesMap;

			if(chain.cmesh!=cmesh)
			{
				std::cout<<"error, product of cochain is only valid on the same mesh"<<std::endl;
			}
			for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
			{
				Vertex *v=it->first;
				out.VerticesMap[v]=chain.VerticesMap[v]*VerticesMap[v];

			}

			return out;
		}

		//f1/f2 where f1 and f2 are zerochains
		ZeroCochain  operator/( ZeroCochain chain )
		{
			ZeroCochain out;
			out.cmesh=cmesh;
			out.VerticesMap=VerticesMap;

			if(chain.cmesh!=cmesh)
			{
				std::cout<<"error, division of cochain is only valid on the same mesh"<<std::endl;
			}
			for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
			{
				Vertex *v=it->first;
				if(chain.VerticesMap[v]!=0)
					out.VerticesMap[v]=VerticesMap[v]/chain.VerticesMap[v];
				else
					out.VerticesMap[v]=1000000;

			}

			return out;
		}

		//f1/a where f1 is zerochains and a is a real number
		ZeroCochain  operator/( double scaler )
		{
			ZeroCochain out;
			out.cmesh=cmesh;
			out.VerticesMap=VerticesMap;

			
			for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
			{
				Vertex *v=it->first;
				out.VerticesMap[v]=VerticesMap[v]/scaler;

			}

			return out;
		}


		//f1+f2 where f1 and f2 are zerochains
		ZeroCochain  operator+( ZeroCochain chain )
		{
			ZeroCochain out;
			out.cmesh=cmesh;
			out.VerticesMap=VerticesMap;

			if(chain.cmesh!=cmesh)
			{
				std::cout<<"error, summation of cochains is only valid on the same mesh"<<std::endl;
			}
			for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
			{
				Vertex *v=it->first;
				out.VerticesMap[v]=chain.VerticesMap[v]+VerticesMap[v];

			}

			return out;
		}
		//f1-f2 where f1 and f2 are zerochains
			ZeroCochain  operator-( ZeroCochain chain )
		{
			ZeroCochain out;
			out.cmesh=cmesh;
			out.VerticesMap=VerticesMap;

			if(chain.cmesh!=cmesh)
			{
				std::cout<<"error, substraction of cochains is only valid on the same mesh"<<std::endl;
			}
			for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
			{
				Vertex *v=it->first;
				out.VerticesMap[v]=VerticesMap[v]-chain.VerticesMap[v];

			}

			return out;
		}
			// returns "-f" where f  is a zerochain
			ZeroCochain  operator-( void )
		{
			
			
			ZeroCochain out;
			out.cmesh=cmesh;
			out.VerticesMap=VerticesMap;
			for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
			{
				Vertex *v=it->first;
				out.VerticesMap[v]=-VerticesMap[v];

			}

			return out;
		}

			//returns |f|_inf
			double infinitynorm()
			{
				double out=-99999999;

				for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
				{
					Vertex *v=it->first;
					if(VerticesMap[v]>out)
						out=VerticesMap[v];

				}

				return abs(out);
			}

			//returns max(f(x))
			double maxvalue(void)
			{
				double max=-99999999;
				for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
				{
					Vertex *v=it->first;
					if(VerticesMap[v]>max)
						max=VerticesMap[v];

				}
				return max;
			}


			//returns min(f(x))
				double minvalue(void)
			{
				double min=99999999;
				for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
				{
					Vertex *v=it->first;
					if(VerticesMap[v]<min)
						min=VerticesMap[v];

				}
				return min;;
			}
			//normalize zerocochain to a range [a,b]
			void normalize(double start, double end)
			{

				double max=maxvalue();
				double min=minvalue();


				for (std::map<Vertex*,double>::iterator it=VerticesMap.begin(); it!=VerticesMap.end(); ++it)
				{
					Vertex *v=it->first;
					VerticesMap[v]= ((VerticesMap[v]-min)/(max-min))*(end-start)+start;

				}

			}

	};
}



#endif