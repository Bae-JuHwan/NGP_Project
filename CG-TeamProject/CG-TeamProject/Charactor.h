#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "헤더.h"

struct AABB {
	glm::vec3 min; // 충돌박스의 최소 좌표 (x, y, z)
	glm::vec3 max; // 충돌박스의 최대 좌표 (x, y, z)

	// AABB 갱신 메서드
	void update(const glm::vec3& position, const glm::vec3& offsetMin, const glm::vec3& offsetMax) {
		min = position + offsetMin;
		max = position + offsetMax;
	}

	void updateRotatedAABB(const glm::vec3& position, const glm::vec3& offsetMin, const glm::vec3& offsetMax, float rotationAngle, glm::vec3 rotationAxis) {
		glm::vec3 vertices[8] = {
			position + offsetMin,
			position + glm::vec3(offsetMin.x, offsetMin.y, offsetMax.z),
			position + glm::vec3(offsetMin.x, offsetMax.y, offsetMin.z),
			position + glm::vec3(offsetMin.x, offsetMax.y, offsetMax.z),
			position + glm::vec3(offsetMax.x, offsetMin.y, offsetMin.z),
			position + glm::vec3(offsetMax.x, offsetMin.y, offsetMax.z),
			position + glm::vec3(offsetMax.x, offsetMax.y, offsetMin.z),
			position + offsetMax
		};

		glm::mat4 rotationMatrix = glm::translate(glm::mat4(1.0f), position);
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngle), rotationAxis);
		rotationMatrix = glm::translate(rotationMatrix, -position);
		for (int i = 0; i < 8; ++i) {
			glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i], 1.0f);
			vertices[i] = glm::vec3(rotatedVertex);
		}

		glm::vec3 newMin = vertices[0];
		glm::vec3 newMax = vertices[0];
		for (int i = 1; i < 8; ++i) {
			newMin = glm::min(newMin, vertices[i]);
			newMax = glm::max(newMax, vertices[i]);
		}

		min = newMin;
		max = newMax;
	}

};


inline void InitBuffer(GLuint& vao, GLuint* vbo, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

inline void InitPart(const std::string& filePath, Model& model, GLuint& vao, GLuint* vbo, const glm::vec3& color) {
	read_obj_file(filePath, model);

	std::vector<Vertex> expandedVertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < model.faces.size(); ++i) {
		Vertex v1 = { model.vertices[model.faces[i].v1].x,
					  model.vertices[model.faces[i].v1].y,
					  model.vertices[model.faces[i].v1].z,
					  color };
		Vertex v2 = { model.vertices[model.faces[i].v2].x,
					  model.vertices[model.faces[i].v2].y,
					  model.vertices[model.faces[i].v2].z,
					  color };
		Vertex v3 = { model.vertices[model.faces[i].v3].x,
					  model.vertices[model.faces[i].v3].y,
					  model.vertices[model.faces[i].v3].z,
					  color };

		expandedVertices.push_back(v1);
		expandedVertices.push_back(v2);
		expandedVertices.push_back(v3);

		indices.push_back(expandedVertices.size() - 3);
		indices.push_back(expandedVertices.size() - 2);
		indices.push_back(expandedVertices.size() - 1);
	}

	InitBuffer(vao, vbo, expandedVertices, indices);
}

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
		vboCharacter1RightLeg[2];	//, vboCharacter2[2]
	Model modelCharacter1Body, modelCharacter1BackPattern, modelCharacter1Blusher, modelCharacter1Eye, modelCharacter1Face, modelCharacter1LeftArm, modelCharacter1RightArm, modelCharacter1LeftLeg, modelCharacter1RightLeg;
	GLuint VaoCheckBox, VboCheckBox[2];
public:
	AABB CAABB = {
	Position + glm::vec3(-0.70f, 0.0f, -0.72f),
	Position + glm::vec3(0.70f, 1.84f, 0.63f)
	};
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