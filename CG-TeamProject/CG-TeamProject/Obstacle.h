#pragma once
#include "Charactor.h"

class Obstacle
{
public:
	Obstacle(glm::vec3 setp, glm::vec3 setd, GLfloat setMS, GLfloat setMMS, GLfloat setA);
	~Obstacle();

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
	

	void Draw(GLuint shaderProgramID, GLint modelMatrixLocation);
	void SetAABB(AABB seta) { CAABB = seta; }
};

