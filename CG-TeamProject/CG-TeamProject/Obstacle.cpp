#include "Obstacle.h"


Obstacle::Obstacle()
{

}

BongGroup::BongGroup(glm::vec3 setp, glm::vec3 setd, GLfloat setMS, GLfloat setMMS, GLfloat setA)
{
	Position = setp;
	Direction = setd;
	MoveSpeed = setMS;
	MaxMoveDistance = setMMS;
	RotationAngle = setA;
}


void BongGroup::Draw(GLuint shaderProgramID, GLint modelMatrixLocation)
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

HorizontalFan::HorizontalFan(glm::vec3 setp)
{
	Position = setp;
}

void HorizontalFan::Draw(GLuint shaderPRogramID, GLint modelMatrixLocation) {
	glm::mat4 HorizontalFanPink1ModelMatrix = glm::mat4(1.0f);
	HorizontalFanPink1ModelMatrix = glm::translate(HorizontalFanPink1ModelMatrix, Position);
	HorizontalFanPink1ModelMatrix = glm::rotate(HorizontalFanPink1ModelMatrix, glm::radians(RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPink1ModelMatrix));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//glm::mat4 HorizontalFanPurple1ModelMatrix = glm::mat4(1.0f);
	//HorizontalFanPurple1ModelMatrix = glm::translate(HorizontalFanPurple1ModelMatrix, Position);
	//HorizontalFanPurple1ModelMatrix = glm::rotate(HorizontalFanPurple1ModelMatrix, glm::radians(RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPink1ModelMatrix));

	//glBindVertexArray(vaoCheckBox);
	//glDrawElements(GL_TRIANGLES, modelCheckBox.faces.size() * 3, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);

}