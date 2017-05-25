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
///////////


//my own functions

double vertextomeshdistance(std::vector<Vertex*> submesh, Vertex * v);
Vertex * meshdistance(Mesh *mesh, std::vector<Vertex*>  submesh);
std::vector<Vertex*> landmark(Mesh * mesh, int n);




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





double vertextomeshdistance(std::vector<Vertex*> submesh, Vertex * v)
{
	double d = -1;
	for (int i = 0; i < submesh.size(); i++)
	{
		double d1 = (submesh[i]->point() - v->point()).norm();
		if (d1 < d)
		{
			d = d1;
		}
	}
	return d;
}

Vertex * meshdistance(Mesh *mesh, std::vector<Vertex*>  submesh)
{
	Vertex *outV = submesh[0];
	double d = -INFINITE;
	for (MeshVertexIterator vvit(mesh); !vvit.end(); ++vvit)
	{
		Vertex *v = *vvit;
		double d1 = vertextomeshdistance(submesh, v);
		if (d1 == 0)
			continue;
		else
		{
			if (d1 > d)
			{
				outV = v;
				d = d1;
			}
		}


		return outV;

	}



}

std::vector<Vertex*> landmark(Mesh * mesh, int n)
{
	std::vector < Vertex* > out;
	MeshVertexIterator viter(mesh);

	Vertex * v = *viter;

	out.push_back(v);

	for (int i = 0; i < n; i++)

	{
		Vertex * v1 = meshdistance(mesh, out);

		out.push_back(v1);
	}
	return out;
}