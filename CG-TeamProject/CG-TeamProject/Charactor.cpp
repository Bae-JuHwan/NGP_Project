#include "Charactor.h"


Charactor::Charactor()
{
    // »ý¼ºÀÚ ÄÚµå
}

Charactor::~Charactor()   // 
{
    // ¼Ò¸êÀÚ ÄÚµå (ºñ¾î ÀÖ¾îµµ ±¦ÂúÀ½)
}

std::vector<float> CheckBox = {
    // Bottom
 -0.47f, 0.f, 0.42f,
 -0.47f, 0.f, -0.48f,
 0.47f, 0.f, -0.48f,
 0.47f, 0.f, 0.42f,

 // Top
 -0.47f, 1.84f, 0.42f,
 -0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, 0.42f,

 // Front
 -0.47f, 0.f, -0.48f,
 -0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, -0.48f,
 0.47f, 0.f, -0.48f,

 // Back face
 -0.47f, 0.f, 0.42f,
 -0.47f, 1.84f, 0.42f,
 0.47f, 1.84f, 0.42f,
 0.47f, 0.f, 0.42f,

 // Left face
 -0.47f, 0.f, 0.42f,
 -0.47f, 0.f, -0.48f,
 -0.47f, 1.84f, -0.48f,
 -0.47f, 1.84f, 0.42f,

 // Right face
 0.47f, 0.f, 0.42f,
 0.47f, 0.f, -0.48f,
 0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, 0.42f,

};


