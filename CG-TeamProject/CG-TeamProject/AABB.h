#pragma once
#include "Obstacle.h"

// ¸Ê
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
////////////////////////////////////////////////////////////////////////////////////////////////

AABB bong1 = {
	glm::vec3(-15.74f , 0.0f, -33.25f), // min
	glm::vec3(-13.74f,  3.6f,  -31.25f)  // max
};
AABB bong2 = {
	glm::vec3(-9.47f, 0.0f, -33.25f), // min
	glm::vec3(-7.47f ,  3.6f,  -31.25f)  // max
};
AABB bong3 = { };
AABB bong4 = {
	glm::vec3(3.045f , 0.0f, -33.25f), // min
	glm::vec3(5.045f,  3.6f,  -31.25f)  // max
};
AABB bong5 = {
	glm::vec3(9.27f , 0.0f, -33.25f), // min
	glm::vec3(11.27f ,  3.6f,  -31.25f)  // max
};
AABB bong6 = {
	glm::vec3(14.945f , 0.0f, -33.25f), // min
	glm::vec3(16.945f ,  3.6f,  -31.25f)  // max
};


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

// ¼¼·ÎÆÒ
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

// ¼¼·ÎÆÒ
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

// °¡·ÎÆÒ
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

// ¹®
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ¸Ê Ãæµ¹¹Ú½º
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
