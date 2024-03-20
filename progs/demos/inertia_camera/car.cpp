#include "car.hpp"

// Car
Car::Car(float posX /*= 0.0f*/, float posY /*= 0.0f*/, float posZ /*= 0.0f*/)
	: MovableObject(posX, posY, posZ)
{
	steeringAngle = 0.0f;
	frontWheelFromCenter = 1.5f;
	rearWheelFromCenter = -1.5f;
	minMovementDuringSteering = (frontWheelFromCenter - rearWheelFromCenter) / 50.0f;
	allAroundSize = 5.0;
	steeringWasModified = false;
}

void Car::drawWheel() const
{
	float y, z;
	
	glBegin(GL_POLYGON);
	for (float angle = 0.0f; angle <= 360.0f; angle += 20.0f)
		glVertex3f(0.1f, 0.35f * cosf(M_PI * angle / 180.0f), 0.35f * sinf(M_PI * angle / 180.0f));
	glEnd();

	glBegin(GL_POLYGON);
	for (float angle = 360.0f; angle >= 0.0f; angle -= 20.0f)
		glVertex3f(-0.1f, 0.35f * cosf(M_PI * angle / 180.0f), 0.35f * sinf(M_PI * angle / 180.0f));
	glEnd();
	
	glBegin(GL_QUAD_STRIP);
	for (float angle = 0.f; angle <= 360.f; angle += 20.f)
	{
		y = 0.35f * cosf(M_PI * angle / 180.0f);
		z = 0.35f * sinf(M_PI * angle / 180.0f);
		glVertex3f(0.1f, y, z);
		glVertex3f(-0.1f, y, z);
	}
	glEnd();
}

void Car::draw() const
{
	glColor3f(0.3f, 0.3f, 0.3f);

	// Undercarriage
	glBegin(GL_QUADS);
		glVertex3f(-1.0f, 0.2f, 2.0f);
		glVertex3f(-1.0f, 0.2f, -2.0f);
		glVertex3f(1.0f, 0.2f, -2.0f);
		glVertex3f(1.0f, 0.2f, 2.0f);
	glEnd();

	// Spoiler
	glBegin(GL_QUAD_STRIP);
		glVertex3f(1.1f, 0.4f, 2.1f);
		glVertex3f(1.0f, 0.2f, 2.0f);
		glVertex3f(1.1f, 0.4f, -2.1f);
		glVertex3f(1.0f, 0.2f, -2.0f);
		glVertex3f(-1.1f, 0.4f, -2.1f);
		glVertex3f(-1.0f, 0.2f, -2.0f);
		glVertex3f(-1.1f, 0.4f, 2.1f);
		glVertex3f(-1.0f, 0.2f, 2.0f);
		glVertex3f(1.1f, 0.4f, 2.1f);
		glVertex3f(1.0f, 0.2f, 2.0f);
	glEnd();

	// Bottom part of coachwork
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_QUAD_STRIP);
		glVertex3f(1.05f, 1.0f, 2.1f);
		glVertex3f(1.1f, 0.4f, 2.1f);
		glVertex3f(1.05f, 1.0f, -0.9f);
		glVertex3f(1.1f, 0.4f, -0.9f);
		glVertex3f(1.0f, 0.8f, -1.9f);
		glVertex3f(1.1f, 0.4f, -2.1f);
		glVertex3f(-1.0f, 0.8f, -1.9f);
		glVertex3f(-1.1f, 0.4f, -2.1f);
		glVertex3f(-1.05f, 1.0f, -0.9f);
		glVertex3f(-1.1f, 0.4f, -0.9f);
		glVertex3f(-1.05f, 1.0f, 2.1f);
		glVertex3f(-1.1f, 0.4f, 2.1f);
		glVertex3f(1.05f, 1.0f, 2.1f);
		glVertex3f(1.1f, 0.4f, 2.1f);
	glEnd();

	// Roof and bonnet
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(0.8f, 1.5f, 1.8f);
		glVertex3f(0.8f, 1.5f, -0.2f);
		glVertex3f(-0.8f, 1.5f, -0.2f);
		glVertex3f(-0.8f, 1.5f, 1.8f);
		glVertex3f(1.05f, 1.0f, -0.9f);
		glVertex3f(1.0f, 0.8f, -1.9f);
		glVertex3f(-1.0f, 0.8f, -1.9f);
		glVertex3f(-1.05f, 1.0f, -0.9f);
	glEnd();

	// Windows
	glColor3f(0.7f, 0.7f, 0.9f);
	glBegin(GL_QUAD_STRIP);
		glVertex3f(0.8f, 1.5f, 1.8f);
		glVertex3f(1.05f, 1.0f, 2.1f);
		glVertex3f(0.8f, 1.5f, -0.2f);
		glVertex3f(1.05f, 1.0f, -0.9f);
		glVertex3f(-0.8f, 1.5f, -0.2f);
		glVertex3f(-1.05f, 1.0f, -0.9f);
		glVertex3f(-0.8f, 1.5f, 1.8f);
		glVertex3f(-1.05f, 1.0f, 2.1f);
		glVertex3f(0.8f, 1.5f, 1.8f);
		glVertex3f(1.05f, 1.0f, 2.1f);
	glEnd();

	// Wheels
	//glColor3f(0.1f, 0.1f, 0.1f);
	//glPushMatrix();
	//	glTranslatef(-1.1f, 0.35f, -frontWheelFromCenter);
	//	glRotatef(steeringAngle, 0.0f, 1.0f, 0.0f);
	//	drawWheel();
	//glPopMatrix();

	//glPushMatrix();
	//	glTranslatef(1.1f, 0.35f, -frontWheelFromCenter);
	//	glRotatef(steeringAngle, 0.0f, 1.0f, 0.0f);
	//	drawWheel();
	//glPopMatrix();

	//glPushMatrix();
	//	glTranslatef(-1.1f, 0.35f, -rearWheelFromCenter);
	//	drawWheel();
	//glPopMatrix();

	//glPushMatrix(); // new added
	//	glTranslatef(1.1f, 0.35f, -rearWheelFromCenter);
	//	drawWheel();
	//glPopMatrix();
}

