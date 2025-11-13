#include "Obstacle.h"


Obstacle::Obstacle(glm::vec3 setp, glm::vec3 setd, GLfloat setMS, GLfloat setMMS, GLfloat setA)
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

void Obstacle::Draw(GLuint shaderProgramID, GLint modelMatrixLocation)
{
	glm::mat4 finalBong1ModelMatrix = ModelMatrix;
	ModelMatrix = glm::translate(glm::mat4(1.0f), Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalBong1ModelMatrix));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//체크박스 그리기
	glm::mat4 bongCheckBox1ModelMatrix = glm::translate(glm::mat4(1.0f), Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(bongCheckBox1ModelMatrix));

	glBindVertexArray(vaoCheckBox);
	glDrawElements(GL_TRIANGLES, modelCheckBox.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}