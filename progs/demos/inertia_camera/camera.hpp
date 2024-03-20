#ifndef CAMERA_H
#define CAMERA_H

//************************************************************************* //
// Camera class which is able to shake while follows a rough moving target
//
// Authors: Aszódi, Barnabás
//          Czuczor, Szabolcs
// Created: July, 2004., Budapest, Hungary
//
// Budapest University of Technology and Economics,
// Department of Control Engineering and Information Technology (BME-IIT)
//************************************************************************* //
#include <string.h>
#include <algorithm>
#include <GL/freeglut.h>

#include "vector.hpp"
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <timeapi.h>

// Overriding the default abs() function to avoid the casting error from float to integer
#define abs(a) (((a)>=0)?(a):(-(a)))

// This "define" block is needed to be able to avoid the undesirable effect of variable frame rate
#ifndef FRAMERATECONTROL
#define FRAMERATECONTROL

//struct __timeb64 timeStruct;

// This solution dies in 50 days
#define getCurrentTimeInto(LongIntTimeStamp) LongIntTimeStamp=timeGetTime()
// The same function, but it not dies in 50 days 
//#define getCurrentTimeInto(LongIntTimeStamp) { _ftime64(&timeStruct); LongIntTimeStamp = 1000 * timeStruct.time + timeStruct.millitm; }
#endif

#define MAXEXTRAPOLLENGTH 1000                    // Frame length that the inertia of the Camera is derived from
#define FPSTIMELISTLENGTH 2000                    // The length of the time measure list for the real-time frame rate calculation
#define FPSMAEASURETIMERANGE 1000                 // The maximum time (in ms) which between the FPS changing can be perceptible
//#define FRAMECOUNTTOAVERAGE 10                  // The number of frames that we use to calculate the last frame time

// For DEBUG
#define FIGLENGTH 2000                            // Needed just for the drawing of the graph

class Car;
class Environment;

//  Camera
class Camera
{
private:
	Vector3f eye;                                 // The eye position of the Camera object
	Vector3f target;                              // The point that the Camera look at (the view point)
	Vector3f up;                                  // The [up] vector of the Camera object (to know, where is "up" in the virtual world)
	float actualFov;                              // Field Of View in degree
	float fovToReach;                             // The camera zooms to reach this value
	
	// Originally saved parameters for RESET (These would be the parameters got through the constructor)
	Vector3f origEye;                             // Original eye point position
	Vector3f origTarget;                          // Original view point position
	Vector3f origUp;                              // Original up vector
	float origFov;                                // Original value of field of view

	bool autoZoom;                                // Should we change the FOV automatically?
	float targetSize;                             // How big is the watched object at the Camera.target that is need to be visible on the screen?

	// Class members for calculate time and the frame rate
	long int timeStamp, lastTimeStamp;            // [timeStamp] = actual time, [lastTimeStamp] = older time to calculate the FPS 
	float actualFPS;                              // The actual FPS value
	long int timeStampList[FPSTIMELISTLENGTH];    // Time measure list for the real-time frame rate calculation
	int tslToIdx, tslFromIdx;                     // Indices for the time measure list

	// For DEBUG /////////
	unsigned char screenMessage;                  // There can be 8 different messages to show on the screen (controlled by the 8 bit of this variable)
	char* screenMessageText[8];                   // The pointers for these messages which can be maximum 1023 char long
	Vector3f screenMessagePosition[8];            // A list for the screen messages
	Vector3f fig[FIGLENGTH];                      // This stores the points of the graphs
	int figX;                                     // Current x value of the drawing index along the X axis
	// End of "For DEBUG" /////

	// Parameters for shake-calculation
	Vector3f objPosList[MAXEXTRAPOLLENGTH];       // List for the object positions
	Vector3f objPosDifList[MAXEXTRAPOLLENGTH];    // List for the differences of the positions
	Vector3f objPosDifDifList[MAXEXTRAPOLLENGTH]; // List for the "differences of differences"
	Vector3f lpfObjPosList[MAXEXTRAPOLLENGTH];    // List for the object positions after low pass filter
	float objFrameWeightList[MAXEXTRAPOLLENGTH];  // List for the weighting of the position values
	float sumWeight[MAXEXTRAPOLLENGTH];           // Stores the sum of the weights

	// Print the help messages
	void printScreenMessages(int w, int h);

	// Animate the camera fov parameter
	void animate();

public:
	// The constructor of the class
	Camera(float eyeX = 0.0f, float eyeY = 0.0f, float eyeZ = 0.0f, float targetX = 0.0f, float targetY = 0.0f, float targetZ = -1.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f, float fov0 = 28.0f);

	// Saving actual parameters as originals
	void saveParametersToOriginals() {
		origEye = eye;
		origTarget = target;
		origUp = up;
		origFov = actualFov;
	}

	// Restoring parameters from originals
	void reset() {
		eye = origEye;
		target = origTarget;
		up = origUp;
		fovToReach = origFov;
	}

	float getActualFPS() { return actualFPS; }

	long int getCurrentTime() { return timeStamp; }

	Vector3f& Eye() { return eye; }
	Vector3f& Target() { return target; }
	float getFov() { return actualFov; }
	void setFov(float newFov) { actualFov = fovToReach = newFov; }
	float& FovToReach() { return fovToReach; }
	bool getAutoZoom() { return autoZoom; }
	void setAutoZoom(bool newAZ, float objectSize) { autoZoom = newAZ; targetSize = objectSize; }
	float& TargetSize() { return targetSize; }
	unsigned char& ScreenMessage() { return screenMessage; }

	// This function receives the actual position of the object,
	// stores it in the list and calculates the actual position of the camera's target  
	void placeObjectHereToFollow(Vector3f objPos, bool directFollow = false);

    // Fill this function to create your own scene
	void buildScene(const Car& car, const Environment& env, GLuint texture);

	void setScreenMessage(unsigned char number, char* message, int posX, int posY);
	Vector3f& ScreenMessagePositionVector(unsigned char number);


	void DrawCarScene(const Car& car, const Environment& env, 
		bool directFollow, int wWidth, int wHeight, GLuint texture);

	float getLastFrameTime();

protected:
	// This function defines the matrices, the parameters of the camera and loads the scene 
	void Render(int width, int height, const Car& car, const Environment& env,
		GLuint texture);

	// Real-time calculation of the frame rate in FPS
	void recalculateTime();

};

#endif
