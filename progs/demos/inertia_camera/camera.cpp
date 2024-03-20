#include "camera.hpp"
#include "car.hpp"
#include "envrnmnt.hpp"
#include "glut_helper.hpp"

#include <algorithm>
//#include <GL/GLU.h>


// Camera
Camera::Camera(float eyeX/*= 0.0f*/, float eyeY/* = 0.0f*/, float eyeZ/* = 0.0f*/, 
	float targetX/*= 0.0f*/, float targetY/*= 0.0f*/, float targetZ/*= -1.0f*/,
	float upX/*= 0.0f*/, float upY/*= 1.0f*/, float upZ/*= 0.0f*/, float fov0/*= 28.0f*/)
{
	// First store the parameters
	eye.set(eyeX, eyeY, eyeZ);   
	target.set(targetX, targetY, targetZ);
	up.set(upX, upY, upZ);
	fovToReach = actualFov = fov0;
	// Saving parameters as 'originals' for RESET
	origEye = eye;
	origTarget = target;
	origUp = up;
	origFov = actualFov;

	autoZoom = false;
	targetSize = 2.0f;
	sumWeight[0] = 0;
	for (int i = 0; i < MAXEXTRAPOLLENGTH; i++)
	{
		objPosList[i].set(target.X(), target.Y(), target.Z());
		lpfObjPosList[i] = objPosList[i];
		objPosDifList[i].set(0.0f, 0.0f, 0.0f);
		objPosDifDifList[i].set(0.0f, 0.0f, 0.0f);
		objFrameWeightList[i] = 1.0f + ((float)i / 50.0f);
		if (i == 0) sumWeight[0] = objFrameWeightList[0];
		else sumWeight[i] = sumWeight[i - 1] + objFrameWeightList[i];
	}

	// FPS measuring
	getCurrentTimeInto(timeStamp);
	lastTimeStamp = timeStamp;
	for (int i = 0; i < FPSTIMELISTLENGTH; i++) timeStampList[i] = lastTimeStamp;
	tslFromIdx = 0;
	tslToIdx = 1;
	// Screen messages
	//screenMessage = 255;                      // All of the screen messages will be shown
	screenMessage = 1 + 32 + 64 + 128;          // Set the basic screen messages (1,5,6,7)
	for (int i = 0; i < 8; i++) {
		screenMessageText[i] = new char[1024];  // The available message length is 1023 characters
		sprintf(screenMessageText[i], "[%d] nothing yet...", i);
		screenMessagePosition[i].set(10.0f, 20.0f * (8 - i), 0.0);
	}
	figX = 0;

}

void Camera::printScreenMessages(int w, int h)
{
	for (int n = 0; n < 7; n++)
	{
		if ((screenMessage >> n) & 0x01)
		{
			glColor3f(0.7f, 0.8f, 0.9f);
			glRasterPos2f(2 * screenMessagePosition[n].X() / (float)w - 1.0f,
				2 * screenMessagePosition[n].Y() / (float)h - 1.0f);
			int lineBreak = 1;
			for (int i = 0; i < 1024; i++) {
				if (screenMessageText[n][i] == '\0')
				{
					break;
				}
				else if (screenMessageText[n][i] == '\n')
				{
					glRasterPos2f(2 * screenMessagePosition[n].X() / (float)w - 1.0f,
						2 * (screenMessagePosition[n].Y() - (float)lineBreak * 16.0f) / (float)h - 1.0f);
					lineBreak++;
				}
				else if (screenMessageText[n][i] == '\t')
				{
					for (int space = 0; space < 4; space++)
					{
						glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ' ');
					}
				}
				else
				{
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, screenMessageText[n][i]);
				}
			}
		}
	}

	//// DEBUG: Figure drawing
	//if (!((screenMessage >> 7) & 0x01)) return;

	//figX = figX % w;
	//fig[figX].set(screenMessagePosition[n].X() / 100.0f, screenMessagePosition[n].Y() / 100.0f, screenMessagePosition[n].Z() / 100.0f);
	//glBegin(GL_POINTS);
	//	for (int i = 0; i < FIGLENGTH; i++) {
	//		glColor3f(0.0, 0.0, 1.0);
	//		glVertex2f(2.0 * (float)i / (float)w - 1.0, fig[i].Z());
	//		glColor3f(0.0, 1.0, 0.0);
	//		glVertex2f(2.0 * (float)i / (float)w - 1.0, fig[i].Y());
	//		glColor3f(1.0, 0.0, 0.0);
	//		glVertex2f(2.0 * (float)i / (float)w - 1.0, fig[i].X());
	//	}
	//glEnd();
}

