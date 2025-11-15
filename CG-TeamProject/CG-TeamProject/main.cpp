#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <set>
#include "stdafx.h"
#include "Obstacle.h"


// 맵
GLuint vaoBottom, vaoArrowAndPillar, vaoEndPoint, vaoPoint;
GLuint vboBottom[2], vboArrowAndPillar[2], vboEndPoint[2], vboPoint[2];
Model modelBottom, modelArrowAndPillar, modelEndPoint, modelPoint;


//장애물
GLuint vaoHorizontalFanPink, vaoHorizontalFanPurple, vaoDoorOut, vaoLeftdoor, vaoRightdoor, vaoJumpBarCenter, vaoJumpBarbargroup1, vaoJumpBarbargroup2, vaoJumpBarbargroup3, vaoVerticalFanBar,
vaoVerticalFanCenter, vaoVerticalFan;
GLuint  vboHorizontalFanPink[2], vboHorizontalFanPurple[2], vboDoorOut[2], vboLeftdoor[2], vboRightdoor[2], vboJumpBarCenter[2], vboJumpBarbargroup1[2], vboJumpBarbargroup2[2],
vboJumpBarbargroup3[2], vboVerticalFanBar[2], vboVerticalFanCenter[2], vboVerticalFan[2];
Model modelHorizontalFanPink, modelHorizontalFanPurple, modelDoorOut, modelLeftdoor, modelRightdoor, modelJumpBarCenter, modelJumpBarbargroup1, modelJumpBarbargroup2, modelJumpBarbargroup3,
modelVerticalFanBar, modelVerticalFanCenter, modelVerticalFan;

//checkbox
GLuint vaoCheckBoxMap1, vboCheckBoxMap1[2], vaoCheckBoxMap2, vboCheckBoxMap2[2], vaoCheckBoxMap3, vboCheckBoxMap3[2], vaoCheckBoxMap4, vboCheckBoxMap4[2], vaoCheckBoxMap5, vboCheckBoxMap5[2];
//GLuint vaoCharacter1CheckBox, vboCharacter1CheckBox[2], vaoCharacter2CheckBox, vboCharacter2CheckBox[2], vaoCharacter3CheckBox, vboCharacter3CheckBox[3];

Model modelBongCheckBox1, modelBongCheckBox2, modelBongCheckBox3, modelBongCheckBox4, modelBongCheckBox5, modelBongCheckBox6;
GLuint vaoBongCheckBox1, vaoBongCheckBox2, vaoBongCheckBox3, vaoBongCheckBox4, vaoBongCheckBox5, vaoBongCheckBox6;
GLuint vboBongCheckBox1[2], vboBongCheckBox2[2], vboBongCheckBox3[2], vboBongCheckBox4[2], vboBongCheckBox5[2], vboBongCheckBox6[2];

GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;



GLfloat BongMove = 0.1f; // 이동 속도
GLfloat MaxBongMove = 1.6f; // 최대 이동 거리
GLfloat obstacleRotation = 0.0f;
GLfloat DoorMove = 0.05f;
GLfloat MaxDoorMove = 1.7f;
GLfloat jumpBarRotationAngle = 0.0f;
//

glm::mat4 LeftdoorModelMatrix = glm::mat4(1.0f);
glm::mat4 RightdoorModelMatrix = glm::mat4(1.0f);
glm::vec3 LeftdoorGroupPosition = glm::vec3(0.0f, 0.0f, 0.0f); // 초기 위치
glm::vec3 LeftdoorGroupDirection = glm::vec3(-1.0f, 0.0f, 0.0f); // 초기 이동 방향 (왼쪽)
glm::vec3 RightdoorGroupPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 RightdoorGroupDirection = glm::vec3(1.0f, 0.0f, 0.0f);


bool moveKeyStates[256] = { false }; // 이동 키 상태
bool arrowKeyStates[256] = { false };
bool commandKeyStates[256] = { false }; // 명령 키 상태
bool isObstacleRotate = true;


bool checkCollision(const AABB& box1, const AABB& box2) {
	return (box1.max.x > box2.min.x && box1.min.x < box2.max.x &&
		box1.max.y > box2.min.y && box1.min.y < box2.max.y &&
		box1.max.z > box2.min.z && box1.min.z < box2.max.z);
}

