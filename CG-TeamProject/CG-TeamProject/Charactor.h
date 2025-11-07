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

public:
	//스피드
	GLfloat MSpeed{ 0.2f };	//Move Speed
	GLfloat JSpeed{ 0.3f };	//Jump Speed
	//중력
	GLfloat gravity{ 0.015f };
	GLfloat realGravity{ 0.7f };


	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 InitialPosition = glm::vec3(0.0f, 0.0f, 0.0f);


	int SwingDirection{ 1 };

	//팔다리 움직임 각도
	GLfloat RotationAngle{ 0.0f };
	GLfloat ArmLegSwingAngle{ 0.0f };
	GLfloat MaxSwingAngle{ 30.0f };

	bool IsSwing{ false };
	bool IsJumping{ false };
	bool IsOnMap{ false };

	//GLvoid SetDirection(glm::vec3 dir);
	//GLvoid SetInitialPosition(glm::vec3 dir);
	//const GLfloat ArmLegSwingAngle() { return ArmLegSwingAngle; }
	//void ArmLegSwingAngle(GLfloat NAngle) { ArmLegSwingAngle+=NAngle; }

};

class Player1 : public Charactor
{
public:
	Player1();
	~Player1();

private:
	GLuint vaoCharacter1Body, vaoCharacter1BackPattern, vaoCharacter1Blusher, vaoCharacter1Eye, vaoCharacter1Face, vaoCharacter1LeftArm, vaoCharacter1RightArm, vaoCharacter1LeftLeg, vaoCharacter1RightLeg;
	GLuint vboCharacter1Body[2], vboCharacter1BackPattern[2], vboCharacter1Blusher[2], vboCharacter1Eye[2], vboCharacter1Face[2], vboCharacter1LeftArm[2], vboCharacter1RightArm[2], vboCharacter1LeftLeg[2],
		vboCharacter1RightLeg[2], vboCharacter2[2];
	Model modelCharacter1Body, modelCharacter1BackPattern, modelCharacter1Blusher, modelCharacter1Eye, modelCharacter1Face, modelCharacter1LeftArm, modelCharacter1RightArm, modelCharacter1LeftLeg, modelCharacter1RightLeg;
	GLuint VaoCheckBox, VboCheckBox[2];

public:
	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
	//glm::vec3 Position() { return Position; }
	//void Position(glm::vec3 NPosition) { Position += NPosition; }

	//void p1ModelMatrix(glm::mat4 NMatrix) { ModelMatrix += NMatrix; }
	//glm::mat4 ModelMatrix() { return ModelMatrix; }
};

class Player2 : public Charactor
{
public:

private:
	GLuint vaoCharacter2Acc, vaoCharacter2Body, vaoCharacter2Clothes, vaoCharacter2Hair, vaoCharacter2LeftLeg, vaoCharacter2RightLeg, vaoCharacter2LeftArm, vaoCharacter2RightArm, vaoCharacter2Eye, vaoCharacter2Face;
	GLuint vboCharacter2Acc[2], vboCharacter2Body[2], vboCharacter2Clothes[2], vboCharacter2Hair[2], vboCharacter2LeftLeg[2], vboCharacter2RightLeg[2], vboCharacter2LeftArm[2], vboCharacter2RightArm[2], vboCharacter2Eye[2], vboCharacter2Face[2];
	Model modelCharacter2Acc, modelCharacter2Body, modelCharacter2Hair, modelCharacter2Clothes, modelCharacter2LeftLeg, modelCharacter2RightLeg, modelCharacter2LeftArm, modelCharacter2RightArm, modelCharacter2Eye, modelCharacter2Face;

};

class Player3 : public Charactor
{
public:

private:
	GLuint vaoCharacter3Body, vaoCharacter3Face, vaoCharacter3Eyes, vaoCharacter3LeftArm, vaoCharacter3RightArm, vaoCharacter3LeftFoot, vaoCharacter3RightFoot;
	GLuint vboCharacter3Body[2], vboCharacter3Face[2], vboCharacter3Eyes[2], vboCharacter3LeftArm[2], vboCharacter3RightArm[2], vboCharacter3LeftFoot[2], vboCharacter3RightFoot[2];
	Model modelCharacter3Body, modelCharacter3Face, modelCharacter3Eyes, modelCharacter3LeftArm, modelCharacter3RightArm, modelCharacter3LeftFoot, modelCharacter3RightFoot;

};