void Car::driveWithSteering(float movement)
{
	float movementMultiplier = abs(movement / minMovementDuringSteering);
	float movementSign = ((movement >= 0) ? (1.0f) : (-1.0f));
	if (movementMultiplier > 1.0) movement = abs(minMovementDuringSteering);
	else movement = abs(movement);
	Vector3f wheelFront(0.0, 0.0, -frontWheelFromCenter);
	Vector3f wheelRear(0.0, 0.0, -rearWheelFromCenter);
	Vector3f movementRear(0.0, 0.0, -movement);
	Vector3f movementFront = movementRear.Rotate(Vector3f(0.0f, 1.0f, 0.0f), steeringAngle);
	wheelFront += movementFront;
	wheelRear += movementRear;
	Vector3f objectMovement = (wheelFront + wheelRear) * 0.5f;  // This is not precise solution (but fast and simple)

	Vector3f newWheelAxis = wheelFront - wheelRear;
	float orientationChange = movementSign * 180.0f * (float)atan((double)(newWheelAxis.X() / newWheelAxis.Z())) / M_PI;

	for (int i = 0; i < (int)movementMultiplier; i++) {
		position += (objectMovement.Rotate(Vector3f(0.0, 1.0, 0.0), orientation.Y()) * movementSign);
		//orientation.Y() += orientationChange;
		orientation.SetY(orientation.Y() + orientationChange);

		objectMovement = objectMovement.Rotate(Vector3f(0.0, 1.0, 0.0), orientationChange);
	}
	objectMovement *= (movementMultiplier - (int)movementMultiplier);
	orientationChange *= (movementMultiplier - (int)movementMultiplier);
	position += (objectMovement.Rotate(Vector3f(0.0, 1.0, 0.0), orientation.Y()) * movementSign);
	//orientation.Y() += orientationChange;
	orientation.SetY(orientation.Y() + orientationChange);
}

void Car::animate(float deltaTime)
{
	driveWithSteering(-speed.Z() * deltaTime / 1000.0f);
	speed += (acceleration * deltaTime / 1000.0f);
	orientation += (rotationSpeed * deltaTime / 1000.0f);
	if (steeringWasModified)
	{
		steeringWasModified = false;
	}
	else if (abs(steeringAngle) < (deltaTime * 0.12))
	{
		steeringAngle = 0.0;
	}
	else
	{
		if (steeringAngle > 0) steeringAngle -= (deltaTime * 0.12f);
		if (steeringAngle < 0) steeringAngle += (deltaTime * 0.12f);
	}
}

void Car::setSteeringAngle(float newSA)
{
	if (newSA > MAXSTEERINGANGLE)
	{
		steeringAngle = MAXSTEERINGANGLE;
	}
	else if (newSA < -MAXSTEERINGANGLE)
	{
		steeringAngle = -MAXSTEERINGANGLE;
	}
	else
	{
		steeringAngle = newSA;
	}
	steeringWasModified = true;
}

void Car::simulateBumps(long int actualTime, float amplitude, double frequency)
{
	//position.Y() = amplitude * (float)sin(2 * M_PI * frequency * (double)actualTime / 1000.0);
	float y = amplitude * (float)sin(2 * M_PI * frequency * (double)actualTime / 1000.0);
	orientation.SetY(y);
}

