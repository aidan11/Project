#include "Renderer.h"
#include "Core_Mesh/Iterators.h"


using namespace XMeshLib;

Renderer::Renderer()
{
	EyePosition[0]=EyePosition[1]=EyePosition[2]=0;
	DltEye[0]=DltEye[1]=DltEye[2]=0;
	ObjCenter[0]=ObjCenter[1]=ObjCenter[2]=0;
	BoxMin[0]=BoxMin[1]=BoxMin[2]=-1;
	BoxMax[0]=BoxMax[1]=BoxMax[2]=1;
	win_width=640;
	win_height=480;
	win_x = 100;
	win_y = 100;
	mouseX = 0;
	mouseY = 0;
	whratio = (float)win_width /(float)win_height;
	needUpdateEye = false;
	rot_x=rot_y =0;
	gButton=0;
	showTexture=false;
	texU = texV = NULL;
	ctexture = NULL;

}

Renderer::~Renderer()
{
	if (ctexture)
		delete ctexture;
}

void Renderer::RenderEdges(std::vector<Halfedge *> & helist){
	
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 0);
	glLineWidth(3.0);
	glBegin(GL_LINES);
	for (int i=0;i<helist.size();++i)
	{
		Halfedge * he = helist[i];
		Vertex * v[2];
		v[0] = he->source();
		v[1] = he->target();
		double x[2],y[2],z[2];
		for (int i=0;i<2;++i)
		{
			x[i]=v[i]->point()[0];
			y[i]=v[i]->point()[1];
			z[i]=v[i]->point()[2];
		}
		glVertex3f(x[0],y[0],z[0]);
		glVertex3f(x[1],y[1],z[1]);
	}
	glEnd();
	glColor3f(1, 1, 1);
	glEnable(GL_LIGHTING);

}

void Renderer::ComputeMassCenterAndBoundingBox()
{
	ObjCenter[0]=ObjCenter[1]=ObjCenter[2]=0;
	BoxMin[0]=BoxMin[1]=BoxMin[2]=1e8;
	BoxMax[0]=BoxMax[1]=BoxMax[2]=-1e8;
	if (cmesh==NULL)
		return;
	for (MeshVertexIterator vit(cmesh); !vit.end(); ++vit)
	{
		Vertex * v = *vit;
		for (int j=0;j<3;++j)
		{
			float value = v->point()[j];
			ObjCenter[j]+= value;
			if (BoxMax[j] < value)
				BoxMax[j] = value;
			if (BoxMin[j] > value)
				BoxMin[j] = value;
		}
	}
	ObjCenter[0]/=cmesh->numVertices();
	ObjCenter[1]/=cmesh->numVertices();
	ObjCenter[2]/=cmesh->numVertices();
	needUpdateEye = true;
}

float Renderer::ComputeNormalAndArea(Face * f, float norm[3])
{
	Halfedge * he = f->he();
	Halfedge * nhe = he->he_next();
	Vertex * v[3];
	v[0] = he->source();
	v[1] = he->target();
	v[2] = he->he_next()->target();
	double x[3],y[3],z[3];
	for (int i=0;i<3;++i)
	{
		x[i]=v[i]->point()[0];
		y[i]=v[i]->point()[1];
		z[i]=v[i]->point()[2];
	}
	double p1[3], p2[3];
	p1[0]=x[1]-x[0];p1[1]=y[1]-y[0];p1[2]=z[1]-z[0];
	p2[0]=x[2]-x[1];p2[1]=y[2]-y[1];p2[2]=z[2]-z[1];
	norm[0]= p1[1]*p2[2]-p1[2]*p2[1];
	norm[1]= p1[2]*p2[0]-p1[0]*p2[2];
	norm[2]= p1[0]*p2[1]-p1[1]*p2[0];
	return (norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);
}

