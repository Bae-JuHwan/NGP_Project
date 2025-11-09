#include "Obstacle.h"


Obstacle::Obstacle(glm::vec3 setp,glm::vec3 setd,GLfloat setMS, GLfloat setMMS,GLfloat setA)
{
	Position = setp;
	Direction = setd;
	MoveSpeed = setMS;
	MaxMoveDistance = setMMS;
	RotationAngle = setA;
}

Obstacle::~Obstacle()
{

}