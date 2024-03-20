
#include <GL/glut.h>
#include <stdlib.h>

#include "camera.hpp"
#include "car.hpp"
#include "envrnmnt.hpp"
#include "vector.hpp"

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
int curCamera = 0;
bool directFollow = false;                        // In case of "direct follow" we don't take the human behavior into account
// end camera ******************

Camera& GetCamera()
{
	return (curCamera == 0 ? camera : camera2);
}

void InitCameras()
{
	camera.setScreenMessage(0,
		"ShakeCam v0.1 - Keyboard Layout:\n\t"
		"[F1] - toggle this help screen ON/OFF\n\t"
		"[F2] - toggle full screen mode\n\t"
		"[UP] [END] [LEFT] [RIGHT] - accelerate ahead, break, steer left, steer right the car\n\t"
		"[DOWN] - accelerate backwards\n\t"
		"[SPACE] - hand break (sudden stop)\n\t"
		"[1]...[7] - show/hide screen message 1...7 (#7 is the DEBUG-graph by default)\n\t"
		"[R] - Reset scene\n\t"
		"[Q] - toggle direct follow / human camera control\n\t"
		"[A] [D] [W] [S] - rotate left/right, raise/sink camera's eye\n\t"
		"[O] - toggle auto zoom ON/OFF\n\t"
		"[I] [U] - zoom in/out\n\t"
		"[TAB] - switch camera\n\tNumpad\n\t\t"
		"[7] [8] [9] - huge amplitude\n\t\t"
		"[4][5][6] - moderate amplitude\n\t\t"
		"[1][2][3] - small amplitude\n\t\t"
		"Slow, fast, extreme fast shaking of the car(vertical movement)\n\t"
		"[ESC] - exit", 10, 460);

	camera2.setScreenMessage(0,
		"ShakeCam v0.1 - Keyboard Layout:\n\t"
		"[F1] - toggle this help screen ON/OFF\n\t"
		"[F2] - toggle full screen mode\n\t"
		"[UP] [END] [LEFT] [RIGHT] - accelerate ahead, break, steer left, steer right the car\n\t"
		"[DOWN] - accelerate backwards\n\t"
		"[SPACE] - hand break (sudden stop)\n\t"
		"[1]...[7] - show/hide screen message 1...7 (#7 is the DEBUG-graph by default)\n\t"
		"[R] - Reset scene\n\t"
		"[Q] - toggle direct follow / human camera control\n\t"
		"[A] [D] [W] [S] - rotate left/right, raise/sink camera's eye\n\t"
		"[O] - toggle auto zoom ON/OFF\n\t"
		"[I] [U] - zoom in/out\n\t"
		"[TAB] - switch camera\n\tNumpad\n\t\t"
		"[7] [8] [9] - huge amplitude\n\t\t"
		"[4][5][6] - moderate amplitude\n\t\t"
		"[1][2][3] - small amplitude\n\t\t"
		"Slow, fast, extreme fast shaking of the car(vertical movement)\n\t"
		"[ESC] - exit", 10, 460);

	// Saving actual parameters into the 'original' variables for the case of reset
	camera.saveParametersToOriginals();
	camera2.saveParametersToOriginals();

	curCamera = 0;
}