void Renderer::ComputeNormal(Vertex * ver, float norm[3])
{
	//this function dynamically computes normal on each vertex
	//a more efficient way is to compute all face normals and areas and store them
	//then only compute weighted average here, without recalculating face normals and areas
	float fnorm[3];
	norm[0]=norm[1]=norm[2]=0;
	float totalArea=0;
	for (VertexFaceIterator vfit(ver); !vfit.end();++vfit)
	{
		Face * f= *vfit;
		float area_f = ComputeNormalAndArea(f, fnorm);
		norm[0] += fnorm[0]*area_f;
		norm[1] += fnorm[1]*area_f;
		norm[2] += fnorm[2]*area_f;
		totalArea += area_f;
	}
	norm[0]/= totalArea;
	norm[1]/= totalArea;
	norm[2]/= totalArea;
}


void Renderer::SetMesh(Mesh * inmesh){
	cmesh = inmesh;
	ComputeMassCenterAndBoundingBox();
}

void Renderer::SetupEye(void)
{
	float maxZ=BoxMax[2];
	float cenX=ObjCenter[0];
	float cenY=ObjCenter[1];
	float diagLen=sqrt((BoxMax[2]-BoxMin[2])*(BoxMax[2]-BoxMin[2])
		+(BoxMax[1]-BoxMin[1])*(BoxMax[1]-BoxMin[1])
		+(BoxMax[0]-BoxMin[0])*(BoxMax[0]-BoxMin[0]));
	EyePosition[0]=cenX; EyePosition[1]=cenY;EyePosition[2]=maxZ+diagLen*2;
	glTranslatef(-EyePosition[0], -EyePosition[1], -EyePosition[2]);  
	needUpdateEye = false;
}

void Renderer::UpdateEye(void)
{
	glTranslatef(ObjCenter[0], ObjCenter[1], ObjCenter[2]);
	glTranslatef(-DltEye[0], -DltEye[1], -DltEye[2]);  
	glRotatef(rot_x, 1.0, 0.0, 0.0);
	glRotatef(rot_y, 0.0, 1.0, 0.0);
	glTranslatef(-ObjCenter[0], -ObjCenter[1], -ObjCenter[2]);
	needUpdateEye=false;
}

void Renderer::InitGL(){

	GLfloat lightOneColor[] = {1, 1, 1, 1};
	GLfloat globalAmb[] = {.1, .1, .1, 1};
	GLfloat lightOnePosition[] = {1, 1, 1, 0.0};

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);      
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,0);
	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);

	glEnable (GL_TEXTURE_2D);

	glLoadIdentity();
	SetupEye();
}

void Renderer::RenderAll(/*std::vector<Loop> & boundaryLoops*/)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//if (showTexture)
	//{
	//	glLoadIdentity();
		//if (needUpdateEye)
			UpdateEye();
		//RenderTexture();
		//RenderMesh();
		glutSwapBuffers();
	//}
	//else
	//{
		
	//	if (needUpdateEye)
	//		UpdateEye();
	//	RenderMesh();
	//	glutSwapBuffers();
//	}
	glPopMatrix();
}




void Renderer::RenderMesh(std::map <Vertex*, double> scalarmap, double multiplier, double shift)
{
	
	glBegin(GL_TRIANGLES);
	for (MeshFaceIterator fit(cmesh); !fit.end();++fit)
	{
		Face * f = *fit;
		float norm[3];
		Halfedge * he = f->he();
		Vertex * v[3];
		v[0]=he->source();
		v[1]=he->target();
		v[2]=he->he_next()->target();
		for (int j=0;j<3;++j)
		{
			ComputeNormal(v[j],norm);
			glNormal3fv(norm);
			double x = v[j]->point()[0];
			double y = v[j]->point()[1];
			double z = v[j]->point()[2];

			glTexCoord2f(multiplier*scalarmap[v[j]]+shift, 0.1);
			glVertex3f(x,y,z);
		}
	}
	glEnd();
	
}