char* filetobuf(const char* file) {
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

void make_vertexShaders();
void make_fragmentShaders();
//void InitBuffer();

// 맵
void InitBottom();
void InitArrowAndPillar();
void InitEndPoint();
void InitPoint();

//void InitBong2();

// 가로팬
//void InitHorizontalFanPink();
void InitHorizontalFanPurple();
// 점프바
void InitJumpbarCenter();
void InitJumpbarbargroup1();
void InitJumpbarbargroup2();
// 세로팬
void InitVerticalFanBar();
void InitVerticalFanCenter();
void InitVerticalFan();

GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void SpecialKey(int key, int x, int y);
void SpecialKeyUp(int key, int x, int y);
GLvoid Timer(int value);

int window_Width = 800;
int window_Height = 600;

// 맵
void InitBottom() {
	InitPart("Map/bottom.obj", modelBottom, vaoBottom, vboBottom, glm::vec3(0.482f, 0.424f, 0.761f));
}
void InitArrowAndPillar() {
	InitPart("Map/arrowAndPillar.obj", modelArrowAndPillar, vaoArrowAndPillar, vboArrowAndPillar, glm::vec3(0.49f, 0.0f, 0.871f));
}
void InitEndPoint() {
	InitPart("Map/endPoint.obj", modelEndPoint, vaoEndPoint, vboEndPoint, glm::vec3(1.0f, 0.0f, 1.0f));
}
void InitPoint() {
	InitPart("Map/point.obj", modelPoint, vaoPoint, vboPoint, glm::vec3(1.0f, 0.0f, 0.0f));
}
// 맵
AABB map1 = {
	glm::vec3(-22.5f, 0.0f, -80.0f), // min
	glm::vec3(22.5f,  0.3f,  3.0f)   // max
};
AABB map2 = {
	glm::vec3(-18.0f, -2.3f, -121.0f), // min
	glm::vec3(18.0f, -0.2f, -79.0f)    // max
};
AABB map3 = {
	glm::vec3(-14.0f, -2.0f, -146.0f), // min
	glm::vec3(14.0f, -0.3f, -120.0f)   // max
};
AABB map4 = {
	glm::vec3(-11.6f, -2.0f, -165.0f), // min
	glm::vec3(11.6f, -0.4f, -143.0f)   // max
};
AABB map5 = {
	glm::vec3(-10.6f, -28.5f, -245.0f), // min
	glm::vec3(10.6f, -26.5f, -165.0f)   // max
};


// 점프바
void InitJumpbarCenter() {
	InitPart("jumpBong/centergroup.obj", modelJumpBarCenter, vaoJumpBarCenter, vboJumpBarCenter, glm::vec3(0.576f, 0.078f, 1.0f));
}
void InitJumpbarbargroup1() {
	InitPart("jumpBong/bar1.obj", modelJumpBarbargroup1, vaoJumpBarbargroup1, vboJumpBarbargroup1, glm::vec3(0.576f, 0.078f, 1.0f));
}
void InitJumpbarbargroup2() {
	InitPart("jumpBong/bar2.obj", modelJumpBarbargroup2, vaoJumpBarbargroup2, vboJumpBarbargroup2, glm::vec3(0.576f, 0.078f, 1.0f));
}
void InitJumpbarbar3() {
	InitPart("jumpBong/bar3.obj", modelJumpBarbargroup3, vaoJumpBarbargroup3, vboJumpBarbargroup3, glm::vec3(0.576f, 0.078f, 1.0f));
}
// 세로팬
void InitVerticalFanBar() {
	InitPart("verticalFan/bar.obj", modelVerticalFanBar, vaoVerticalFanBar, vboVerticalFanBar, glm::vec3(0.5f, 0.5f, 0.5f));
}
void InitVerticalFanCenter() {
	InitPart("verticalFan/center.obj", modelVerticalFanCenter, vaoVerticalFanCenter, vboVerticalFanCenter, glm::vec3(1.0f, 0.4f, 0.7f));
}
void InitVerticalFan() {
	InitPart("verticalFan/fan.obj", modelVerticalFan, vaoVerticalFan, vboVerticalFan, glm::vec3(1.0f, 0.4f, 0.7f));
}



// 문
AABB leftdoor1 = {
	glm::vec3(-8.475f, -0.76f, -159.129f), // min
	glm::vec3(-6.4f,  2.4f,  -158.53f)   // max
};
AABB leftdoor2 = {
	glm::vec3(-2.168f, -0.76f, -159.129f), // min
	glm::vec3(-0.09f,  2.4f,  -158.53f)   // max
};
AABB leftdoor3 = {
	glm::vec3(4.227f, -0.76f, -159.129f), // min
	glm::vec3(6.297f,  2.4f,  -158.53f)   // max
};
AABB rightdoor1 = {
	glm::vec3(6.408f, -0.76f, -159.129f), // min
	glm::vec3(4.38f,  2.4f,  -158.53f)   // max
};
AABB rightdoor2 = {
	glm::vec3(-0.1f, -0.76f, -159.129f), // min
	glm::vec3(1.926f,  2.4f,  -158.53f)   // max
};
AABB rightdoor3 = {
	glm::vec3(6.294f, -0.76f, -159.129f), // min
	glm::vec3(8.322f,  2.4f,  -158.53f)   // max
};
AABB outdoor1 = {
	glm::vec3(-9.546f, -0.6f,-160.437f), // min
	glm::vec3(-8.346f,  2.6, -158.4f)   // max
};
AABB outdoor2 = {
	glm::vec3(-4.344f, -0.6f,-160.437f), // min
	glm::vec3(-2.144f,  2.6, -158.4f)   // max
};
AABB outdoor3 = {
	glm::vec3(2.004f, -0.6f,-160.437f), // min
	glm::vec3(4.304f,  2.6, -158.4f)   // max
};
AABB outdoor4 = {
	glm::vec3(8.295f, -0.6f,-160.437f), // min
	glm::vec3(9.495f,  2.6, -158.4f)   // max
};
//점프바
AABB barcenter1 = {
	glm::vec3(-10.2f, 0.0f, -94.93f), // min
	glm::vec3(-9.0f, 0.76f,  -93.73f)   // max
};
AABB barcenter2 = {
	glm::vec3(-0.29f, 0.0f, -94.93f), // min
	glm::vec3(0.9f, 0.76f,  -93.73f)   // max
};
AABB barcenter3 = {
	glm::vec3(10.03f, 0.0f, -94.93f), // min
	glm::vec3(11.23f, 0.76f,  -93.73f)   // max
};
AABB barbar1 = {
	glm::vec3(-9.8f, -0.36f, -94.457f), // min
	glm::vec3(-9.4f,0.0399f,  -88.457f)   // max
};
AABB barbar2 = {
	glm::vec3(0.155f, -0.36f, -94.457f), // min
	glm::vec3(0.555f,0.0399f,  -88.457f)   // max
};
AABB barbar3 = {
	glm::vec3(10.43f, -0.36f, -94.457f), // min
	glm::vec3(10.83f,0.0399f,  -88.457f)   // max
};
//// 가로팬
//AABB horizontalFan1 = {
//	glm::vec3(-6.1f, -0.3f, -140.49f),  // min
//	glm::vec3(6.1f, 4.1f, -139.51f)     // max
//};
//AABB horizontalFan2 = {
//	glm::vec3(0.9f, -0.3f, -115.49f),   // min
//	glm::vec3(13.1f, 4.1f, -114.51f)    // max
//};
//AABB horizontalFan3 = {
//	glm::vec3(-13.1f, -0.3f, -115.49f), // min
//	glm::vec3(-0.9f, 4.1f, -114.51f)    // max
//};
// 세로팬
AABB leftBar1 = {
	glm::vec3(-18.12f, -0.64f, -62.86f),
	glm::vec3(-16.98f, 3.65f, -61.72f)
};
AABB leftBar2 = {
	glm::vec3(-10.62f, -0.64f, -62.86f),
	glm::vec3(-9.48f, 3.65f, -61.72f)
};
AABB leftBar3 = {
	glm::vec3(-3.12f, -0.64f, -62.86f),
	glm::vec3(-1.98f, 3.65f, -61.72f)
};
AABB leftBar4 = {
	glm::vec3(4.38f, -0.64f, -62.86f),
	glm::vec3(5.52f, 3.65f, -61.72f)
};
AABB leftBar5 = {
	glm::vec3(11.88f, -0.64f, -62.86f),
	glm::vec3(13.02f, 3.65f, -61.72f)
};
AABB middleBar1 = {
	glm::vec3(-17.17f, 3.51f, -62.86f),
	glm::vec3(-12.89f, 3.64f, -61.72f)
};
AABB middleBar2 = {
	glm::vec3(-9.67f, 3.51f, -62.86f),
	glm::vec3(-5.39f, 3.64f, -61.72f)
};
AABB middleBar3 = {
	glm::vec3(-2.17f, 3.51f, -62.86f),
	glm::vec3(2.11f, 3.64f, -61.72f)
};
AABB middleBar4 = {
	glm::vec3(5.33f, 3.51f, -62.86f),
	glm::vec3(9.61f, 3.64f, -61.72f)
};
AABB middleBar5 = {
	glm::vec3(12.83f, 3.51f, -62.86f),
	glm::vec3(17.11f, 3.64f, -61.72f)
};
AABB rightBar1 = {
	glm::vec3(-13.07f, -0.64f, -62.86f),
	glm::vec3(-11.93f, 3.65f, -61.72f)
};
AABB rightBar2 = {
	glm::vec3(-5.57f, -0.64f, -62.86f),
	glm::vec3(-4.43f, 3.65f, -61.72f)
};
AABB rightBar3 = {
	glm::vec3(1.93f, -0.64f, -62.86f),
	glm::vec3(3.07f, 3.65f, -61.72f)
};
AABB rightBar4 = {
	glm::vec3(9.43f, -0.64f, -62.86f),
	glm::vec3(10.57f, 3.65f, -61.72f)
};
AABB rightBar5 = {
	glm::vec3(16.93f, -0.64f, -62.86f),
	glm::vec3(18.07f, 3.65f, -61.72f)
};
AABB verticalFan1 = {
	glm::vec3(-17.33f, -0.39f, -60.46f),
	glm::vec3(-12.67f, 6.39f, -59.54f)
};
AABB verticalFan2 = {
	glm::vec3(-9.83f, -0.39f, -60.46f),
	glm::vec3(-5.17f, 6.39f, -59.54f)
};
AABB verticalFan3 = {
	glm::vec3(-2.33f, -0.39f, -60.46f),
	glm::vec3(2.33f, 6.39f, -59.54f)
};
AABB verticalFan4 = {
	glm::vec3(5.17f, -0.39f, -60.46f),
	glm::vec3(9.83f, 6.39f, -59.54f)
};
AABB verticalFan5 = {
	glm::vec3(12.67f, -0.39f, -60.46f),
	glm::vec3(17.33f, 6.39f, -59.54f)
};


// 맵 충돌박스
std::vector<float> CheckBoxVerticesMap1 = {
	// Bottom
	-22.5f, -2.0f,  0.0f,
	-22.5f,  0.0f,  0.0f,
	-22.5f, -2.0f, -80.0f,
	-22.5f,  0.0f, -80.0f,

	// Top
	22.5f, -2.0f,  0.0f,
	22.5f,  0.0f,  0.0f,
	22.5f, -2.0f, -80.0f,
	22.5f,  0.0f, -80.0f,

	// Front
	-22.5f, -2.0f, -80.0f,
	22.5f, -2.0f, -80.0f,
	22.5f,  0.0f, -80.0f,
	-22.5f,  0.0f, -80.0f,

	// Back face
	-22.5f, -2.0f,  0.0f,
	22.5f, -2.0f,  0.0f,
	22.5f,  0.0f,  0.0f,
	-22.5f,  0.0f,  0.0f,

	// Left face
	-22.5f, -2.0f,  0.0f,
	-22.5f, -2.0f, -80.0f,
	-22.5f,  0.0f, -80.0f,
	-22.5f,  0.0f,  0.0f,

	// Right face
	22.5f, -2.0f,  0.0f,
	22.5f, -2.0f, -80.0f,
	22.5f,  0.0f, -80.0f,
	22.5f,  0.0f,  0.0f
};
std::vector<float> CheckBoxVerticesMap2 = {
	// Bottom
	-18.0f, -2.3f, -79.0f,
	-18.0f, -0.3f, -79.0f,
	-18.0f, -2.3f, -121.0f,
	-18.0f, -0.3f, -121.0f,

	// Top
	18.0f, -2.3f, -79.0f,
	18.0f, -0.3f, -79.0f,
	18.0f, -2.3f, -121.0f,
	18.0f, -0.3f, -121.0f,

	// Front
	-18.0f, -2.3f, -121.0f,
	18.0f, -2.3f, -121.0f,
	18.0f, -0.3f, -121.0f,
	-18.0f, -0.3f, -121.0f,

	// Back face
	-18.0f, -2.3f, -79.0f,
	18.0f, -2.3f, -79.0f,
	18.0f, -0.3f, -79.0f,
	-18.0f, -0.3f, -79.0f,

	// Left face
	-18.0f, -2.3f, -79.0f,
	-18.0f, -2.3f, -121.0f,
	-18.0f, -0.3f, -121.0f,
	-18.0f, -0.3f, -79.0f,

	// Right face
	18.0f, -2.3f, -79.0f,
	18.0f, -2.3f, -121.0f,
	18.0f, -0.3f, -121.0f,
	18.0f, -0.3f, -79.0f
};
std::vector<float> CheckBoxVerticesMap3 = {
	// Bottom
	-14.0f, -2.6f, -120.0f,
	-14.0f, -0.6f, -120.0f,
	-14.0f, -2.6f, -146.0f,
	-14.0f, -0.6f, -146.0f,

	// Top
	14.0f, -2.6f, -120.0f,
	14.0f, -0.6f, -120.0f,
	14.0f, -2.6f, -146.0f,
	14.0f, -0.6f, -146.0f,

	// Front
	-14.0f, -2.6f, -146.0f,
	14.0f, -2.6f, -146.0f,
	14.0f, -0.6f, -146.0f,
	-14.0f, -0.6f, -146.0f,

	// Back face
	-14.0f, -2.6f, -120.0f,
	14.0f, -2.6f, -120.0f,
	14.0f, -0.6f, -120.0f,
	-14.0f, -0.6f, -120.0f,

	// Left face
	-14.0f, -2.6f, -120.0f,
	-14.0f, -2.6f, -146.0f,
	-14.0f, -0.6f, -146.0f,
	-14.0f, -0.6f, -120.0f,

	// Right face
	14.0f, -2.6f, -120.0f,
	14.0f, -2.6f, -146.0f,
	14.0f, -0.6f, -146.0f,
	14.0f, -0.6f, -120.0f
};
std::vector<float> CheckBoxVerticesMap4 = {
	// Bottom
	-11.6f, -2.8f, -143.0f,
	-11.6f, -0.8f, -143.0f,
	-11.6f, -2.8f, -165.0f,
	-11.6f, -0.8f, -165.0f,

	// Top
	11.6f, -2.8f, -143.0f,
	11.6f, -0.8f, -143.0f,
	11.6f, -2.8f, -165.0f,
	11.6f, -0.8f, -165.0f,

	// Front
	-11.6f, -2.8f, -165.0f,
	11.6f, -2.8f, -165.0f,
	11.6f, -0.8f, -165.0f,
	-11.6f, -0.8f, -165.0f,

	// Back face
	-11.6f, -2.8f, -143.0f,
	11.6f, -2.8f, -143.0f,
	11.6f, -0.8f, -143.0f,
	-11.6f, -0.8f, -143.0f,

	// Left face
	-11.6f, -2.8f, -143.0f,
	-11.6f, -2.8f, -165.0f,
	-11.6f, -0.8f, -165.0f,
	-11.6f, -0.8f, -143.0f,

	// Right face
	11.6f, -2.8f, -143.0f,
	11.6f, -2.8f, -165.0f,
	11.6f, -0.8f, -165.0f,
	11.6f, -0.8f, -143.0f
};
std::vector<float> CheckBoxVerticesMap5 = {
	// Bottom
	 -10.6f, -28.5f, -165.0f,
	 -10.6f, -26.5f, -165.0f,
	 -10.6f, -28.5f, -245.0f,
	 -10.6f, -26.5f, -245.0f,

	 // Top
	 10.6f, -28.5f, -165.0f,
	 10.6f, -26.5f, -165.0f,
	 10.6f, -28.5f, -245.0f,
	 10.6f, -26.5f, -245.0f,

	 // Front
	 -10.6f, -28.5f, -245.0f,
	 10.6f, -28.5f, -245.0f,
	 10.6f, -26.5f, -245.0f,
	 -10.6f, -26.5f, -245.0f,

	 // Back face
	 -10.6f, -28.5f, -165.0f,
	 10.6f, -28.5f, -165.0f,
	 10.6f, -26.5f, -165.0f,
	 -10.6f, -26.5f, -165.0f,

	 // Left face
	 -10.6f, -28.5f, -165.0f,
	 -10.6f, -28.5f, -245.0f,
	 -10.6f, -26.5f, -245.0f,
	 -10.6f, -26.5f, -165.0f,

	 // Right face
	 10.6f, -28.5f, -165.0f,
	 10.6f, -28.5f, -245.0f,
	 10.6f, -26.5f, -245.0f,
	 10.6f, -26.5f, -165.0f
};

void InitCheckBoxMap1() {
	glGenVertexArrays(1, &vaoCheckBoxMap1);
	glBindVertexArray(vaoCheckBoxMap1);

	glGenBuffers(1, vboCheckBoxMap1);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap1[0]);
	glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap1.size() * sizeof(float), CheckBoxVerticesMap1.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
void InitCheckBoxMap2() {
	glGenVertexArrays(1, &vaoCheckBoxMap2);
	glBindVertexArray(vaoCheckBoxMap2);

	glGenBuffers(1, vboCheckBoxMap2);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap2[0]);
	glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap2.size() * sizeof(float), CheckBoxVerticesMap2.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
void InitCheckBoxMap3() {
	glGenVertexArrays(1, &vaoCheckBoxMap3);
	glBindVertexArray(vaoCheckBoxMap3);

	glGenBuffers(1, vboCheckBoxMap3);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap3[0]);
	glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap3.size() * sizeof(float), CheckBoxVerticesMap3.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
void InitCheckBoxMap4() {
	glGenVertexArrays(1, &vaoCheckBoxMap4);
	glBindVertexArray(vaoCheckBoxMap4);

	glGenBuffers(1, vboCheckBoxMap4);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap4[0]);
	glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap4.size() * sizeof(float), CheckBoxVerticesMap4.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
void InitCheckBoxMap5() {
	glGenVertexArrays(1, &vaoCheckBoxMap5);
	glBindVertexArray(vaoCheckBoxMap5);

	glGenBuffers(1, vboCheckBoxMap5);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap5[0]);
	glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap5.size() * sizeof(float), CheckBoxVerticesMap5.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

// 맵 그리기
void DrawMap(GLuint shaderPRogramID, GLint modelMatrixLocation) {
	// 바닥
	glm::mat4 bottomModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(bottomModelMatrix));

	glBindVertexArray(vaoBottom);
	glDrawElements(GL_TRIANGLES, modelBottom.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 화살표와 기둥
	glm::mat4 arrowAndPillarModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(arrowAndPillarModelMatrix));

	glBindVertexArray(vaoArrowAndPillar);
	glDrawElements(GL_TRIANGLES, modelArrowAndPillar.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 결승점
	glm::mat4 endPointModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(endPointModelMatrix));

	glBindVertexArray(vaoEndPoint);
	glDrawElements(GL_TRIANGLES, modelEndPoint.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 포인트
	glm::mat4 pointModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(pointModelMatrix));

	glBindVertexArray(vaoPoint);
	glDrawElements(GL_TRIANGLES, modelPoint.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// 체크박스 그리기
void DrawMapCheckBox(GLuint shaderProgramID, GLint modelMatrixLocation) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 checkBoxModelMatrix1 = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix1));
	glBindVertexArray(vaoCheckBoxMap1);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);

	glm::mat4 checkBoxModelMatrix2 = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix2));
	glBindVertexArray(vaoCheckBoxMap2);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);

	glm::mat4 checkBoxModelMatrix3 = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix3));
	glBindVertexArray(vaoCheckBoxMap3);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);

	glm::mat4 checkBoxModelMatrix4 = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix4));
	glBindVertexArray(vaoCheckBoxMap4);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);

	glm::mat4 checkBoxModelMatrix5 = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix5));
	glBindVertexArray(vaoCheckBoxMap5);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// 장애물 그리기
