#ifndef _XMESHLIB_TRAIT_H_
#define _XMESHLIB_TRAIT_H_

#pragma warning (disable : 4786)
#pragma warning (disable : 4018)
#include <list>
#include <vector>
#include <algorithm>

namespace XMeshLib
{


#define ID_AdjHeTrait	0000

class TraitNode
{
public:
	TraitNode(){tId = -1;}
	virtual ~TraitNode(){tId = -1;}
	bool operator== (const TraitNode & t) const
	{
		int tid1 = t.tId;			
		if (tId == tid1)
			return true;
		else
			return false;			
	}
	int tId;
};	

class TraitList
{
public:
	TraitList(){;}
	~TraitList(){clear();}
	void clear()
	{
		for (std::list<TraitNode *>::iterator titer=tlist.begin(); titer!=tlist.end(); ++titer)
		{				
			TraitNode * tempTrait = * titer;				
			delete tempTrait;
		}
		tlist.clear();
	}
	void add(TraitNode * t)
	{
		tlist.push_back(t);
	}
	bool del(int id)
	{
		std::list<TraitNode *>::iterator titer;
		for (titer=tlist.begin(); titer!=tlist.end(); ++titer)
		{				
			TraitNode * tempTrait = * titer;
			if (id == tempTrait->tId)
			{
				tlist.erase(titer);
				delete tempTrait;
				return true;
			}
		}
		return false;
	}
	TraitNode * get(int id)
	{
		std::list<TraitNode *>::iterator titer;
		for (titer=tlist.begin(); titer!=tlist.end(); ++titer)
		{				
			TraitNode * tempTrait = * titer;
			if (id == tempTrait->tId)
				return tempTrait;
		}
		return NULL;
	}
	std::list<TraitNode *> tlist;		
};


class Halfedge;
class AdjHeTrait:public TraitNode
{
public:
	AdjHeTrait()
	{
		tId = ID_AdjHeTrait;
	}
	~AdjHeTrait()
	{
		;
	}		
	void AddHE(Halfedge * he)
	{
		adjhelist.push_back(he);
	}
	int NumAdjHE()
	{
		return adjhelist.size();
	}
	Halfedge * indAdjHe(int i)
	{
		if (i<0 || i>= adjhelist.size())
			return NULL;
		return adjhelist[i];
	}
public:
	std::vector<Halfedge *> adjhelist;
};

}
#endif

