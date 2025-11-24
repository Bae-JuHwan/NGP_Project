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
	// 1. Model Matrix 전달
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->ModelMatrix));

	// 2. 봉 모델 그리기
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 3. 체크박스 렌더링 (체크박스도 ModelMatrix를 사용해야 함)
	// 체크박스 위치를 봉과 동일하게 이동시키기 위해 동일한 행렬을 다시 전달
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->ModelMatrix));

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