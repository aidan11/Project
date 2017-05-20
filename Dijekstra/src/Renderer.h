#ifndef _RENDERER_H_
#define _RENDERER_H_
#include <cstdlib>
#include <GL/glut.h>
#include "Core_Mesh/Mesh.h"
#include "Core_Mesh/Loop.h"
#include "BMP/BmpReader.h"
#include "BMP/Texture.h"
#include <map>

//class Morphing;
namespace XMeshLib
{
	class Renderer
	{

	public:
		Renderer();
		~Renderer();

		void SetMesh(Mesh * inmesh);
		void SetupEye();
		void UpdateEye();
		void InitGL();
		void PrintUsage();
		void LoadTexture(const char file[100]);

		void ReshapeScene(int w, int h);

		void RenderAll(/*std::vector<Loop> & boundaryLoops*/);
		void RenderMesh(std::map <Vertex*,double> Morse, double multiplier,double shift );
		void RenderEdges(std::vector<Halfedge *> & helist);
		void RenderTexture();
		void ComputeMassCenterAndBoundingBox();
		float ComputeNormalAndArea(Face * f, float norm[3]);
		void ComputeNormal(Vertex * ver, float norm[3]);

		void MouseMove(int x, int y);
		void MouseClick(int button , int state, int x, int y);
		void KeyBoard(unsigned char key, int x, int y);
		


	public:
		float EyePosition[3];
		float DltEye[3];
		float ObjCenter[3];
		float BoxMin[3];
		float BoxMax[3];
		int win_width, win_height;
		int win_x, win_y;
		float whratio;
		int mouseX, mouseY;
		Mesh * cmesh;
		stdext::hash_map<Vertex *, double> * texU;
		stdext::hash_map<Vertex *, double> * texV;
		bool needUpdateEye;
		float rot_x, rot_y;
		int gButton;
		bool showTexture;

		Texture * ctexture;
		//Morphing *morphing;
		//double morphStep;

	
		
	};
}
#endif
