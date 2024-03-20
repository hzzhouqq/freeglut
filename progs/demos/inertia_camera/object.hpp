#ifndef OBJECT_H
#define OBJECT_H

//#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include "vector.hpp"

#define abs(a) (((a)>=0)?(a):(-(a)))

// This class is the base object class.
// This defines all of the common parameters for the objects.
class MovableObject
{
protected:
	Vector3f position;                            // The position of the Object
	Vector3f speed;                               // The actual speed
	Vector3f acceleration;                        // The actual acceleration
	Vector3f orientation;                         // The actual orientation (Euler angles in degree, around X, Y and Z axes)
	Vector3f rotationSpeed;                       // The actual speed of rotation;
	float allAroundSize;                          // This is the diameter of the bounding sphere of the moveable object

	// You need to fill this function with another content to set your inherited object visible
	virtual void draw() const;
	
public:
	MovableObject(float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f);

	// This function places the object into the scene
	void drawInScene() const;
	const Vector3f& GetPosition() const { return position; }
	const Vector3f& GetSpeed() const { return speed; }
	void SetSpeed(const Vector3f& newSpeed);
	

	const Vector3f& Acceleration() const { return acceleration; }
	const Vector3f& Orientation() const { return orientation; }
	const Vector3f& RotationSpeed() const { return rotationSpeed; }

	// This function stops the object at once
	void stopMotion() {
		acceleration.set(0.0, 0.0, 0.0);
		speed.set(0.0, 0.0, 0.0);
		rotationSpeed.set(0.0, 0.0, 0.0);
	}

    // This function stops accelerating
	void evenPace() {
		acceleration.set(0.0, 0.0, 0.0);
	}

	// This function sets a new position for the object
	void setPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		position.set(x, y, z);
	}

	// This function sets a new orientation for the object
	void setOrientation(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		orientation.set(x, y, z);
	}

	// This function sets a new speed for the object
	void setSpeed(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		speed.set(x, y, z);
	}

	// This function updates the properties of the object depending on time
	// [timeDelta] : the time elapsed since the last animating sequence (in milliseconds)
	virtual void animate(float timeDelta);

	float& AllAroundSize() { return allAroundSize; }

};


#endif
