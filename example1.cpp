// In this version, idle function increments angles
// which are sent to vertex shader where rotation takes place
#include "off_io.h"
#include "Angel.h"




typedef Angel::vec4 color4;
typedef Angel::vec4 point4;
LoadData dragon;
graphicData graphicdragon;

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 40.0f, beta = 45.0f;
float r = 5.25f;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices / triangle)
point4 points[NumVertices];
color4 colors[NumVertices];
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices1[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};
// RGBA colors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0), // black
	color4(1.0, 0.0, 0.0, 1.0), // red
	color4(1.0, 1.0, 0.0, 1.0), // yellow
	color4(0.0, 1.0, 0.0, 1.0), // green
	color4(0.0, 0.0, 1.0, 1.0), // blue
	color4(1.0, 0.0, 1.0, 1.0), // magenta
	color4(1.0, 1.0, 1.0, 1.0), // white
	color4(0.0, 1.0, 1.0, 1.0) // cyan
};
// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Yaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

GLuint theta; // The location of the "theta" shader uniform variable
//----------------------------------------------------------------------
// quad generates two triangles for each face and assigns colors
// to the vertices
GLuint lProjection;
GLuint lModel;
GLuint lview;
GLuint llposition;
int Index = 0;
glm::mat4 projection;
glm::mat4 model;
glm::mat4 view;
glm::vec3 lightdirection;
glm::vec3 eye=glm::vec3(0,0,-5); 
void
quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

GLuint vao;

//----------------------------------------------------------------------
// generate 12 triangles: 36 vertices and 36 colors
void
colorcube(void)
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}




//----------------------------------------------------------------------
// OpenGL initialization
void
init(void)
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	//camX=0;
    //camY=0;
	//camZ=-10;

	//ifstream drag("dragon.off",std::ifstream::in);
	//ifstream drag("klingon.off",std::ifstream::in);
	//ifstream drag("goblet.off",std::ifstream::in);
	ifstream drag("sphere2.off",std::ifstream::in);

	off_io modelloader;
	
	if(modelloader.Load(drag,&dragon))
	{
		std::cout<<"success";
	}
	else
		std::cout<<"unsuccess";

	graphicdragon.nVertices=dragon.verts.size();
	graphicdragon.pVertices=new GLfloat[graphicdragon.nVertices*3];

	for(int i=0;i<graphicdragon.nVertices;i++)
	{
		for(int j=0;j<3;j++)
		{
			graphicdragon.pVertices[3*i+j]=dragon.verts[i][j];
		}
	}
	std::cout<<graphicdragon.pVertices[1];
	graphicdragon.nTris=dragon.tris.size();
	graphicdragon.pIndices=new GLuint[graphicdragon.nTris*3];

	for(int i=0;i<graphicdragon.nTris;i++)
	{
		for(int j=0;j<3;j++)
		{
			graphicdragon.pIndices[3*i+j]=dragon.tris[i][j];
		}
	}
	std::cout<<graphicdragon.pIndices[1];
	graphicdragon.pNormals=new GLfloat[graphicdragon.nVertices*3];

	for(int i=0;i<graphicdragon.nVertices;i++)
	{
		for(int j=0;j<3;j++)
		{
			graphicdragon.pNormals[3*i+j]=dragon.vertnormal[i][j];
		}
	}


	//colorcube();
	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// Create a vertex array object
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* graphicdragon.nVertices*3,graphicdragon.pVertices, GL_STATIC_DRAW);
	GLuint buffer2;
    GLuint buffer3;
	glGenBuffers(1, &buffer2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* graphicdragon.nTris*3,graphicdragon.pIndices, GL_STATIC_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),sizeof(colors), colors);
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,3 * sizeof(GLfloat),(GLvoid*)0);
	glGenBuffers(1, &buffer3);
	glBindBuffer(GL_ARRAY_BUFFER, buffer3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* graphicdragon.nVertices*3,graphicdragon.pNormals, GL_STATIC_DRAW);
	
	GLuint vNormal = glGetAttribLocation(program, "vnormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE,3 * sizeof(GLfloat),(GLvoid*)0);
	glBindVertexArray(0);
	lightdirection=glm::vec3(10,3,1);


	/*GLuint vNormal = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));*/
	theta = glGetUniformLocation(program, "theta");
	lProjection=glGetUniformLocation(program,"projection");
	lModel=glGetUniformLocation(program,"model");
	lview=glGetUniformLocation(program,"view");
	llposition=glGetUniformLocation(program,"ldir");
	glUniform3fv(llposition,1,glm::value_ptr(lightdirection));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
//----------------------------------------------------------------------
void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glMatrixMode(GL_MODELVIEW);
	
    glUniform3fv(theta, 1, Theta);
	//glMatrixMode(GL_PROJECTION);
	projection=glm::perspective(glm::radians(90.0f),(float)640 / (float)480,0.1f,1000.0f);
	//glGetFloatv(GL_PROJECTION_MATRIX,glm::value_ptr( projection));
	glUniformMatrix4fv(lProjection,1,GL_FALSE,glm::value_ptr( projection));

	//glMatrixMode(GL_MODELVIEW);
	//glGetFloatv(GL_MODELVIEW_MATRIX,glm::value_ptr( modelview));

	model=glm::mat4(1);
	glUniformMatrix4fv(lModel,1,GL_FALSE,glm::value_ptr(model));

	view=glm::lookAt(glm::vec3(camX,camY,camZ),glm::vec3(0,0,0),glm::vec3(0,1,0));
	glUniformMatrix4fv(lview,1,GL_FALSE,glm::value_ptr(view));
	
	

	


	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES,graphicdragon.nTris*3,GL_UNSIGNED_INT,(void*)0);
	glBindVertexArray(0);
	glutSwapBuffers();
}
//----------------------------------------------------------------------
void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}
//----------------------------------------------------------------------
void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON: Axis = Xaxis; break;
		case GLUT_MIDDLE_BUTTON: Axis = Yaxis; break;
		case GLUT_RIGHT_BUTTON: Axis = Zaxis; break;
		}
	}
}
//----------------------------------------------------------------------
void
idle(void)
{
	Theta[Axis] += 0.1;
	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}
//	glutPostRedisplay();
}
void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}



	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	glutPostRedisplay();
}
//----------------------------------------------------------------------
int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Color Cube");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	//glutMouseFunc(mouse);
	//glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}