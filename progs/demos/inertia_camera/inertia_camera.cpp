#if 0

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "camera.hpp"
#include "car.hpp"
#include "envrnmnt.hpp"

void draw_simple()
{
	float ratio;
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
}

// Moving object in the virtual scene **********************
Car car(0.0, 0.0, -5.0);

// Environment ************************
Environment environment;

// Screen parameters
int wWidth = 1024;
int wHeight = 768;

GLuint	texture[1];

// Cameras *****************************
Camera camera(-5.0, 5.0, -20.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0, 28.0);
Camera camera2(-3.0, 10.0, 20.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0, 28.0);
bool directFollow = false;                        // In case of "direct follow" we don't take the human behavior into account
// end camera ******************

void drawInertiaCameraScene(Camera& cam)
{
	cam.DrawCarScene(car, environment, directFollow, wWidth, wHeight, texture[0]);
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(wWidth, wHeight, "inertia camera", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	do{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear( GL_COLOR_BUFFER_BIT );

		// Draw nothing, see you in tutorial 2 !
		//draw_simple();

		// drawInertiaCameraScene
		drawInertiaCameraScene(camera);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
#else

#include <GL/glut.h>
#include <stdlib.h>

#include "camera.hpp"
#include "car.hpp"
#include "envrnmnt.hpp"

// Moving object in the virtual scene **********************
Car car(0.0, 0.0, -5.0);

// Environment ************************
Environment environment;

// Screen parameters
int wWidth = 1024;
int wHeight = 768;

GLuint	texture[1];

// Cameras *****************************
Camera camera(-5.0, 5.0, -20.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0, 28.0);
Camera camera2(-3.0, 10.0, 20.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0, 28.0);
bool directFollow = false;                        // In case of "direct follow" we don't take the human behavior into account
// end camera ******************

void drawInertiaCameraScene(Camera& cam)
{
	cam.DrawCarScene(car, environment, directFollow, wWidth, wHeight, texture[0]);
}

void init(void)
{
#if 0
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
#else
	//LoadGLTexture(0, "asphalt.tga");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      // Black background
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_CULL_FACE);						// Enable back-face culling (to enhance the performance)
#endif

}

void display(void)
{
#if 0
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glScalef(1.0, 2.0, 1.0);      /* modeling transformation */
	glutWireCube(1.0);

	glutSwapBuffers();

	//glFlush();
#else
	drawInertiaCameraScene(camera);
	glutSwapBuffers();
#endif
}

void reshape(int width, int height)
{
#if 0
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
#else
	if (height == 0)
	{
		height = 1;
	}
	wWidth = width;
	wHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(28.0f, (GLfloat)wWidth / (GLfloat)wHeight, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(wWidth, wHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
#endif
