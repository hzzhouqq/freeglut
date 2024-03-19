#include "object.hpp"

// MovableObject
MovableObject::MovableObject(float posX /*= 0.0f*/, float posY /*= 0.0f*/, float posZ /*= 0.0f*/)
{
	position.set(posX, posY, posZ);
	allAroundSize = 2.0f;
}

void MovableObject::draw() const
{
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.0f, 0.4f, -0.3f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_QUADS);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
	glColor3f(0.3f, 0.3f, 0.3f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.0f, 0.4f, -0.3f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
	glEnd();
}

void MovableObject::drawInScene() const
{
	glPushMatrix();
		glTranslatef(position.X(), position.Y(), position.Z());
		glRotatef(orientation.X(), 1.0f, 0.0f, 0.0f);
		glRotatef(orientation.Y(), 0.0f, 1.0f, 0.0f);
		glRotatef(orientation.Z(), 0.0f, 0.0f, 1.0f);
		draw();
	glPopMatrix();
}