void Camera::animate()
{
	float fovDiff = fovToReach - actualFov;
	float oneBitFovAddition = 0.0;
	if (fovDiff) {
		float deltaTime = getLastFrameTime();
		oneBitFovAddition = ((fovDiff > 0)?(1.0f):(-1.0f)) * deltaTime * 0.06f;
		if (abs(fovDiff) < 15.0f) oneBitFovAddition *= (abs(fovDiff) / 15.0f);
		if (abs(oneBitFovAddition) < abs(fovDiff)) actualFov += oneBitFovAddition;
		if (actualFov > 179.0f) actualFov = 179.0f;
		if (actualFov < 0.001f) actualFov = 0.001f;
	}
}

void Camera::DrawCarScene(const Car& car, const Environment& env,
	bool directFollow, int wWidth, int wHeight, GLuint texture)
{
	recalculateTime();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	placeObjectHereToFollow(car.GetPosition(), directFollow);
	
	Render(wWidth, wHeight, car, env, texture);

}

void Camera::Render(int width, int height, const Car& car,
	const Environment& env, GLuint texture)
{
	glPushMatrix();
		glLoadIdentity();				
		glMatrixMode(GL_PROJECTION);							
		glLoadIdentity();
		printScreenMessages(width, height);

		if (autoZoom)
		{
			fovToReach = 360.0f * (float)atan((targetSize / 2.0f) / (target - eye).Length()) / M_PI;
		}
	
		if (fovToReach > 179.0f)
		{
			fovToReach = 179.0f;
		}
	
		if (fovToReach < 0.001f)
		{
			fovToReach = 0.001f;
		}

		gluPerspective(actualFov, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);
		gluLookAt(eye.X(), eye.Y(), eye.Z(), target.X(), target.Y(), target.Z(), up.X(), up.Y(), up.Z());
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);								
	glPushMatrix();
		glLoadIdentity();
		buildScene(car, env, texture);	// Sample scene inserted here. This function needs to be defined  
	glPopMatrix();

}

void Camera::buildScene(const Car& car, const Environment& env, GLuint texture)
{
	car.drawInScene();
	//glBindTexture(GL_TEXTURE_2D, texture);
	env.drawInScene();
}