void drawInertiaCameraScene(Camera& cam)
{
	char message[256];

	//cam.setScreenMessage(0,
	//	"ShakeCam v0.1 - Keyboard Layout:\n\t"
	//	"[F1] - toggle this help screen ON/OFF\n\t"
	//	"[F2] - toggle full screen mode\n\t"
	//	"[UP] [END] [LEFT] [RIGHT] - accelerate ahead, break, steer left, steer right the car\n\t"
	//	"[DOWN] - accelerate backwards\n\t"
	//	"[SPACE] - hand break (sudden stop)\n\t"
	//	"[1]...[7] - show/hide screen message 1...7 (#7 is the DEBUG-graph by default)\n\t"
	//	"[R] - Reset scene\n\t"
	//	"[Q] - toggle direct follow / human camera control\n\t"
	//	"[A] [D] [W] [S] - rotate left/right, raise/sink camera's eye\n\t"
	//	"[O] - toggle auto zoom ON/OFF\n\t"
	//	"[I] [U] - zoom in/out\n\t"
	//	"[TAB] - switch camera\n\tNumpad\n\t\t"
	//	"[7] [8] [9] - huge amplitude\n\t\t"
	//	"[4][5][6] - moderate amplitude\n\t\t"
	//	"[1][2][3] - small amplitude\n\t\t"
	//	"Slow, fast, extreme fast shaking of the car(vertical movement)\n\t"
	//	"[ESC] - exit", 10, 460);

	sprintf(message, "[6] FPS: %f", cam.getActualFPS());
	cam.setScreenMessage(6, message, 10, 40);


	car.animate(cam.getLastFrameTime());
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

	InitCameras();
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
	Camera& camRef = GetCamera();
	drawInertiaCameraScene(camRef);
	glutSwapBuffers();
	glutPostRedisplay();
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

void KeyInputCarControl(int key, Camera& cam)
{
	const float oneBitOfChange = cam.getLastFrameTime() * 0.012f;  // Animating the scene, this value makes the movements FPS-independent
	Vector3f originalSpeed = car.GetSpeed();
	Vector3f newSpeed = originalSpeed;

	if (key == GLUT_KEY_UP)
	{
		// Accelerate the car (ahead)
		newSpeed.z -= oneBitOfChange;
		car.SetSpeed(newSpeed);
	}
	else if (key == GLUT_KEY_DOWN)
	{
		// Accelerate the car (backwards)
		newSpeed.z += oneBitOfChange;
		car.SetSpeed(newSpeed);
	}
	else if (key == GLUT_KEY_END)
	{
		if (fabs(originalSpeed.Z()) < oneBitOfChange)
		{
			newSpeed.z = 0.0f;
			car.SetSpeed(newSpeed);
		}
		else
		{
			if (originalSpeed.Z() < 0.0f)
			{
				newSpeed.z += oneBitOfChange;
				car.SetSpeed(newSpeed);
			}

			if (originalSpeed.Z() > 0.0f)
			{
				newSpeed.z -= oneBitOfChange;
				car.SetSpeed(newSpeed);
			}
		}
	}

	if (key == 0x20)
	{
		car.stopMotion();
	}

	if (key == GLUT_KEY_LEFT)
	{   // Steering left
		car.setSteeringAngle(car.getSteeringAngle() + (oneBitOfChange * 10));
	}
	else if (key == GLUT_KEY_RIGHT)
	{   // Steering right
		car.setSteeringAngle(car.getSteeringAngle() - (oneBitOfChange * 10));
	}
}

void KeyInputCameraControl(int key, Camera& cam)
{
	const float oneBitOfChange = cam.getLastFrameTime() * 0.012f;  // Animating the scene, this value makes the movements FPS-independent

	if (key == GLUT_KEY_HOME)
	{
		curCamera = (curCamera + 1) % 2;
	}
	else if (key == GLUT_KEY_F1)
	{
		cam.ScreenMessage() ^= 0x01;
	}
	// key Q and q
	else if (key == 0x51 || key == 0x71)
	{
		directFollow = !(directFollow);
	}
	// key U and u, zoom out
	else if (key == 0x55 || key == 0x75)
	{
		cam.setFov(cam.getFov() + (oneBitOfChange * 10.0f));
		if (cam.getFov() > 179.0f)
		{
			cam.setFov(179.0f);
		}
	}
	// key I and i, zoom in
	else if (key == 0x49 || key == 0x69)
	{
		cam.setFov(cam.getFov() - (oneBitOfChange * 10.0f));
		if (cam.getFov() < 1.0f)
		{
			cam.setFov(1.0f);
		}
	}
}

void keyboard(unsigned char key, int x, int y)
{
	Camera& currentCam = GetCamera();

	switch (key)
	{
	case 'A':
	{
		//Vector3f eye2target = camera.Eye() - camera.Target();     // For the first camera 
		//Vector3f eye2target2 = camera2.Eye() - camera2.Target();  // For the second camera

		//if (cam)
		//{
		//	eye2target2 = eye2target2.Rotate(Vector3f(0.0, 1.0, 0.0), -oneBitOfChange * 4);
		//	camera2.Eye() = camera2.Target() + eye2target2;
		//}
		//else
		//{
		//	eye2target = eye2target.Rotate(Vector3f(0.0, 1.0, 0.0), -oneBitOfChange * 4);
		//	camera.Eye() = camera.Target() + eye2target;
		//}
	} break;
	case 'Q':
	case 'q':
	case 'U':
	case 'u':
	case 'I':
	case 'i':
	{
		KeyInputCameraControl(key, currentCam);
	} break;
	case 'R':
	case 'r':
	{
		car.stopMotion();
		car.setPosition(0.0f, 0.0f, -5.0f);
		car.setSteeringAngle(0.0f);
		car.setOrientation(0.0f, 0.0f, 0.0f);
		currentCam.reset();
	} break;
	case ' ':
	{
		KeyInputCarControl(key, currentCam);
	} break;
	case 27:
		exit(0);
		break;
	}
}

void SpecialKeyboard(int key, int x, int y)
{
	Camera& currentCam = GetCamera();
	const float oneBitOfChange = currentCam.getLastFrameTime() * 0.012f;  // Animating the scene, this value makes the movements FPS-independent
	Vector3f originalSpeed = car.GetSpeed();
	Vector3f newSpeed = originalSpeed;

	switch (key)
	{
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
		case GLUT_KEY_END:
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT:
		{
			KeyInputCarControl(key, currentCam);
		} break;
		break;
		case GLUT_KEY_HOME:
		case GLUT_KEY_F1:
		{
			KeyInputCameraControl(key, currentCam);
		} break;
	}

	glutPostRedisplay();
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
	glutSpecialFunc(SpecialKeyboard);
	//glutTimerFunc(30, timer_cb, 0);
	glutMainLoop();
	return 0;
}

