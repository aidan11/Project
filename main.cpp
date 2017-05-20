

#include "shortestpath.h"
#include <vector>
#include <string>
#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <sstream>



using namespace XMeshLib;
using namespace std;


bool write_DJdistancematrix_tohd_1(Mesh* cmesh, const char outFile[])
{
	FILE * fp = fopen(outFile, "w");
	if (!fp)
	{
		printf("Cannot open file %s to write\n", outFile);
		return false;
	}

	std::vector<Vertex*> allver;
	for (MeshVertexIterator fit(cmesh); !fit.end(); ++fit)
	{
		allver.push_back(*fit);
	}
	int k = 0;

	shortestpath * Shortpath = new shortestpath;
	Shortpath->cmesh = cmesh;
	Shortpath->SetEdgesWeight();

	for (int i = 0; i < allver.size(); i++)
	{
		Vertex* ver = allver[i];
		Shortpath->source = ver;

		Shortpath->getgeodesicfunction();
		for (int j = 0; j < allver.size(); j++)
		{
			Vertex* ver1 = allver[j];
			double val=Shortpath->D[ver1];
			std::ostringstream oss;
			oss.precision(5);
			oss.setf(std::ios::fixed, std::ios::floatfield);
			if (j == allver.size() - 1)
				oss << val;
			else
				oss << val << " ";


			fprintf(fp, "%s ", oss.str().c_str());


		}
		if (i == allver.size() - 1)
			continue;
		else
			fprintf(fp, "\n");
	}

	std::cout << "Done DDM!";
	fclose(fp);
	return 1;
}

bool write_Edistancematrix_tohd_1(Mesh* cmesh, const char outFile[])
{
	FILE * fp = fopen(outFile, "w");
	if (!fp)
	{
		printf("Cannot open file %s to write\n", outFile);
		return false;
	}

	std::vector<Vertex*> allver;
	for (MeshVertexIterator fit(cmesh); !fit.end(); ++fit)
	{
		allver.push_back(*fit);
	}
	

	for (int i = 0; i < allver.size(); i++)
	{
		Vertex* ver = allver[i];
		Point p = ver->point();

		for (int j = 0; j < allver.size(); j++)
		{
			Vertex* ver1 = allver[j];
			Point p1 = ver1->point();
			double val = (p - p1).norm();
			std::ostringstream oss;
			oss.precision(5);
			oss.setf(std::ios::fixed, std::ios::floatfield);
			if (j == allver.size() - 1)
				oss << val;
			else
				oss << val << " ";


			fprintf(fp, "%s ", oss.str().c_str());


		}
		if (i == allver.size() - 1)
			continue;
		else
			fprintf(fp, "\n");
	}

	std::cout << "Done EDM!";
	fclose(fp);
	return 1;
}



int getdir(string dir, vector<string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}

int main()
{

	string absolute_path = "C:/Users/Aidan/Documents/Visual Studio 2015/Projects/geo/"; //PATH SPECIFIC

	string dir = string(absolute_path + "Meshs");   //PATH SPECIFIC
	vector<string> files = vector<string>();

	getdir(dir, files);

	for (int f = 2; f < files.size(); f++)
	{

		Mesh *srcMesh = new Mesh;

	
		string full_file_in = absolute_path + "Meshs/" + files[f];   //PATH SPECIFIC

		
		const char * file_in = full_file_in.c_str();

		if (!srcMesh->read(file_in))
		{
			delete srcMesh;
			exit(-1);
		}




		for (int i = 0; i < 2; i++)
		{
			files[f].pop_back();
		}

		

		string full_file_out_edm = absolute_path + "Distance Matrices/EDMS/" + files[f] + "_EDM.txt";  //PATH SPECIFIC
		string full_file_out_ddm = absolute_path + "Distance Matrices/DDMS/" + files[f] + "_DDM.txt";  //PATH SPECIFIC

		

		const char * file_out_edm = full_file_out_edm.c_str();
		const char * file_out_ddm = full_file_out_ddm.c_str();

		write_DJdistancematrix_tohd_1(srcMesh, file_out_ddm);
		write_Edistancematrix_tohd_1(srcMesh, file_out_edm);

		
		srcMesh->clear();

	}
	return 0;
}