void DrawObstacleBong(GLuint shaderPRogramID, GLint modelMatrixLocation) {
}
void DrawBongCheckBoxes(GLuint shaderProgramID, GLint modelMatrixLocation) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void DrawObstacleHorizontalFan(GLuint shaderPRogramID, GLint modelMatrixLocation) {
	glm::vec3 horizontalFan1Position = glm::vec3(0.0f, -0.3f, -140.0f);
	glm::mat4 HorizontalFanPink1ModelMatrix = glm::mat4(1.0f);
	HorizontalFanPink1ModelMatrix = glm::translate(HorizontalFanPink1ModelMatrix, horizontalFan1Position);
	HorizontalFanPink1ModelMatrix = glm::rotate(HorizontalFanPink1ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPink1ModelMatrix));

	glBindVertexArray(vaoHorizontalFanPink);
	glDrawElements(GL_TRIANGLES, modelHorizontalFanPink.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 HorizontalFanPurple1ModelMatrix = glm::mat4(1.0f);
	HorizontalFanPurple1ModelMatrix = glm::translate(HorizontalFanPurple1ModelMatrix, horizontalFan1Position);
	HorizontalFanPurple1ModelMatrix = glm::rotate(HorizontalFanPurple1ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPink1ModelMatrix));

	glBindVertexArray(vaoHorizontalFanPurple);
	glDrawElements(GL_TRIANGLES, modelHorizontalFanPurple.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
void DrawObstacleDoor(GLuint shaderPRogramID, GLint modelMatrixLocation) {
	glm::mat4 DooroutModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(DooroutModelMatrix));

	glBindVertexArray(vaoDoorOut);
	glDrawElements(GL_TRIANGLES, modelDoorOut.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 finalLeftdoorModelMatrix = LeftdoorModelMatrix;
	LeftdoorModelMatrix = glm::translate(glm::mat4(1.0f), LeftdoorGroupPosition);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalLeftdoorModelMatrix));

	glBindVertexArray(vaoLeftdoor);
	glDrawElements(GL_TRIANGLES, modelLeftdoor.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 finalRightdoorModelMatrix = RightdoorModelMatrix;
	RightdoorModelMatrix = glm::translate(glm::mat4(1.0f), RightdoorGroupPosition);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalRightdoorModelMatrix));

	glBindVertexArray(vaoRightdoor);
	glDrawElements(GL_TRIANGLES, modelRightdoor.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void DrawObstacleJumpbar(GLuint shaderPRogramID, GLint modelMatrixLocation) {
	glm::mat4 JumpBarCenterModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarCenterModelMatrix));

	glBindVertexArray(vaoJumpBarCenter);
	glDrawElements(GL_TRIANGLES, modelJumpBarCenter.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 첫 번째 바 회전
	glm::mat4 JumpBarbargroup1ModelMatrix = glm::mat4(1.0f);
	JumpBarbargroup1ModelMatrix = glm::translate(JumpBarbargroup1ModelMatrix, glm::vec3(-9.5f, 0.0f, -94.93f)); // 센터로 이동
	JumpBarbargroup1ModelMatrix = glm::rotate(JumpBarbargroup1ModelMatrix, glm::radians(jumpBarRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Y축 회전
	JumpBarbargroup1ModelMatrix = glm::translate(JumpBarbargroup1ModelMatrix, glm::vec3(9.5f, 0.0f, 94.93f)); // 원래 위치로 이동
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarbargroup1ModelMatrix));

	glBindVertexArray(vaoJumpBarbargroup1);
	glDrawElements(GL_TRIANGLES, modelJumpBarbargroup1.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 JumpBarbargroup2ModelMatrix = glm::mat4(1.0f);
	JumpBarbargroup2ModelMatrix = glm::translate(JumpBarbargroup2ModelMatrix, glm::vec3(0.6f, 0.0f, -94.93f)); // 센터로 이동
	JumpBarbargroup2ModelMatrix = glm::rotate(JumpBarbargroup2ModelMatrix, glm::radians(-jumpBarRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // 반대 방향 회전
	JumpBarbargroup2ModelMatrix = glm::translate(JumpBarbargroup2ModelMatrix, glm::vec3(-0.6f, 0.0f, 94.93f)); // 원래 위치로 이동
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarbargroup2ModelMatrix));

	glBindVertexArray(vaoJumpBarbargroup2);
	glDrawElements(GL_TRIANGLES, modelJumpBarbargroup2.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 JumpBarbargroup3ModelMatrix = glm::mat4(1.0f);
	JumpBarbargroup3ModelMatrix = glm::translate(JumpBarbargroup3ModelMatrix, glm::vec3(10.5f, 0.0f, -94.93f)); // 센터로 이동
	JumpBarbargroup3ModelMatrix = glm::rotate(JumpBarbargroup3ModelMatrix, glm::radians(jumpBarRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // 반대 방향 회전
	JumpBarbargroup3ModelMatrix = glm::translate(JumpBarbargroup3ModelMatrix, glm::vec3(-10.5f, 0.0f, 94.93f)); // 원래 위치로 이동
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarbargroup3ModelMatrix));

	glBindVertexArray(vaoJumpBarbargroup3);
	glDrawElements(GL_TRIANGLES, modelJumpBarbargroup3.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void DrawObstacleVerticalFan(GLuint shaderPRogramID, GLint modelMatrixLocation) {
	glm::vec3 verticalFan1Position = glm::vec3(0.0f, 3.0f, -60.0f);
	glm::mat4 verticalFanBar1ModelMatrix = glm::mat4(1.0f);
	verticalFanBar1ModelMatrix = glm::translate(verticalFanBar1ModelMatrix, verticalFan1Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanBar1ModelMatrix));

	glBindVertexArray(vaoVerticalFanBar);
	glDrawElements(GL_TRIANGLES, modelVerticalFanBar.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFanCenter1ModelMatrix = glm::mat4(1.0f);
	verticalFanCenter1ModelMatrix = glm::translate(verticalFanCenter1ModelMatrix, verticalFan1Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanCenter1ModelMatrix));

	glBindVertexArray(vaoVerticalFanCenter);
	glDrawElements(GL_TRIANGLES, modelVerticalFanCenter.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFan1ModelMatrix = glm::mat4(1.0f);
	verticalFan1ModelMatrix = glm::translate(verticalFan1ModelMatrix, verticalFan1Position);
	verticalFan1ModelMatrix = glm::rotate(verticalFan1ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFan1ModelMatrix));

	glBindVertexArray(vaoVerticalFan);
	glDrawElements(GL_TRIANGLES, modelVerticalFan.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	verticalFan1.update(verticalFan1Position, glm::vec3(-2.33f, -3.39f, -0.46f), glm::vec3(2.33f, 3.39f, 0.46f));

	glm::vec3 verticalFan2Position = glm::vec3(-15.0f, 3.0f, -60.0f);
	glm::mat4 verticalFanBar2ModelMatrix = glm::mat4(1.0f);
	verticalFanBar2ModelMatrix = glm::translate(verticalFanBar2ModelMatrix, verticalFan2Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanBar2ModelMatrix));

	glBindVertexArray(vaoVerticalFanBar);
	glDrawElements(GL_TRIANGLES, modelVerticalFanBar.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFanCenter2ModelMatrix = glm::mat4(1.0f);
	verticalFanCenter2ModelMatrix = glm::translate(verticalFanCenter2ModelMatrix, verticalFan2Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanCenter2ModelMatrix));

	glBindVertexArray(vaoVerticalFanCenter);
	glDrawElements(GL_TRIANGLES, modelVerticalFanCenter.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFan2ModelMatrix = glm::mat4(1.0f);
	verticalFan2ModelMatrix = glm::translate(verticalFan2ModelMatrix, verticalFan2Position);
	verticalFan2ModelMatrix = glm::rotate(verticalFan2ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFan2ModelMatrix));

	glBindVertexArray(vaoVerticalFan);
	glDrawElements(GL_TRIANGLES, modelVerticalFan.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	verticalFan2.update(verticalFan2Position, glm::vec3(-2.33f, -3.39f, -0.46f), glm::vec3(2.33f, 3.39f, 0.46f));

	glm::vec3 verticalFan3Position = glm::vec3(15.0f, 3.0f, -60.0f);
	glm::mat4 verticalFanBar3ModelMatrix = glm::mat4(1.0f);
	verticalFanBar3ModelMatrix = glm::translate(verticalFanBar3ModelMatrix, verticalFan3Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanBar3ModelMatrix));

	glBindVertexArray(vaoVerticalFanBar);
	glDrawElements(GL_TRIANGLES, modelVerticalFanBar.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFanCenter3ModelMatrix = glm::mat4(1.0f);
	verticalFanCenter3ModelMatrix = glm::translate(verticalFanCenter3ModelMatrix, verticalFan3Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanCenter3ModelMatrix));

	glBindVertexArray(vaoVerticalFanCenter);
	glDrawElements(GL_TRIANGLES, modelVerticalFanCenter.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFan3ModelMatrix = glm::mat4(1.0f);
	verticalFan3ModelMatrix = glm::translate(verticalFan3ModelMatrix, verticalFan3Position);
	verticalFan3ModelMatrix = glm::rotate(verticalFan3ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFan3ModelMatrix));

	glBindVertexArray(vaoVerticalFan);
	glDrawElements(GL_TRIANGLES, modelVerticalFan.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	verticalFan3.update(verticalFan3Position, glm::vec3(-2.33f, -3.39f, -0.46f), glm::vec3(2.33f, 3.39f, 0.46f));

	glm::vec3 verticalFan4Position = glm::vec3(-7.5f, 3.0f, -60.0f);
	glm::mat4 verticalFanBar4ModelMatrix = glm::mat4(1.0f);
	verticalFanBar4ModelMatrix = glm::translate(verticalFanBar4ModelMatrix, verticalFan4Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanBar4ModelMatrix));

	glBindVertexArray(vaoVerticalFanBar);
	glDrawElements(GL_TRIANGLES, modelVerticalFanBar.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFanCenter4ModelMatrix = glm::mat4(1.0f);
	verticalFanCenter4ModelMatrix = glm::translate(verticalFanCenter4ModelMatrix, verticalFan4Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanCenter4ModelMatrix));

	glBindVertexArray(vaoVerticalFanCenter);
	glDrawElements(GL_TRIANGLES, modelVerticalFanCenter.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFan4ModelMatrix = glm::mat4(1.0f);
	verticalFan4ModelMatrix = glm::translate(verticalFan4ModelMatrix, verticalFan4Position);
	verticalFan4ModelMatrix = glm::rotate(verticalFan4ModelMatrix, glm::radians(-obstacleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFan4ModelMatrix));

	glBindVertexArray(vaoVerticalFan);
	glDrawElements(GL_TRIANGLES, modelVerticalFan.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	verticalFan4.update(verticalFan4Position, glm::vec3(-2.33f, -3.39f, -0.46f), glm::vec3(2.33f, 3.39f, 0.46f));

	glm::vec3 verticalFan5Position = glm::vec3(7.5f, 3.0f, -60.0f);
	glm::mat4 verticalFanBar5ModelMatrix = glm::mat4(1.0f);
	verticalFanBar5ModelMatrix = glm::translate(verticalFanBar5ModelMatrix, verticalFan5Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanBar5ModelMatrix));

	glBindVertexArray(vaoVerticalFanBar);
	glDrawElements(GL_TRIANGLES, modelVerticalFanBar.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFanCenter5ModelMatrix = glm::mat4(1.0f);
	verticalFanCenter5ModelMatrix = glm::translate(verticalFanCenter5ModelMatrix, verticalFan5Position);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFanCenter5ModelMatrix));

	glBindVertexArray(vaoVerticalFanCenter);
	glDrawElements(GL_TRIANGLES, modelVerticalFanCenter.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glm::mat4 verticalFan5ModelMatrix = glm::mat4(1.0f);
	verticalFan5ModelMatrix = glm::translate(verticalFan5ModelMatrix, verticalFan5Position);
	verticalFan5ModelMatrix = glm::rotate(verticalFan5ModelMatrix, glm::radians(-obstacleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(verticalFan5ModelMatrix));

	glBindVertexArray(vaoVerticalFan);
	glDrawElements(GL_TRIANGLES, modelVerticalFan.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	verticalFan5.update(verticalFan5Position, glm::vec3(-2.33f, -3.39f, -0.46f), glm::vec3(2.33f, 3.39f, 0.46f));
}

Player1* P1 = nullptr;
BongGroup* Bong1 = nullptr;
BongGroup* Bong2 = nullptr;
HorizontalFan* PinkFan1 = nullptr;
HorizontalFan* PurpleFan1 = nullptr;

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_Width, window_Height);
	glutCreateWindow("team_project");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();

	// 맵
	InitBottom();
	InitArrowAndPillar();
	InitEndPoint();
	InitPoint();

	//mapcheckbox
	InitCheckBoxMap1();
	InitCheckBoxMap2();
	InitCheckBoxMap3();
	InitCheckBoxMap4();
	InitCheckBoxMap5();

	P1 = new Player1();


	//장애물

	AABB bong1 = {
		glm::vec3(-15.74f , 0.0f, -33.25f), // min
		glm::vec3(-13.74f,  3.6f,  -31.25f)  // max
	};
	Bong1 = new BongGroup(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.1f, 1.6f, 0.0f);
	InitPart("bong/bonggroup1.obj", Bong1->model, Bong1->vao, Bong1->vbo, glm::vec3(1.0f, 0.078f, 0.576f));
	Bong1->SetAABB(bong1);

	AABB bong2 = {
	glm::vec3(-9.47f, 0.0f, -33.25f), // min
	glm::vec3(-7.47f ,  3.6f,  -31.25f)  // max
	};
	Bong2 = new BongGroup(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 0.1f, 1.6f, 0.0f);
	InitPart("bong/bonggroup2.obj", Bong2->model, Bong2->vao, Bong2->vbo, glm::vec3(1.0f, 0.078f, 0.576f));
	Bong2->SetAABB(bong2);

	// 가로팬
	AABB horizontalFan1 = {
		glm::vec3(-6.1f, -0.3f, -140.49f),  // min
		glm::vec3(6.1f, 4.1f, -139.51f)     // max
	};
	AABB horizontalFan2 = {
		glm::vec3(0.9f, -0.3f, -115.49f),   // min
		glm::vec3(13.1f, 4.1f, -114.51f)    // max
	};
	AABB horizontalFan3 = {
		glm::vec3(-13.1f, -0.3f, -115.49f), // min
		glm::vec3(-0.9f, 4.1f, -114.51f)    // max
	};

	PinkFan1 = new HorizontalFan(glm::vec3(0.0f, -0.3f, -140.0f));
	InitPart("horizontalFan/pink.obj", PinkFan1->model, PinkFan1->vao, PinkFan1->vbo, glm::vec3(1.0f, 0.7f, 0.75f));
	PinkFan1->SetAABB(horizontalFan1);

	PurpleFan1 = new HorizontalFan(PinkFan1->Position);
	InitPart("horizontalFan/purple.obj", PurpleFan1->model, PurpleFan1->vao, PurpleFan1->vbo, glm::vec3(0.5f, 0.0f, 0.5f));


	//InitBong1();

	//InitBong2();
	//InitAllBongCheckBoxes();

	//InitHorizontalFanPink();
	//InitHorizontalFanPurple();
	//InitDoorOut();
	//InitDoorLeft();
	//InitDoorRight();
	InitJumpbarCenter();
	InitJumpbarbargroup1();
	InitJumpbarbargroup2();
	InitJumpbarbar3();
	InitVerticalFanBar();
	InitVerticalFanCenter();
	InitVerticalFan();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);
	glutTimerFunc(16, Timer, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	glutMainLoop();
}

// 캐릭터


void make_vertexShaders() {
	GLchar* vertexSource;

	vertexSource = filetobuf("vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShaders() {
	GLchar* fragmentSource;

	fragmentSource = filetobuf("fragment.glsl");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
GLuint make_shaderProgram() {
	make_vertexShaders();
	make_fragmentShaders();

	shaderProgramID = glCreateProgram();

	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgramID);
	return shaderProgramID;
}

GLvoid drawScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0f);  // 배경을 흰색으로 설정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	// **뷰포트 1: 왼쪽 (캐릭터 1의 카메라)**
	glViewport(0, 0, window_Width, window_Height); //전체화면
	glm::vec3 camera1Position = P1->Position + glm::vec3(0.0f, 10.0f, 15.0f);
	glm::vec3 camera1Target = P1->Position;

	glm::mat4 viewMatrix1 = glm::lookAt(
		camera1Position,  // 카메라 1 위치
		camera1Target,    // 카메라 1 바라보는 지점
		glm::vec3(0.0f, 1.0f, 0.0f) // 상향 벡터
	);

	GLint viewMatrixLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix1));

	glm::mat4 projectionMatrix1 = glm::perspective(
		glm::radians(45.0f),
		(float)(window_Width ) / (float)window_Height, // 좌우 절반의 종횡비
		0.1f,
		10000.0f
	);
	GLint projMatrixLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix1));

	GLint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelTransform");

	if (P1->Position.y < -75.0f) {
		P1->Position = P1->InitialPosition;
	}

	DrawMap(shaderProgramID, modelMatrixLocation);
	//DrawObstacleBong(shaderProgramID, modelMatrixLocation);
	Bong1->Draw(shaderProgramID, modelMatrixLocation);
	Bong2->Draw(shaderProgramID, modelMatrixLocation);
	P1->Draw(shaderProgramID, modelMatrixLocation);
	//상대 캐릭터도 받아서 그려야함.
	PinkFan1->Draw(shaderProgramID, modelMatrixLocation);
	PurpleFan1->Draw(shaderProgramID, modelMatrixLocation);
	//DrawObstacleHorizontalFan(shaderProgramID, modelMatrixLocation);
	//DrawObstacleVerticalFan(shaderProgramID, modelMatrixLocation);
	//DrawObstacleDoor(shaderProgramID, modelMatrixLocation);
	//DrawObstacleJumpbar(shaderProgramID, modelMatrixLocation);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
		moveKeyStates[key] = true;
	}
	else {
		switch (key) {
		case 'q':
			glutLeaveMainLoop();
			break;
		case ' ':
			if (!P1->IsJumping) {
				P1->IsJumping = true;
			}
			break;
		}
	}
	glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
		moveKeyStates[key] = false;
	}
}

