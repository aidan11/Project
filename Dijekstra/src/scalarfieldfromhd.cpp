
#include "scalarfieldfromhd.h"


using namespace XMeshLib;


bool scalarfieldfromhd::sread( const char *filename,std::map<Vertex*,double>& map1)
{	
	std::ifstream input(filename);
	if (!input)
	{
		std::cout << "The file " << filename << " does not exist in the working directory" << "\n";
		exit(0);
	}

	printf("Reading %s ...", filename);
	while(!input.eof())
	{
		std::string line;

		std::getline(input, line);

		std::stringstream ss(line);

		int id;
		double value;
		std::string temp;

		ss >> id >> value;
	
		//mapping the scalar values to the maps scalar map and the input map "map1"
		scalarmap[idtover[id]]=value;
		map1[idtover[id]]=value;

	}

	input.close();
	
	printf("Done!\n");
	return true;
}


bool scalarfieldfromhd::read2(const char *filename,std::map<Vertex*,double> & funcvalue) {
	std::cout<<"Loading function from HD..."<<std::endl;
	std::ifstream input (filename);
	while(input.good()) {
		std::string line;
		getline(input, line);
		if (line.empty()) {
			continue;
		}
		std::stringstream ss(line);
		std::string title;
		int id = -1;
		double value = 0;
		
		ss >> id >> value;
		//TODO: some check here, like id validation, duplication validation, etc.
		funcvalue[cmesh->idVertex(id)] = value;
	}
	input.close();
	std::cout<<"Done!"<<std::endl;

	return 1;
}


bool scalarfieldfromhd::swrite( const char outputFile[],std::map<Vertex*,double> map )
{
	FILE * fp = fopen( outputFile,"w");
	if ( !fp )
	{
		printf("Cannot open file %s to write\n", outputFile);
		return false;
	}
	
	
	std::ostringstream oss;
	oss.precision(20);
	oss.setf(std::ios::fixed,std::ios::floatfield);
	oss<<map.size();
	fprintf(fp, "%s ", oss.str().c_str());
	fprintf(fp, "\n");

	printf("Writing scalar values to HD %s ...", outputFile);
	
	for(MeshVertexIterator fit(cmesh); !fit.end(); ++fit)
	{
		Vertex * ver = *fit;
		std::ostringstream oss;
		oss.precision(20);
		oss.setf(std::ios::fixed,std::ios::floatfield);
		oss <<ver->id()<<" "<< map[ver];
		fprintf(fp, "%s ", oss.str().c_str());
		fprintf(fp, "\n");
	}

	
	fclose(fp);
	printf("Done!\n");
	return true;
}

bool scalarfieldfromhd::writevalues(const char outFile[], std::vector<double>&vec )
{
	FILE * fp = fopen( outFile,"w");
	if ( !fp )
	{
		printf("Cannot open file %s to write\n", outFile);
		return false;
	}
	

	printf("Writing cutting values %s ...", outFile);
	for(int i=0;i<vec.size();i++)
	{
		double regularV=vec[i];
		std::ostringstream oss;
		oss.precision(20);
		oss.setf(std::ios::fixed,std::ios::floatfield);
		oss << vec[i];
		fprintf(fp, "%s ", oss.str().c_str());
		fprintf(fp, "\n");
	}

	fclose(fp);
	printf("Done!\n");
	return true;
}