void Renderer::MouseMove(int x, int y)
{
	needUpdateEye=true;
	/* rotation*/
	if (gButton == GLUT_LEFT_BUTTON ) 
	{
		////////////added////////////////
		rot_y += (x - mouseX);
		rot_x += (y - mouseY);
		////////////////////////////////
		glutPostRedisplay();
	}

	/*xy translation */
	if (gButton == GLUT_MIDDLE_BUTTON) 
	{
		////////////added////////////////
		GLfloat scale_y = 10.f / win_height;
		GLfloat scale_x = 10.f / win_width;
		DltEye[0] -= scale_x * ( x - mouseX  ); //ObjTranslation[0] += scale_x * ( x - mouseX  );
		DltEye[1] -= scale_y * ( mouseY - y); //ObjTranslation[1] += scale_y * ( mouseY - y);
		glutPostRedisplay();
	}

	/* zoom in and out */
	if (gButton == GLUT_RIGHT_BUTTON ) 
	{
		double scale = 10./win_height;
		DltEye[2] += scale*(mouseY-y);//ObjTranslation[2] -= scale*(mouseY-y);
		glutPostRedisplay();
	}

	mouseX = x;
	mouseY = y;
}

void Renderer::MouseClick(int button , int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{ 
		gButton = GLUT_LEFT_BUTTON;  
		mouseX = x;
		mouseY = y;
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		gButton = GLUT_MIDDLE_BUTTON;
		mouseX = x;
		mouseY = y;      
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		gButton = GLUT_RIGHT_BUTTON;
		mouseX = x;
		mouseY = y;
	}
	return ;
}

void Renderer::ReshapeScene(int width, int height)
{
	win_width = width;
	win_height = height;

	float mwidth = (width<height*whratio)?width:(height*whratio);

	glViewport(0,0,mwidth, (float)mwidth/whratio);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 

	gluPerspective( 40.0, /* field of view in degrees */
		whratio, /* aspect ratio */
		0.01, /* Z near */ 
		9000.0 /* Z far */);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

#include <iostream>

void Renderer::KeyBoard(unsigned char key, int x, int y) 
{	
	
	switch( key )
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
	case 't':
		showTexture = !showTexture;
		break;

	case '?':
		//usage
		PrintUsage();
		break;  
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();  
}

void Renderer::PrintUsage(void)
{
	printf("This program is the first homework from XXX \r\n");
	printf("The input is a .m triangle mesh. \r\n");
	printf(" f - flat shading\r\n");
	printf(" s - smooth shading\r\n");
	printf(" w - wireframe\r\n");
	printf(" leftarrow, rightarrow - morphing\r\n");
	printf(" key ESC - exit\r\n");
}

void Renderer::LoadTexture(const char file[])
{
	HBITMAP bmp = BmpReader::loadBMP(file);
	if (ctexture)
		delete ctexture;
	ctexture = new Texture(bmp);
	ctexture->bind();
}

void Renderer::RenderTexture()
{
	////glBindTexture(GL_TEXTURE_2D, 13);
	////glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	////glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	////glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	////glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	////glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);


	//if (BitmapInfo)
	//{
	//	float xsize = win_width;
	//	float ysize = BitmapInfo->bmiHeader.biHeight * xsize / BitmapInfo->bmiHeader.biWidth;
	//	if (ysize > win_height)
	//	{
	//		ysize = win_height;
	//		xsize = BitmapInfo->bmiHeader.biWidth * ysize /
	//			BitmapInfo->bmiHeader.biHeight;
	//	}

	//	float xscale  = xsize / BitmapInfo->bmiHeader.biWidth;
	//	float yscale  = ysize / BitmapInfo->bmiHeader.biHeight;

	//	float xoffset = (win_width - xsize) * 0.5;
	//	float yoffset = (win_height - ysize) * 0.5;

	//	glRasterPos2f(xoffset, yoffset);
	//	glPixelZoom(xscale, yscale);

	//	glDrawPixels(BitmapInfo->bmiHeader.biWidth,
	//		BitmapInfo->bmiHeader.biHeight,
	//		GL_BGR_EXT, GL_UNSIGNED_BYTE, BitmapBits);
	//}
}