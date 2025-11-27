#pragma once
#include "Charactor.h"


class Obstacle
{
public:
	Obstacle() {};
	Obstacle(glm::vec3 setp);
	~Obstacle() {};

public:
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f); // 초기 위치
	glm::vec3 Direction = glm::vec3(1.0f, 0.0f, 0.0f); // 초기 이동방향 (오른쪽)

	GLuint vao, vbo[2];
	Model model, modelCheckBox;
	GLuint vaoCheckBox, vboCheckBox[2];		//체크박스


	GLfloat MoveSpeed{ 0.1f }; // 이동 속도
	GLfloat MaxMoveDistance{ 1.6f }; // 최대 이동속도
	GLfloat RotationAngle{ 0.0f }; // 회전 각도

	AABB CAABB{};


	void SetAABB(AABB seta) { CAABB = seta; }
	void JumpBarDraw(GLuint shaderProgramID, GLint modelMatrixLocation);
};

class Obstacle3 :public Obstacle
{
public:
	Obstacle3() {};
	Obstacle3(glm::vec3 setp);
	~Obstacle3() {};

	AABB CAABB1{};
	AABB CAABB2{};
	AABB CAABB3{};

	void SetAABB(AABB seta, AABB setb, AABB setc) { CAABB1 = seta; CAABB2 = setb; CAABB3 = setc; }
};

class BongGroup :public Obstacle3
{
public:
	BongGroup(glm::vec3 setp, glm::vec3 setd, GLfloat setMS, GLfloat setMMS, GLfloat setA);
	~BongGroup() {};

	//여기에 다른 체크박스 2개 더 추가해야함

	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
	void JumpBarCenterDraw(GLuint shaderProgramID, GLint modelMatrixLocation);
};

class HorizontalFan :public Obstacle
{
public:
	HorizontalFan(glm::vec3 setp);
	~HorizontalFan() {};

	Obstacle* Pink = nullptr;
	Obstacle* Purple = nullptr;
	AABB CAABB{};

	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
};

class VerticalFan :public Obstacle
{
public:
	VerticalFan(glm::vec3 setBp, glm::vec3 setCp, glm::vec3 setFp);
	VerticalFan(glm::vec3 setp);
	~VerticalFan() {};

	Obstacle* FanBar = nullptr;
	Obstacle* FanCenter = nullptr;
	Obstacle* VFan = nullptr;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f); // 초기 위치
	AABB CAABB{};

	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
};

class Door :public Obstacle
{
public:
	Door(glm::vec3 setBp, glm::vec3 setCp, glm::vec3 setFp);
	Door(glm::vec3 setp);
	~Door() {};

	Obstacle3* LeftD = nullptr;
	Obstacle3* Center = nullptr;
	Obstacle3* RightD = nullptr;

	glm::mat4 LeftdoorModelMatrix = glm::mat4(1.0f);
	glm::mat4 RightdoorModelMatrix = glm::mat4(1.0f);

	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
};