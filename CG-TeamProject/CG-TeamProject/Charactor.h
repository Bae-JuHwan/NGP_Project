#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "헤더.h"

class Charactor
{
public:
	Charactor();
	~Charactor();

private:
	
	//Model CharModel;	//이건 와르르 넣어야할 거 ㅅ갗은데... 그냥 ㄹ배열마냥 ㅜ 벡터 써서 핤싸

	//스피드
	GLfloat MSpeed{ 0.2f };	//Move Speed
	GLfloat JSpeed{ 0.3f };	//Jump Speed
	//팔다리 움직임 각도
	GLfloat RotationAngle{ 0.0f };
	GLfloat ArmLegSwingAngle{ 0.0f };
	GLfloat MaxSwingAngle{ 30.0f };
	//중력
	GLfloat gravity{ 0.015f };
	GLfloat realGravity{ 0.7f };


	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 InitialPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	bool IsSwing{ false };
	bool IsJumping{ false };
	bool IsOnMap{ false };

	int SwingDirection{ 1 };

public:
	//GLvoid SetDirection(glm::vec3 dir);
	//GLvoid SetInitialPosition(glm::vec3 dir);


};

class Player1 : public Charactor
{
public:

private:
	GLuint vaoCharacter1Body, vaoCharacter1BackPattern, vaoCharacter1Blusher, vaoCharacter1Eye, vaoCharacter1Face, vaoCharacter1LeftArm, vaoCharacter1RightArm, vaoCharacter1LeftLeg, vaoCharacter1RightLeg;
	GLuint vboCharacter1Body[2], vboCharacter1BackPattern[2], vboCharacter1Blusher[2], vboCharacter1Eye[2], vboCharacter1Face[2], vboCharacter1LeftArm[2], vboCharacter1RightArm[2], vboCharacter1LeftLeg[2],
		vboCharacter1RightLeg[2], vboCharacter2[2];
	//Model modelCharacter1Body, modelCharacter1BackPattern, modelCharacter1Blusher, modelCharacter1Eye, modelCharacter1Face, modelCharacter1LeftArm, modelCharacter1RightArm, modelCharacter1LeftLeg, modelCharacter1RightLeg;

};
