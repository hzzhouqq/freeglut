#ifndef ENVRNMNT_H
#define ENVRNMNT_H

#include "object.hpp"

// This class defines the stationary surroundings, however it is inherited
// from the MovableObject class to have the drawInScene() function.
//===============================================================
class Environment : public MovableObject
{
protected:

	virtual void draw() const override;

public:
	Environment(float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f);
};

#endif
