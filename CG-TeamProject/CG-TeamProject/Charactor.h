#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

class Charactor
{
public:
	Charactor();
	~Charactor();

private:
	GLfloat MSpeed{ 0.2f };	//Move Speed
	GLfloat JSpeed{ 0.3f };	//Jump Speed
	//팔다리 움직임 각도
	GLfloat CRotationAngle{ 0.0f };
	GLfloat CArmLegSwingAngle{ 0.0f };
	GLfloat MaxSwingAngle{ 30.0f };
};

