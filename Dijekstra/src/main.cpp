#include <cstdlib>
#include <GL/glut.h>
#include "Renderer.h"
#include <iostream>
#include<map>
#include <queue>


using namespace XMeshLib;

Renderer theRenderer;
std::map<Vertex*, double>renderingscalarmap;
Mesh *srcMesh = new Mesh;

std::map< Edge *, double> HEWeights;
std::map<Vertex*, double> HWeightSums;


std::map<Halfedge*, double> EWeights;
std::map<Vertex*, double> WeightSums;

static double multiplier = 1;
static double shift = 0;

//my own functions
double angeloftwovectors(Halfedge *he1, Halfedge *he2);
std::map<Halfedge*, std::vector<double>>  ComputeFloaterAngle(Mesh *cmesh);
void  SetFloaterWeights(Mesh *cmesh, std::map<Halfedge*, double> &FEWeights, std::map<Vertex*, double> &FWeightSums);



void ComputeCotanWeights(Mesh *cmesh, std::map<Edge*, double> &EWeights, std::map<Vertex*, double> &WeightSums);
std::map<Halfedge*, double> ComputeCornerAngle(Mesh *M);



std::map<Vertex*, double> HeatScalarFunction(Mesh *M, int iterations);



///////


void KeyBoardB(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'f':
		//Flat Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	case 's':
		//Smooth Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	case 'w':
		//Wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case '+':
		//Wireframe mode
		multiplier = multiplier + 0.1;
		break;
	case '-':
		//Wireframe mode
		multiplier = multiplier - 0.1;
		break;
	case 'a':
		//Wireframe mode
		shift = shift + 0.01;
		break;
	case 'z':
		//Wireframe mode
		shift = shift - 0.01;
		break;

	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void renderScene(void) 
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	theRenderer.UpdateEye();
	// begin drawing things here

	theRenderer.RenderMesh(renderingscalarmap, multiplier,shift);//this function draws the mesh into the screen

	

	//end drawing things here
	glutSwapBuffers();
	glPopMatrix();

}
void mouseMove(int x, int y){theRenderer.MouseMove(x,y);}
void mouseClick(int button , int state, int x, int y){theRenderer.MouseClick(button, state, x, y);}
void reshapeScene(GLsizei width, GLsizei height){theRenderer.ReshapeScene(width, height);}
//void keyBoard(unsigned char key, int x, int y){theRenderer.KeyBoard(key,x,y);} 
void keyBoard(unsigned char key, int x, int y){ KeyBoardB(key, x, y); }




int main(int argc, char **argv) {
	
	
	if(!srcMesh->read("eight.m")) {
		delete srcMesh;
		exit(-1);
	};

	SetFloaterWeights(srcMesh, EWeights, WeightSums);
	std::map<Vertex*, double> heatsc = HeatScalarFunction(srcMesh, 1000);

	renderingscalarmap = heatsc;
	

	
	//do not change this part 

	//openGL standard commands
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition(theRenderer.win_x,theRenderer.win_y);
	glutInitWindowSize(theRenderer.win_width, theRenderer.win_height);
	glutCreateWindow("Test");
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeScene);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);


	theRenderer.SetMesh(srcMesh);
	theRenderer.InitGL();
	///////////////////////////////
	theRenderer.LoadTexture("color_strip.bmp"); //this command loads a texture from the HD and maps it into the UV space 	

	glutMainLoop();

	delete srcMesh; srcMesh = NULL;

	return 0;
}



