#ifndef X_TRAITPARSER_H
#define X_TRAITPARSER_H

#include "Parser.h"
#include "Mesh.h"
#include "Iterators.h"

namespace XMeshLib
{

class TraitParser
{
public:
	/*
		static method: ReadTrait, parse the "keystr" in "parsestr", the result will be return by values[0..dim-1]
			parsestr: the string to be parsed
			keystr:	the trait name
			dim: dimention of the key
			values: store the parsing results
	*/
	static bool ReadTrait(std::string &parsestr, 
		const char * keystr, const int dim, double * values);

	static bool ReadTrait(std::string &parsestr, 
		std::string &keystr, const int dim, double * values);

	static bool ReadIntTrait(std::string &parsestr, const std::string &keystr, 
		const int dim, int * values);

	static bool RemoveTrait(std::string &parsestr, std::string &keystr);

	static bool UpdateTrait(std::string &parsestr, 
		std::string &keystr, 
		const int dim, 
		double * values);

	static bool UpdateIntTrait(std::string &parsestr, 
		std::string &keystr, 
		const int dim, 
		int * values);
	
	static bool CopyTraits(Mesh * mesh1, Mesh * mesh2, 
		const char * keystr, int dim);
	
};
}

#endif