void SpecialKey(int key, int x, int y) {
	if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
		arrowKeyStates[key] = true;
	}
}

void SpecialKeyUp(int key, int x, int y) {
	if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
		arrowKeyStates[key] = false;
	}
}

GLvoid Timer(int value) {
	if (moveKeyStates['w']) {
		P1->Direction = glm::vec3(0.0f, 0.0f, -P1->MSpeed);
		P1->RotationAngle = 0.0f;
		P1->IsSwing = true;
	}
	else if (moveKeyStates['s']) {
		P1->Direction = glm::vec3(0.0f, 0.0f, P1->MSpeed);
		P1->RotationAngle = 180.0f;
		P1->IsSwing = true;
	}
	else if (moveKeyStates['a']) {
		P1->Direction = glm::vec3(-P1->MSpeed, 0.0f, 0.0f);
		P1->RotationAngle = 90.0f;
		P1->IsSwing = true;
	}
	else if (moveKeyStates['d']) {
		P1->Direction = glm::vec3(P1->MSpeed, 0.0f, 0.0f);
		P1->RotationAngle = -90.0f;
		P1->IsSwing = true;
	}
	else {
		P1->Direction = glm::vec3(0.0f, 0.0f, 0.0f);
		P1->IsSwing = false;
	}

	AABB maps[] = { map1, map2, map3, map4, map5 };
	P1->IsOnMap = false;
	for (const auto& map : maps) {
		if (checkCollision(P1->CAABB, map)) {
			P1->IsOnMap = true;
			break;
		}
	}

	// 점프 로직
	if (P1->IsJumping) {
		// character1Position.y += character1JumpSpeed;
		P1->Position += glm::vec3(0.0f, P1->JSpeed, 0.0f);
		P1->JSpeed -= P1->gravity;

		if (P1->JSpeed <= 0.0f && P1->IsOnMap) {
			P1->IsJumping = false;
			P1->JSpeed = 0.3f;
		}
	}
	else if (!P1->IsOnMap) {
		//character1Position.y -= realGravity;
		P1->Position = glm::vec3(0.0f, -P1->realGravity, 0.0f);
	}

	// 캐릭터1 모델 매트릭스 업데이트
	P1->ModelMatrix = glm::translate(glm::mat4(1.0f), P1->Position);
	P1->ModelMatrix = glm::rotate(P1->ModelMatrix, glm::radians(P1->RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	// AABB 업데이트
	P1->CAABB.update(P1->Position, glm::vec3(-0.7f, 0.0f, -0.72f), glm::vec3(0.7f, 1.84f, 0.63f));

	// 팔 흔들림 업데이트
	if (P1->IsSwing) {
		P1->ArmLegSwingAngle += P1->SwingDirection * 2.0f;
		if (P1->ArmLegSwingAngle >= P1->MaxSwingAngle) {
			P1->SwingDirection = -1; // 방향 반전
		}
		else if (P1->ArmLegSwingAngle <= -P1->MaxSwingAngle) {
			P1->SwingDirection = 1; // 방향 반전
		}
	}
	else {
		// 흔들림 비활성화 시 초기 상태로 복구
		if (P1->ArmLegSwingAngle > 0.0f) {
			P1->ArmLegSwingAngle -= 2.0f;
			if (P1->ArmLegSwingAngle < 0.0f) P1->ArmLegSwingAngle = 0.0f;
		}
		else if (P1->ArmLegSwingAngle < 0.0f) {
			P1->ArmLegSwingAngle += 2.0f;
			if (P1->ArmLegSwingAngle > 0.0f) P1->ArmLegSwingAngle = 0.0f;
		}
	}


	// 캐릭터 간 충돌 검사
   //if (checkCollision(character1, character2)) {
   //	float overlapX = std::min(character1.max.x, character2.max.x) - std::max(character1.min.x, character2.min.x);
   //	float overlapZ = std::min(character1.max.z, character2.max.z) - std::max(character1.min.z, character2.min.z);

   //	if (overlapX < overlapZ) {
   //		if (character1Direction.x > 0.0f && character1.max.x > character2.min.x) {
   //			character1Direction.x = 0.0f;
   //		}
   //		else if (character1Direction.x < 0.0f && character1.min.x < character2.max.x) {
   //			character1Direction.x = 0.0f;
   //		}

   //		if (character2Direction.x > 0.0f && character2.max.x > character1.min.x) {
   //			character2Direction.x = 0.0f;
   //		}
   //		else if (character2Direction.x < 0.0f && character2.min.x < character1.max.x) {
   //			character2Direction.x = 0.0f;
   //		}
   //	}
   //	else {
   //		if (character1Direction.z > 0.0f && character1.max.z > character2.min.z) {
   //			character1Direction.z = 0.0f;
   //		}
   //		else if (character1Direction.z < 0.0f && character1.min.z < character2.max.z) {
   //			character1Direction.z = 0.0f;
   //		}

   //		if (character2Direction.z > 0.0f && character2.max.z > character1.min.z) {
   //			character2Direction.z = 0.0f;
   //		}
   //		else if (character2Direction.z < 0.0f && character2.min.z < character1.max.z) {
   //			character2Direction.z = 0.0f;
   //		}
   //	}
   //}

   // 봉과 캐릭터1 충돌 처리
	/*AABB bongs[] = { bong1, bong2, bong3, bong4, bong5, bong6 };
	for (const auto& bong : bongs) {
		if (checkCollision(P1->CAABB, bong)) {
			float overlapbX = std::min(P1->CAABB.max.x, bong.max.x) - std::max(P1->CAABB.min.x, bong.min.x);
			float overlapbZ = std::min(P1->CAABB.max.z, bong.max.z) - std::max(P1->CAABB.min.z, bong.min.z);

			if (overlapbX < overlapbZ) {
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > bong.min.x) {
					P1->Direction.x = 0.0f;
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < bong.max.x) {
					P1->Direction.x = 0.0f;
				}
			}
			else {
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > bong.min.z) {
					P1->Direction.z = 0.0f;
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < bong.max.z) {
					P1->Direction.z = 0.0f;
				}
			}
		}
	}*/

	//봉 움직이기
	//Bong1->Position.x += BongGroup1Direction.x * BongMove;
	Bong1->Position.x += Bong1->Direction.x * Bong1->MoveSpeed;

	if (Bong1->Position.x >= Bong1->MaxMoveDistance) {
		Bong1->Direction.x = -1; // 왼쪽으로 이동
	}
	else if (Bong1->Position.x <= -Bong1->MaxMoveDistance) {
		Bong1->Direction.x = 1;  // 오른쪽으로 이동
	}

	// 봉 그룹 2 움직이기 (반대 방향)
	Bong2->Position.x += Bong2->Direction.x * Bong2->MoveSpeed;
	if (Bong2->Position.x >= Bong2->MaxMoveDistance) {
		Bong2->Direction.x = -1;
	}
	else if (Bong2->Position.x <= -Bong2->MaxMoveDistance) {
		Bong2->Direction.x = 1;
	}

	//if (isObstacleRotate) {
	//	obstacleRotation += 2.0f;
	//}
	Bong1->CAABB.update(Bong1->Position, glm::vec3(-15.74f, 0.0f, -33.25f), glm::vec3(-13.74f, 3.6f, -31.25f));
	Bong2->CAABB.update(Bong2->Position, glm::vec3(-9.47f, 0.0f, -33.25f), glm::vec3(-7.47f, 3.6f, -31.25f));
	//bong3.update(BongGroup1Position, glm::vec3(-3.169f, 0.0f, -33.25f), glm::vec3(-1.169f, 3.6f, -31.25f));
	//bong4.update(BongGroup2Position, glm::vec3(3.045f, 0.0f, -33.25f), glm::vec3(5.045f, 3.6f, -31.25f));
	//bong5.update(BongGroup1Position, glm::vec3(9.27f, 0.0f, -33.25f), glm::vec3(11.27f, 3.6f, -31.25f));
	//bong6.update(BongGroup2Position, glm::vec3(14.945f, 0.0f, -33.25f), glm::vec3(16.945f, 3.6f, -31.25f));


	// 문짝 움직이기
	LeftdoorGroupPosition.x += LeftdoorGroupDirection.x * DoorMove;
	if (LeftdoorGroupPosition.x >= 0.0) {
		LeftdoorGroupDirection.x = -1; // 왼쪽으로 이동
	}
	else if (LeftdoorGroupPosition.x <= -MaxDoorMove) {
		LeftdoorGroupDirection.x = 1;  // 오른쪽으로 이동
	}
	RightdoorGroupPosition.x += RightdoorGroupDirection.x * DoorMove;
	if (RightdoorGroupPosition.x >= MaxDoorMove) {
		RightdoorGroupDirection.x = -1;
	}
	else if (RightdoorGroupPosition.x <= -0.0) {
		RightdoorGroupDirection.x = 1;
	}
	leftdoor1.update(LeftdoorGroupPosition, glm::vec3(-8.475f, -0.76f, -159.129f), glm::vec3(-6.4f, 2.4f, -158.53f));
	leftdoor2.update(LeftdoorGroupPosition, glm::vec3(-2.168f, -0.76f, -159.129f), glm::vec3(-0.09f, 2.4f, -158.53f));
	leftdoor3.update(LeftdoorGroupPosition, glm::vec3(4.227f, -0.76f, -159.129f), glm::vec3(6.297f, 2.4f, -158.53f));
	rightdoor1.update(RightdoorGroupPosition, glm::vec3(6.408f, -0.76f, -159.129f), glm::vec3(4.38f, 2.4f, -158.53f));
	rightdoor2.update(RightdoorGroupPosition, glm::vec3(-0.1f, -0.76f, -159.129f), glm::vec3(1.926f, 2.4f, -158.53f));
	rightdoor3.update(RightdoorGroupPosition, glm::vec3(6.294f, -0.76f, -159.129f), glm::vec3(8.322f, 2.4f, -158.53f));
	AABB leftDoors[] = { leftdoor1, leftdoor2, leftdoor3 };
	AABB rightDoors[] = { rightdoor1, rightdoor2, rightdoor3 };

	// 캐릭터1과 문짝 충돌 처리
	for (const auto& door : leftDoors) {
		if (checkCollision(P1->CAABB, door)) {
			float overlapX = std::min(P1->CAABB.max.x, door.max.x) - std::max(P1->CAABB.min.x, door.min.x);
			float overlapZ = std::min(P1->CAABB.max.z, door.max.z) - std::max(P1->CAABB.min.z, door.min.z);

			if (overlapX < overlapZ) {
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > door.min.x) {
					P1->Direction.x = 0.0f;
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < door.max.x) {
					P1->Direction.x = 0.0f;
				}
			}
			else {
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > door.min.z) {
					P1->Direction.z = 0.0f;
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < door.max.z) {
					P1->Direction.z = 0.0f;
				}
			}
		}
	}
	for (const auto& door : rightDoors) {
		if (checkCollision(P1->CAABB, door)) {
			float overlapX = std::min(P1->CAABB.max.x, door.max.x) - std::max(P1->CAABB.min.x, door.min.x);
			float overlapZ = std::min(P1->CAABB.max.z, door.max.z) - std::max(P1->CAABB.min.z, door.min.z);

			if (overlapX < overlapZ) {
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > door.min.x) {
					P1->Direction.x = 0.0f;
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < door.max.x) {
					P1->Direction.x = 0.0f;
				}
			}
			else {
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > door.min.z) {
					P1->Direction.z = 0.0f;
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < door.max.z) {
					P1->Direction.z = 0.0f;
				}
			}
		}
	}

	AABB outdoors[] = { outdoor1, outdoor2, outdoor3, outdoor4 };
	for (const auto& outdoor : outdoors) {
		if (checkCollision(P1->CAABB, outdoor)) {
			float overlapX = std::min(P1->CAABB.max.x, outdoor.max.x) - std::max(P1->CAABB.min.x, outdoor.min.x);
			float overlapZ = std::min(P1->CAABB.max.z, outdoor.max.z) - std::max(P1->CAABB.min.z, outdoor.min.z);

			if (overlapX < overlapZ) {
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > outdoor.min.x) {
					P1->Direction.x = 0.0f;
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < outdoor.max.x) {
					P1->Direction.x = 0.0f;
				}
			}
			else {
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > outdoor.min.z) {
					P1->Direction.z = 0.0f;
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < outdoor.max.z) {
					P1->Direction.z = 0.0f;
				}
			}
		}
	}


	// 장애물 AABB 업데이트
	PinkFan1->CAABB.updateRotatedAABB(
		glm::vec3(0.0f, -0.3f, -140.0f),  // 장애물의 중심 위치
		glm::vec3(-6.1f, -0.3f, -0.49f), // 로컬 최소 오프셋
		glm::vec3(6.1f, 4.4f, 0.49f),    // 로컬 최대 오프셋
		obstacleRotation,                // 회전 각도
		glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	);

	//horizontalFan2.updateRotatedAABB(
	//	glm::vec3(7.0f, -0.3f, -115.0f),  // 장애물의 중심 위치
	//	glm::vec3(-6.1f, -0.3f, -0.49f), // 로컬 최소 오프셋
	//	glm::vec3(6.1f, 4.4f, 0.49f),    // 로컬 최대 오프셋
	//	obstacleRotation,                // 회전 각도
	//	glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	//);

	//horizontalFan3.updateRotatedAABB(
	//	glm::vec3(-7.0f, -0.3f, -115.0f), // 장애물의 중심 위치
	//	glm::vec3(-6.1f, -0.3f, -0.49f), // 로컬 최소 오프셋
	//	glm::vec3(6.1f, 4.4f, 0.49f),    // 로컬 최대 오프셋
	//	obstacleRotation,                // 회전 각도
	//	glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	//);

	// 장애물 AABB 배열 업데이트
	AABB horizontalFans[] = { PurpleFan1->CAABB/*, horizontalFan2, horizontalFan3 */};

	// 캐릭터1과 장애물 충돌 체크
	for (const auto& fan : horizontalFans) {
		if (checkCollision(P1->CAABB, fan)) {
			float overlapX = std::min(P1->CAABB.max.x, fan.max.x) - std::max(P1->CAABB.min.x, fan.min.x);
			float overlapZ = std::min(P1->CAABB.max.z, fan.max.z) - std::max(P1->CAABB.min.z, fan.min.z);

			if (overlapX < overlapZ) {
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > fan.min.x) {
					P1->Direction.x = 0.0f;
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < fan.max.x) {
					P1->Direction.x = 0.0f;
				}
			}
			else {
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > fan.min.z) {
					P1->Direction.z = 0.0f;
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < fan.max.z) {
					P1->Direction.z = 0.0f;
				}
			}
		}
	}

	//// 캐릭터2와 장애물 충돌 체크
	//for (const auto& fan : horizontalFans) {
	//	if (checkCollision(character2, fan)) {
	//		float overlapX = std::min(character2.max.x, fan.max.x) - std::max(character2.min.x, fan.min.x);
	//		float overlapZ = std::min(character2.max.z, fan.max.z) - std::max(character2.min.z, fan.min.z);

	//		if (overlapX < overlapZ) {
	//			if (character2Direction.x > 0.0f && character2.max.x > fan.min.x) {
	//				character2Direction.x = 0.0f;
	//			}
	//			else if (character2Direction.x < 0.0f && character2.min.x < fan.max.x) {
	//				character2Direction.x = 0.0f;
	//			}
	//		}
	//		else {
	//			if (character2Direction.z > 0.0f && character2.max.z > fan.min.z) {
	//				character2Direction.z = 0.0f;
	//			}
	//			else if (character2Direction.z < 0.0f && character2.min.z < fan.max.z) {
	//				character2Direction.z = 0.0f;
	//			}
	//		}
	//	}
	//}

	// 점프바 회전
	jumpBarRotationAngle += 2.0f;
	if (jumpBarRotationAngle >= 360.0f) {
		jumpBarRotationAngle -= 360.0f;
	}
	AABB barbars[] = { barbar1, barbar2, barbar3 };
	AABB barcenters[] = { barcenter1, barcenter2, barcenter3 };
	// barbar1 AABB 업데이트
	barbar1.updateRotatedAABB(
		glm::vec3(-9.5f, 0.0f, -94.93f),  // 장애물의 중심 위치
		glm::vec3(-0.3f, -0.36f, -0.5f), // 로컬 최소 오프셋
		glm::vec3(0.3f, 0.04f, 0.5f),    // 로컬 최대 오프셋
		jumpBarRotationAngle,            // 회전 각도
		glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	);

	// barbar2 AABB 업데이트
	barbar2.updateRotatedAABB(
		glm::vec3(0.6f, 0.0f, -94.93f),  // 장애물의 중심 위치
		glm::vec3(-0.3f, -0.36f, -0.5f), // 로컬 최소 오프셋
		glm::vec3(0.3f, 0.04f, 0.5f),    // 로컬 최대 오프셋
		-jumpBarRotationAngle,           // 회전 각도 (반대 방향)
		glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	);

	// barbar3 AABB 업데이트
	barbar3.updateRotatedAABB(
		glm::vec3(10.5f, 0.0f, -94.93f), // 장애물의 중심 위치
		glm::vec3(-0.3f, -0.36f, -0.5f), // 로컬 최소 오프셋
		glm::vec3(0.3f, 0.04f, 0.5f),    // 로컬 최대 오프셋
		jumpBarRotationAngle,            // 회전 각도
		glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	);

	for (const auto& barcenter : barcenters) {
		if (checkCollision(P1->CAABB, barcenter)) {
			float overlapX = std::min(P1->CAABB.max.x, barcenter.max.x) - std::max(P1->CAABB.min.x, barcenter.min.x);
			float overlapZ = std::min(P1->CAABB.max.z, barcenter.max.z) - std::max(P1->CAABB.min.z, barcenter.min.z);

			if (overlapX < overlapZ) { // X축에서 더 겹친 경우
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > barcenter.min.x) {
					P1->Direction.x = 0.0f; // 캐릭터가 오른쪽으로 이동 중이면 정지
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < barcenter.max.x) {
					P1->Direction.x = 0.0f; // 캐릭터가 왼쪽으로 이동 중이면 정지
				}
			}
			else { // Z축에서 더 겹친 경우
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > barcenter.min.z) {
					P1->Direction.z = 0.0f; // 캐릭터가 위쪽으로 이동 중이면 정지
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < barcenter.max.z) {
					P1->Direction.z = 0.0f; // 캐릭터가 아래쪽으로 이동 중이면 정지
				}
			}
		}
	}
	for (const auto& bar : barbars) {
		if (checkCollision(P1->CAABB, bar)) {
			float overlapX = std::min(P1->CAABB.max.x, bar.max.x) - std::max(P1->CAABB.min.x, bar.min.x);
			float overlapZ = std::min(P1->CAABB.max.z, bar.max.z) - std::max(P1->CAABB.min.z, bar.min.z);

			if (overlapX < overlapZ) { // X축에서 더 겹친 경우
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > bar.min.x) {
					P1->Direction.x = 0.0f; // 오른쪽 이동 정지
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < bar.max.x) {
					P1->Direction.x = 0.0f; // 왼쪽 이동 정지
				}
			}
			else { // Z축에서 더 겹친 경우
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > bar.min.z) {
					P1->Direction.z = 0.0f; // 위쪽 이동 정지
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < bar.max.z) {
					P1->Direction.z = 0.0f; // 아래쪽 이동 정지
				}
			}
		}
	}


	// 바와 캐릭터1 충돌 처리
	AABB bars[] = { leftBar1, leftBar2, leftBar3, leftBar4, leftBar5, middleBar1, middleBar2, middleBar3, middleBar4, middleBar5, rightBar1, rightBar2, rightBar3, rightBar4, rightBar5 };
	for (const auto& bar : bars) {
		if (checkCollision(P1->CAABB, bar)) {
			float overlapbX = std::min(P1->CAABB.max.x, bar.max.x) - std::max(P1->CAABB.min.x, bar.min.x);
			float overlapbZ = std::min(P1->CAABB.max.z, bar.max.z) - std::max(P1->CAABB.min.z, bar.min.z);

			if (overlapbX < overlapbZ) {
				if (P1->Direction.x > 0.0f && P1->CAABB.max.x > bar.min.x) {
					P1->Direction.x = 0.0f;
				}
				else if (P1->Direction.x < 0.0f && P1->CAABB.min.x < bar.max.x) {
					P1->Direction.x = 0.0f;
				}
			}
			else {
				if (P1->Direction.z > 0.0f && P1->CAABB.max.z > bar.min.z) {
					P1->Direction.z = 0.0f;
				}
				else if (P1->Direction.z < 0.0f && P1->CAABB.min.z < bar.max.z) {
					P1->Direction.z = 0.0f;
				}
			}
		}
	}

	//// 바와 캐릭터2 충돌 처리
	//for (const auto& bar : bars) {
	//	if (checkCollision(character2, bar)) {
	//		float overlapX = std::min(character2.max.x, bar.max.x) - std::max(character2.min.x, bar.min.x);
	//		float overlapZ = std::min(character2.max.z, bar.max.z) - std::max(character2.min.z, bar.min.z);

	//		if (overlapX < overlapZ) {
	//			if (character2Direction.x > 0.0f && character2.max.x > bar.min.x) {
	//				character2Direction.x = 0.0f;
	//			}
	//			else if (character2Direction.x < 0.0f && character2.min.x < bar.max.x) {
	//				character2Direction.x = 0.0f;
	//			}
	//		}
	//		else {
	//			if (character2Direction.z > 0.0f && character2.max.z > bar.min.z) {
	//				character2Direction.z = 0.0f;
	//			}
	//			else if (character2Direction.z < 0.0f && character2.min.z < bar.max.z) {
	//				character2Direction.z = 0.0f;
	//			}
	//		}
	//	}
	//}



	// 이동 처리
	//character1Position += P1.Direction;
	P1->Position += P1->Direction;
	//character2Position += character2Direction;

	// 화면 갱신
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}