void Camera::placeObjectHereToFollow(Vector3f objPos, bool directFollow /* = false*/)
{
	if (actualFPS == 0.0) return;
	// A cameraman is not so fast. He needs about half second to notice that the followed object moves away.
	float lengthForExtrpl = std::min<float>(actualFPS, (float)MAXEXTRAPOLLENGTH) * 0.7f;
	int iLengthForExtrpl = (int)lengthForExtrpl;  // To save many float->int casting time
	// Besides, a cameraman has inertia too, so he works like a Low Pass Filter
	float lengthForLPF = std::min<float>(actualFPS, (float)MAXEXTRAPOLLENGTH) * 0.4f;
	int iLengthForLPF = (int)lengthForLPF;  // To save many float->int casting time

	// Calculating of a new "low pass filtered object position"
	Vector3f lpfObjectLastPos = objPos;
	for (int i = 0; i < (iLengthForLPF - 1); i++) lpfObjectLastPos += objPosList[i];
	if (iLengthForLPF != 0) lpfObjectLastPos *= (1 / (float)iLengthForLPF);
	else lpfObjectLastPos = objPos;

	// Shifting and inserting the object positions and differences
	for (int i = MAXEXTRAPOLLENGTH - 1; i > 0; i--) {  // Shifting...
		objPosList[i] = objPosList[i - 1];
		lpfObjPosList[i] = lpfObjPosList[i - 1];
		objPosDifList[i] = objPosDifList[i - 1];
		objPosDifDifList[i] = objPosDifDifList[i - 1];
	}
	objPosList[0] = objPos;  // ...and inserting new element
	lpfObjPosList[0] = lpfObjectLastPos;
	objPosDifList[0] = lpfObjPosList[0] - lpfObjPosList[1];
	objPosDifDifList[0] = objPosDifList[0] - objPosDifList[1];

	// Calculating the camera position
	Vector3f averageTargetStep;
	Vector3f averagePosDifDif;
	// We need just [iLengthForExtrpl - 2] element because they are the differences
	for (int i = 0; i < iLengthForExtrpl; i++) {
		averageTargetStep += (objPosDifList[i] * objFrameWeightList[i]);
		averagePosDifDif += (objPosDifDifList[i] * objFrameWeightList[iLengthForExtrpl - i - 1]);
	}
	averageTargetStep *= (1 / sumWeight[iLengthForExtrpl - 1]);
	averagePosDifDif *= (1 / sumWeight[iLengthForExtrpl - 1]);
	// In case of "direct follow" we leave the above calculated data
	if (directFollow) {
		target = objPos;
		return;
	}
	target = lpfObjPosList[iLengthForExtrpl] + averageTargetStep * (lengthForExtrpl + lengthForLPF / 2.0f) + averagePosDifDif;
}

void Camera::setScreenMessage(unsigned char number, char* message, int posX, int posY)
{
	if ((number < 0) || (number > 7)) return;

	strcpy(screenMessageText[number], message);
	screenMessagePosition[number].set((float)posX, (float)posY, 0.0);
}

Vector3f& Camera::ScreenMessagePositionVector(unsigned char number)
{
	if ((number < 0) || (number > 7))
		return Vector3f();
	return screenMessagePosition[number];
}

void Camera::recalculateTime()
{
	getCurrentTimeInto(timeStamp);
	timeStampList[tslToIdx] = timeStamp;
	if ((timeStampList[tslToIdx] - timeStampList[tslFromIdx]) > 0) {
		if (tslFromIdx < tslToIdx) actualFPS = ((1000.0f * (float)(tslToIdx - tslFromIdx)) / (float)(timeStampList[tslToIdx] - timeStampList[tslFromIdx])) + 1.0f;
		else actualFPS = ((1000.0f * (float)(tslToIdx + FPSTIMELISTLENGTH - tslFromIdx)) / (float)(timeStampList[tslToIdx] - timeStampList[tslFromIdx])) + 1.0f;
	}
	while ((timeStampList[tslToIdx] - timeStampList[tslFromIdx] > FPSMAEASURETIMERANGE) && (tslFromIdx != tslToIdx - 1)) {
		tslFromIdx++;
		if (tslFromIdx == FPSTIMELISTLENGTH) {
			if (tslToIdx == 0) tslToIdx = 1;
			tslFromIdx = 0;
		}
	}
	if (tslToIdx == FPSTIMELISTLENGTH - 1) {
		if (tslFromIdx == 0) tslFromIdx++;
		tslToIdx = 0;
	}
	else {
		if (tslToIdx == tslFromIdx - 1) tslFromIdx++;
		tslToIdx++;
	}
	if (tslFromIdx == FPSTIMELISTLENGTH) tslFromIdx = 0;

	// Animate the camera parameters
	animate();

	figX++;
}

float Camera::getLastFrameTime()
{
	if (actualFPS == 0.0f)
	{
		return 0.0f;
	}
	else
	{
		return (1000.0f / actualFPS);
	}
}