double angeloftwovectors(Halfedge *he1, Halfedge *he2){


	if (he1->source() != he2->source())
		std::cout << "this is not a correct computation " << std::endl;
	Point p = he1->source()->point();
	Point p1 = he1->target()->point();

	Point p2 = he2->target()->point();

	Point V1 = (p1 - p) / (p1 - p).norm();
	Point V2 = (p2 - p) / (p2 - p).norm();
	if (!(acos((V1*V2)) >= 0 && acos((V1*V2)) <= 3.1415926536))
	{
		std::cout << "p1 " << p1[0] << ", " << p1[1] << ", " << p1[2] << std::endl;

		std::cout << "p2 " << p2[0] << ", " << p2[1] << ", " << p2[2] << std::endl;
		std::cout << "p " << p[0] << ", " << p[1] << ", " << p[2] << std::endl;

		std::cout << "norm1 " << (p1 - p).norm() << std::endl;
		std::cout << "norm2 " << (p2 - p).norm() << std::endl;

		std::cout << "first v " << V1[0] << ", " << V1[1] << ", " << V1[2] << std::endl;
		std::cout << "second v " << V2[0] << ", " << V2[1] << ", " << V2[2] << std::endl;
		std::cout << "dot product " << V1*V2 << std::endl;
		system("pause");
	}

	return acos((V1*V2));
}



std::map<Halfedge*, std::vector<double>>  ComputeFloaterAngle(Mesh *cmesh)
{
	std::map<Halfedge*, std::vector<double>> FloaterAngles;
	std::cout << "Computing angles in order to use for Mean value coordinates...";
	for (MeshHalfedgeIterator heiter(cmesh); !heiter.end(); ++heiter)
	{

		Halfedge * he = *heiter;
		if (he->source()->boundary() == 1)
			continue;
		Halfedge * hePrevtwin = he->he_prev()->he_twin();


		std::vector<double>angles;

		double theta1 = angeloftwovectors(he, hePrevtwin);
		double theta2 = angeloftwovectors(he, he->he_twin()->he_next());


		angles.push_back(theta1);
		angles.push_back(theta2);

		//assert(theta1 >= 0 && theta1 <= 3.1415926536);
		//assert(theta2 >= 0 && theta2 <= 3.1415926536);
		if (!(theta1 >= 0 && theta1 <= 3.1415926536))
		{
			std::cout << "theta is " << theta1 << std::endl;
			std::cout << "The mesh you entered has a problem. The angels of this triangle is valid";
			system("pause");
		}
		if (!(theta2 >= 0 && theta2 <= 3.1415926536))
		{
			std::cout << "theta is  " << theta2 << std::endl;
			std::cout << "The mesh you entered has a problem. The angels of this triangle is valid";
			system("pause");
		}



		FloaterAngles[he] = angles;
	}

	std::cout << "Done!" << std::endl;
	return FloaterAngles;
}


void  SetFloaterWeights(Mesh *cmesh, std::map<Halfedge*, double> &FEWeights, std::map<Vertex*, double> &FWeightSums)
{
	std::map<Halfedge*, std::vector<double>> FloaterAngles=ComputeFloaterAngle(cmesh);
	std::cout << "Storing Mean Value coordinates...";
	for (MeshHalfedgeIterator eiter(cmesh); !eiter.end(); ++eiter)
	{
		Halfedge * e = *eiter;
		if (e->source()->boundary() == 1)
			continue;
		//std::vector<double> angles=FloaterAngles[e];
		double distance = (e->source()->point() - e->target()->point()).norm();

		double weight = (tan(FloaterAngles[e][0] / 2) + tan(FloaterAngles[e][1] / 2)) / distance;
		FEWeights[e] = weight;

	}
	for (MeshVertexIterator vit(cmesh); !vit.end(); ++vit)
	{
		Vertex * v = *vit;
		if (v->boundary())
			continue;
		double wsum = 0;
		for (VertexOutHalfedgeIterator veit(cmesh, v); !veit.end(); ++veit)
		{
			Halfedge * e = *veit;
			wsum += FEWeights[e];
		}
		FWeightSums[v] = wsum;
	}
	std::cout << "Done!" << std::endl;
}



