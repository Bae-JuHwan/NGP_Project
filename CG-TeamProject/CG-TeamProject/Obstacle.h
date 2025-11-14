#pragma once
#include "Charactor.h"

class Obstacle
{
public:
	Obstacle();
	~Obstacle() {};

public:
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f); // 초기 위치
	glm::vec3 Direction = glm::vec3(1.0f, 0.0f, 0.0f); // 초기 이동 방향 (오른쪽)

	GLuint vao, vbo[2];
	Model model,modelCheckBox;
	GLuint vaoCheckBox, vboCheckBox[2];		//체크박스


	GLfloat MoveSpeed{ 0.1f }; // 이동 속도
	GLfloat MaxMoveDistance{ 1.6f }; // 최대 이동 거리
	GLfloat RotationAngle{ 0.0f }; // 회전 각도

	AABB CAABB{};
	

	void SetAABB(AABB seta) { CAABB = seta; }
};

class BongGroup :public Obstacle
{
	public:
	BongGroup(glm::vec3 setp, glm::vec3 setd, GLfloat setMS, GLfloat setMMS, GLfloat setA);
	~BongGroup() {};

	//여기에 다른 체크박스 2개 더 추가해야함

	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
};

class HorizontalFan :public Obstacle
{
public:
	HorizontalFan(glm::vec3 setp);
	~HorizontalFan() {};

	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
};