Player1::Player1()
{
	InitPart("Character1/body.obj", modelCharacter1Body, vaoCharacter1Body, vboCharacter1Body, glm::vec3(1.0f, 1.0f, 0.0f));
    InitPart("Character1/backPattern.obj", modelCharacter1BackPattern, vaoCharacter1BackPattern, vboCharacter1BackPattern, glm::vec3(0.0f, 0.0f, 0.0f));
    InitPart("Character1/blusher.obj", modelCharacter1Blusher, vaoCharacter1Blusher, vboCharacter1Blusher, glm::vec3(1.0f, 0.0f, 0.0f));
    InitPart("Character1/eye.obj", modelCharacter1Eye, vaoCharacter1Eye, vboCharacter1Eye, glm::vec3(0.0f, 0.0f, 0.0f));
    InitPart("Character1/face.obj", modelCharacter1Face, vaoCharacter1Face, vboCharacter1Face, glm::vec3(1.0f, 1.0f, 1.0f));
    InitPart("Character1/leftArm.obj", modelCharacter1LeftArm, vaoCharacter1LeftArm, vboCharacter1LeftArm, glm::vec3(1.0f, 1.0f, 0.0f));
    InitPart("Character1/rightArm.obj", modelCharacter1RightArm, vaoCharacter1RightArm, vboCharacter1RightArm, glm::vec3(1.0f, 1.0f, 0.0f));
    InitPart("Character1/leftLeg.obj", modelCharacter1LeftLeg, vaoCharacter1LeftLeg, vboCharacter1LeftLeg, glm::vec3(1.0f, 1.0f, 0.0f));
    InitPart("Character1/rightLeg.obj", modelCharacter1RightLeg, vaoCharacter1RightLeg, vboCharacter1RightLeg, glm::vec3(1.0f, 1.0f, 0.0f));

    //Ã¼Å©¹Ú½º »ý¼º
    glGenVertexArrays(1, &VaoCheckBox);
    glBindVertexArray(VaoCheckBox);

    glGenBuffers(1, VboCheckBox);

    glBindBuffer(GL_ARRAY_BUFFER, VboCheckBox[0]);
    glBufferData(GL_ARRAY_BUFFER, CheckBox.size() * sizeof(float), CheckBox.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
};

void Player1:: Draw(GLuint shaderProgramID, GLint modelMatrixLocation) {
    glm::mat4 finalCharacter1ModelMatrix = ModelMatrix;

    // ¸ö
    glm::mat4 Character1BodyModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1BodyModelMatrix));
    glBindVertexArray(vaoCharacter1Body);
    glDrawElements(GL_TRIANGLES, modelCharacter1Body.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // µî¿¡ °ËÀº ÁÙ
    glm::mat4 Character1BackPatternModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1BackPatternModelMatrix));
    glBindVertexArray(vaoCharacter1BackPattern);
    glDrawElements(GL_TRIANGLES, modelCharacter1BackPattern.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ºí·¯¼Å
    glm::mat4 Character1BlusherModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1BlusherModelMatrix));
    glBindVertexArray(vaoCharacter1Blusher);
    glDrawElements(GL_TRIANGLES, modelCharacter1Blusher.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ´«
    glm::mat4 Character1EyeModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1EyeModelMatrix));
    glBindVertexArray(vaoCharacter1Eye);
    glDrawElements(GL_TRIANGLES, modelCharacter1Eye.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ¾ó±¼
    glm::mat4 Character1FaceModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1FaceModelMatrix));
    glBindVertexArray(vaoCharacter1Face);
    glDrawElements(GL_TRIANGLES, modelCharacter1Face.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ¿ÞÆÈ
    glm::mat4 Character1LeftArmModelMatrix = finalCharacter1ModelMatrix;
    Character1LeftArmModelMatrix = glm::translate(Character1LeftArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    Character1LeftArmModelMatrix = glm::rotate(Character1LeftArmModelMatrix, glm::radians(ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1LeftArmModelMatrix = glm::translate(Character1LeftArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1LeftArmModelMatrix));
    glBindVertexArray(vaoCharacter1LeftArm);
    glDrawElements(GL_TRIANGLES, modelCharacter1LeftArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ¿À¸¥ÆÈ
    glm::mat4 Character1RightArmModelMatrix = finalCharacter1ModelMatrix;
    Character1RightArmModelMatrix = glm::translate(Character1RightArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    Character1RightArmModelMatrix = glm::rotate(Character1RightArmModelMatrix, glm::radians(-ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1RightArmModelMatrix = glm::translate(Character1RightArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1RightArmModelMatrix));
    glBindVertexArray(vaoCharacter1RightArm);
    glDrawElements(GL_TRIANGLES, modelCharacter1RightArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ¿Þ´Ù¸®
    glm::mat4 Character1LeftLegModelMatrix = finalCharacter1ModelMatrix;
    Character1LeftLegModelMatrix = glm::translate(Character1LeftLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    Character1LeftLegModelMatrix = glm::rotate(Character1LeftLegModelMatrix, glm::radians(-ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1LeftLegModelMatrix = glm::translate(Character1LeftLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1LeftLegModelMatrix));
    glBindVertexArray(vaoCharacter1LeftLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter1LeftLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ¿À¸¥´Ù¸®
    glm::mat4 Character1RightLegModelMatrix = finalCharacter1ModelMatrix;
    Character1RightLegModelMatrix = glm::translate(Character1RightLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    Character1RightLegModelMatrix = glm::rotate(Character1RightLegModelMatrix, glm::radians(ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1RightLegModelMatrix = glm::translate(Character1RightLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1RightLegModelMatrix));
    glBindVertexArray(vaoCharacter1RightLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter1RightLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Player1::~Player1()
{

};




//// Ä³¸¯ÅÍ2 ±×¸®±â
//void DrawCharacter2(GLuint shaderProgramID, GLint modelMatrixLocation) {
//	glm::mat4 finalCharacter2ModelMatrix = character2ModelMatrix;
//
//	//acc
//	glm::mat4 Character2AccModelMatrix = finalCharacter2ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2AccModelMatrix));
//	glBindVertexArray(vaoCharacter2Acc);
//	glDrawElements(GL_TRIANGLES, modelCharacter2Acc.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//body
//	glm::mat4 Character2BodyModelMatrix = finalCharacter2ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2BodyModelMatrix));
//	glBindVertexArray(vaoCharacter2Body);
//	glDrawElements(GL_TRIANGLES, modelCharacter2Body.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//hair
//	glm::mat4 Character2HairModelMatrix = finalCharacter2ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2HairModelMatrix));
//	glBindVertexArray(vaoCharacter2Hair);
//	glDrawElements(GL_TRIANGLES, modelCharacter2Hair.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//leftLeg
//	glm::mat4 Character2LeftLegModelMatrix = finalCharacter2ModelMatrix;
//	Character2LeftLegModelMatrix = glm::translate(Character2LeftLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
//	Character2LeftLegModelMatrix = glm::rotate(Character2LeftLegModelMatrix, glm::radians(-character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character2LeftLegModelMatrix = glm::translate(Character2LeftLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2LeftLegModelMatrix));
//	glBindVertexArray(vaoCharacter2LeftLeg);
//	glDrawElements(GL_TRIANGLES, modelCharacter2LeftLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//leftArm
//	glm::mat4 Character2LeftArmModelMatrix = finalCharacter2ModelMatrix;
//	Character2LeftArmModelMatrix = glm::translate(Character2LeftArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
//	Character2LeftArmModelMatrix = glm::rotate(Character2LeftArmModelMatrix, glm::radians(character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character2LeftArmModelMatrix = glm::translate(Character2LeftArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2LeftArmModelMatrix));
//	glBindVertexArray(vaoCharacter2LeftArm);
//	glDrawElements(GL_TRIANGLES, modelCharacter2LeftArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//RightLeg
//	glm::mat4 Character2RightLegModelMatrix = finalCharacter2ModelMatrix;
//	Character2RightLegModelMatrix = glm::translate(Character2RightLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
//	Character2RightLegModelMatrix = glm::rotate(Character2RightLegModelMatrix, glm::radians(character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character2RightLegModelMatrix = glm::translate(Character2RightLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2RightLegModelMatrix));
//	glBindVertexArray(vaoCharacter2RightLeg);
//	glDrawElements(GL_TRIANGLES, modelCharacter2RightLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//RightArm
//	glm::mat4 Character2RightArmModelMatrix = finalCharacter2ModelMatrix;
//	Character2RightArmModelMatrix = glm::translate(Character2RightArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
//	Character2RightArmModelMatrix = glm::rotate(Character2RightArmModelMatrix, glm::radians(-character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character2RightArmModelMatrix = glm::translate(Character2RightArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2RightArmModelMatrix));
//	glBindVertexArray(vaoCharacter2RightArm);
//	glDrawElements(GL_TRIANGLES, modelCharacter2RightArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//Clothes
//	glm::mat4 Character2ClothesModelMatrix = finalCharacter2ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2ClothesModelMatrix));
//	glBindVertexArray(vaoCharacter2Clothes);
//	glDrawElements(GL_TRIANGLES, modelCharacter2Clothes.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//Eye
//	glm::mat4 Character2EyeModelMatrix = finalCharacter2ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2EyeModelMatrix));
//	glBindVertexArray(vaoCharacter2Eye);
//	glDrawElements(GL_TRIANGLES, modelCharacter2Eye.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	//Face
//	glm::mat4 Character2FaceModelMatrix = finalCharacter2ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2FaceModelMatrix));
//	glBindVertexArray(vaoCharacter2Face);
//	glDrawElements(GL_TRIANGLES, modelCharacter2Face.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//}

//// Ä³¸¯ÅÍ3 ±×¸®±â
//void DrawCharacter3(GLuint shaderProgramID, GLint modelMatrixLocation) {
//	glm::mat4 finalCharacter3ModelMatrix = character3ModelMatrix;
//
//	// ¸ö
//	glm::mat4 Character3BodyModelMatrix = finalCharacter3ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character3BodyModelMatrix));
//	glBindVertexArray(vaoCharacter3Body);
//	glDrawElements(GL_TRIANGLES, modelCharacter3Body.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	// ´«
//	glm::mat4 Character3EyeModelMatrix = finalCharacter3ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character3EyeModelMatrix));
//	glBindVertexArray(vaoCharacter3Eyes);
//	glDrawElements(GL_TRIANGLES, modelCharacter3Eyes.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	// ¾ó±¼
//	glm::mat4 Character3FaceModelMatrix = finalCharacter3ModelMatrix;
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character3FaceModelMatrix));
//	glBindVertexArray(vaoCharacter3Face);
//	glDrawElements(GL_TRIANGLES, modelCharacter3Face.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	// ¿ÞÆÈ
//	glm::mat4 Character3LeftArmModelMatrix = finalCharacter3ModelMatrix;
//	Character3LeftArmModelMatrix = glm::translate(Character3LeftArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
//	Character3LeftArmModelMatrix = glm::rotate(Character3LeftArmModelMatrix, glm::radians(character3ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character3LeftArmModelMatrix = glm::translate(Character3LeftArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character3LeftArmModelMatrix));
//	glBindVertexArray(vaoCharacter3LeftArm);
//	glDrawElements(GL_TRIANGLES, modelCharacter3LeftArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	// ¿À¸¥ÆÈ
//	glm::mat4 Character3RightArmModelMatrix = finalCharacter3ModelMatrix;
//	Character3RightArmModelMatrix = glm::translate(Character3RightArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
//	Character3RightArmModelMatrix = glm::rotate(Character3RightArmModelMatrix, glm::radians(-character3ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character3RightArmModelMatrix = glm::translate(Character3RightArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character3RightArmModelMatrix));
//	glBindVertexArray(vaoCharacter3RightArm);
//	glDrawElements(GL_TRIANGLES, modelCharacter3RightArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	// ¿Þ´Ù¸®
//	glm::mat4 Character3LeftLegModelMatrix = finalCharacter3ModelMatrix;
//	Character3LeftLegModelMatrix = glm::translate(Character3LeftLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
//	Character3LeftLegModelMatrix = glm::rotate(Character3LeftLegModelMatrix, glm::radians(-character3ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character3LeftLegModelMatrix = glm::translate(Character3LeftLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character3LeftLegModelMatrix));
//	glBindVertexArray(vaoCharacter3LeftFoot);
//	glDrawElements(GL_TRIANGLES, modelCharacter3LeftFoot.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	// ¿À¸¥´Ù¸®
//	glm::mat4 Character3RightLegModelMatrix = finalCharacter3ModelMatrix;
//	Character3RightLegModelMatrix = glm::translate(Character3RightLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
//	Character3RightLegModelMatrix = glm::rotate(Character3RightLegModelMatrix, glm::radians(character3ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
//	Character3RightLegModelMatrix = glm::translate(Character3RightLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
//	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character3RightLegModelMatrix));
//	glBindVertexArray(vaoCharacter3RightFoot);
//	glDrawElements(GL_TRIANGLES, modelCharacter3RightFoot.faces.size() * 3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//}
