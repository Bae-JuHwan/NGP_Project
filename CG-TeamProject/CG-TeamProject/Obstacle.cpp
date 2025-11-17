#include "Obstacle.h"

Obstacle::Obstacle(glm::vec3 setp)
{
	Position = setp;
}

void Obstacle::JumpBarDraw(GLuint shaderProgramID, GLint modelMatrixLocation)
{
	glm::mat4 JumpBarModelMatrix = glm::mat4(1.0f);
	JumpBarModelMatrix = glm::translate(JumpBarModelMatrix, -Position); // 센터로 이동
	JumpBarModelMatrix = glm::rotate(JumpBarModelMatrix, glm::radians(RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); //Y축 회전
	JumpBarModelMatrix = glm::translate(JumpBarModelMatrix, Position); // 원래 위치로 이동
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarModelMatrix));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


////////////////////////////////////////////////////////////////////////////////////////////
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
	glm::mat4 finalModelMatrix = ModelMatrix;
	ModelMatrix = glm::translate(glm::mat4(1.0f), Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalModelMatrix));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//체크박스
	glm::mat4 bongCheckBox1ModelMatrix = glm::translate(glm::mat4(1.0f), Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(bongCheckBox1ModelMatrix));

	glBindVertexArray(vaoCheckBox);
	glDrawElements(GL_TRIANGLES, modelCheckBox.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BongGroup::JumpBarCenterDraw(GLuint shaderProgramID, GLint modelMatrixLocation)
{
	glm::mat4 finalModelMatrix = ModelMatrix;
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalModelMatrix));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


////////////////////////////////////////////////////////////////////////////////////////////
HorizontalFan::HorizontalFan(glm::vec3 setp)
{
	Position = setp;
}

void HorizontalFan::Draw(GLuint shaderPRogramID, GLint modelMatrixLocation) 
{
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


/////////////////////////////////////////////////////////////////////////////////////////////

VerticalFan::VerticalFan(glm::vec3 setBp, glm::vec3 setCp, glm::vec3 setFp)
{
	FanBar=new Obstacle(setBp);
	FanCenter=new Obstacle(setCp);
	VFan = new Obstacle(setFp);
}


VerticalFan::VerticalFan(glm::vec3 setp)
{
	Position = setp;
}

void VerticalFan::Draw(GLuint shaderPRogramID, GLint modelMatrixLocation)
{

	glm::mat4 verticalFanBar1ModelMatrix = glm::mat4(1.0f);
	verticalFanBar1ModelMatrix = glm::translate(verticalFanBar1ModelMatrix, Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanBar1ModelMatrix));

	glBindVertexArray(FanBar->vao);
	glDrawElements(GL_TRIANGLES, FanBar->model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	glm::mat4 verticalFanCenter1ModelMatrix = glm::mat4(1.0f);
	verticalFanCenter1ModelMatrix = glm::translate(verticalFanCenter1ModelMatrix, Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanCenter1ModelMatrix));

	glBindVertexArray(FanCenter->vao);
	glDrawElements(GL_TRIANGLES, FanCenter->model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	glm::mat4 verticalFan1ModelMatrix = glm::mat4(1.0f);
	verticalFan1ModelMatrix = glm::translate(verticalFan1ModelMatrix, Position);
	verticalFan1ModelMatrix = glm::rotate(verticalFan1ModelMatrix, glm::radians(VFan->RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFan1ModelMatrix));

	glBindVertexArray(VFan->vao);
	glDrawElements(GL_TRIANGLES, VFan->model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	VFan->CAABB.update(Position, glm::vec3(-2.33f, -3.39f, -0.46f), glm::vec3(2.33f, 3.39f, 0.46f));

}