std::map<Halfedge*,double> ComputeCornerAngle(Mesh *M)
{// every face has three corners, compute and store their angles on corresponding halfedges

	std::map<Halfedge*, double> HEAngles;
	std::cout << "Computing corner angles of the mesh...";
	for (MeshHalfedgeIterator heiter(M); !heiter.end(); ++heiter)
	{
		//the halfedge he points to an angle in the triangle with sides abc. We will compute the angle theta
		//that he points at.
		Halfedge * he = *heiter;
		Point &p0 = he->target()->point();
		Point &p1 = he->he_next()->target()->point();
		Point &p2 = he->source()->point();
		double a = (p1 - p0).norm();
		double b = (p0 - p2).norm();
		double c = (p2 - p1).norm();
		double costheta = (a*a + b*b - c*c) / (2 * a*b);
		double theta = acos(costheta);
		//assert(theta >= 0 && theta <= 3.1415928);

		//he points towards the angle theta which is basically the angle p2p0p1.
		HEAngles[he] = theta;
	}
	std::cout << "Done!" << std::endl;

	return HEAngles;
}


void ComputeCotanWeights(Mesh *cmesh, std::map<Edge*, double> &EWeights, std::map<Vertex*, double> &WeightSums)
{
	std::map<Halfedge*, double>  HEAngles=ComputeCornerAngle(cmesh);
	std::cout << "Storing Harmonic weights...";
	for (MeshEdgeIterator eiter(cmesh); !eiter.end(); ++eiter)
	{
		Edge * e = *eiter;
		if (e->boundary())
		{
		
		}
		else
		{
			Halfedge * he1 = e->he(0)->he_next();
			Halfedge * he2 = e->he(1)->he_next();
			double weight = (1 / tan(HEAngles[he1]) + 1 / tan(HEAngles[he2])) / 2;
			EWeights[e] = weight;
		}
	}
	for (MeshVertexIterator vit(cmesh); !vit.end(); ++vit)
	{
		Vertex * v = *vit;
		double wsum = 0;
		for (VertexEdgeIterator veit(v); !veit.end(); ++veit)
		{
			Edge * e = *veit;
			wsum += EWeights[e];
		}
		WeightSums[v] = wsum;
	}
	std::cout << "Done!" << std::endl;
}



std::map<Vertex*, double> HeatScalarFunction(Mesh *M, int iterations)
{

	std::map<Vertex*, double> heatfunction;

	MeshVertexIterator vit(M);
	Vertex *v = *vit;
	Vertex * highesty=v;
	Vertex * lowesty=v;
	//looking for the highest and the lowest vertices (in terms of the y coordinate of that vertex).These vertices will be called non-boundary vertices.
	for (; !vit.end(); ++vit)
	{
		Vertex *V = *vit;
		if (highesty->point()[1] < V->point()[1])
			highesty = V;
		if (lowesty->point()[1] > V->point()[1])
			lowesty = V;
	}


	//setting the highest and the lowest vertices to certain scalar values:
	heatfunction[highesty] = 1;
	heatfunction[lowesty] = 0;
		
	//setting every other vertex scalar value to zero
	for (MeshVertexIterator vit(M); !vit.end(); ++vit)
	{
		Vertex *V = *vit;
		if (V != highesty && V != lowesty)
		{
			heatfunction[V] = 0;
		}
	}

	//average the values around each non-boundary vertex till convegence 

	for (int j = 0; j < iterations; j++)
	{
		for (MeshVertexIterator vit(M); !vit.end(); ++vit)
		{

			Vertex *V = *vit;
			if (V != highesty && V != lowesty)
			{


				double heat = 0;
				for (VertexOutHalfedgeIterator heit(M,V); !heit.end(); ++heit)
				{
					Halfedge *he = *heit;

					Vertex *ver = he->target();
					double wij =double( EWeights[he] / WeightSums[V]);
					heat = heat + heatfunction[ver]*wij;
				}
				heatfunction[V] = heat;
			}
		}


	}

	
	

		return heatfunction;


}


