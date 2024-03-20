#ifndef CAR_H
#define CAR_H

#include "object.hpp"

#define MAXSTEERINGANGLE 50                       // The max value of steering angle in degree

// This class defines the object that we want to follow.
// This class is inherited from the MovableObject class to have the drawInScene() function.
// Currently, this car has a REALLY limited controllability.
//===============================================================
class Car : public MovableObject
{
private:
	float steeringAngle;                          // The angle of the steering-wheel
	bool steeringWasModified;                     // A flag, which tells us that the steering wheel was turned
	float frontWheelFromCenter, rearWheelFromCenter;  // The distance between front and rear wheels and the center point of the car
	float minMovementDuringSteering;              // The minimal movement during steering to calculate the orientation change properly

	// This function draws a wheel
	void drawWheel() const;


	// Reposition and orientate the car depending its steeringAngle and movement
	void driveWithSteering(float movement);

protected:
	// This function draws the body of the car
	virtual void draw() const override;

public:
	Car(float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f);

	float getSteeringAngle() { return steeringAngle; }

	void setSteeringAngle(float newSA);

	// This function updates the properties of the car
	// [deltaTime] : the time in milliseconds elapsed since the last animating sequence
	virtual void animate(float deltaTime) override;

	// Tricky shake simulation of the car
	void simulateBumps(long int actualTime, float amplitude, double frequency);
};

#endif
