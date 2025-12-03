#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <set>
#include "AABB.h"
#include "Common.h"
#include "obstacle_network.h"

CRITICAL_SECTION g_cs_client;

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
SOCKET Socket = INVALID_SOCKET; //전역 변수로 소켓 선언

// 패킷 타입 정의
enum PacketType {
	PACKET_CHARACTER = 1,
	PACKET_OBSTACLE = 2
};

// 패킷 헤더 구조체
#pragma pack(1)
struct PacketHeader {
	int type;      // 패킷 종류
	int size;      // 데이터 크기
};
#pragma pack()



#pragma pack(1)
struct character {
	int ID;
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat ArmLegSwingAngle;
	bool isCollision;
};
#pragma pack()

Player1* P1 = nullptr;
Player1* P2 = nullptr;
Player1* P3 = nullptr;


std::atomic<bool> count1_check, count2_check, count3_check;

// 다른 클라이언트들의 캐릭터 정보 저장
#define MAX_OTHER_PLAYERS 2
character otherPlayers[MAX_OTHER_PLAYERS];
bool otherPlayersActive[MAX_OTHER_PLAYERS] = { false, false };

// 캐릭터 정보를 서버에 전송하는 함수
int sendCharacterCount = 0;
void C2S_Character(SOCKET sock, const character& char_info)
{
	// 3. 서버에 패킷 전송
	int retval = send(sock, (char*)&char_info, sizeof(char_info), 0);
	if (sendCharacterCount++ % 100 == 0) {
		std::cout << "\n[전송] 클라이언트 정보 전송 완료 (" << sizeof(char_info) << " 바이트)" << std::endl;
		std::cout << "보낸 ID: " << char_info.ID << std::endl;
		printf("  Position: (%.2f, %.2f, %.2f)\n",
			char_info.position.x, char_info.position.y, char_info.position.z);
		printf("  Direction: (%.2f, %.2f, %.2f)\n",
			char_info.direction.x, char_info.direction.y, char_info.direction.z);
		printf("  ArmLegSwingAngle: %.2f\n", char_info.ArmLegSwingAngle);
		printf("  isCollision: %s\n\n", char_info.isCollision ? "true" : "false");
	}

	if (retval == SOCKET_ERROR) {
		err_display("send() - C2S_Character, position");
		return;
	}

}
// 캐릭터 정보 업데이트
void UpdatePlayer() {
	P2->Position = otherPlayers[0].position;
	P2->Direction = otherPlayers[0].direction;
	// 모델 매트릭스 업데이트
	P2->ModelMatrix = glm::translate(glm::mat4(1.0f), P2->Position);
	P2->ModelMatrix = glm::rotate(P2->ModelMatrix, glm::radians(P2->RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	P2->ArmLegSwingAngle = otherPlayers[0].ArmLegSwingAngle;
	// AABB 업데이트
	P2->CAABB.update(P2->Position, glm::vec3(-0.7f, 0.0f, -0.72f), glm::vec3(0.7f, 1.84f, 0.63f));

	//P3 업데이트
	P3->Position = otherPlayers[1].position;
	P3->Direction = otherPlayers[1].direction;
	// 모델 매트릭스 업데이트
	P3->ModelMatrix = glm::translate(glm::mat4(1.0f), P3->Position);
	P3->ModelMatrix = glm::rotate(P3->ModelMatrix, glm::radians(P3->RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	P3->ArmLegSwingAngle = otherPlayers[1].ArmLegSwingAngle;
	// AABB 업데이트
	P3->CAABB.update(P3->Position, glm::vec3(-0.7f, 0.0f, -0.72f), glm::vec3(0.7f, 1.84f, 0.63f));
}
// 다른 플레이어 정보 저장 헬퍼 함수
void StoreOtherPlayer(const character& received_char) {
	if (P1->ID == 0) {
		// 내가 0번이면: 1번→[0], 2번→[1]
		otherPlayers[received_char.ID - 1] = received_char;
	}
	else if (P1->ID == 1) {
		// 내가 1번이면: 0번→[0], 2번→[1]
		if (received_char.ID == 0)
			otherPlayers[0] = received_char;
		else if (received_char.ID == 2)
			otherPlayers[1] = received_char;
	}
	else {
		// 내가 2번이면: 0번→[0], 1번→[1]
		if (received_char.ID == 0)
			otherPlayers[0] = received_char;
		else if (received_char.ID == 1)
			otherPlayers[1] = received_char;
	}
}
//서버로부터 다른 캐릭터들 정보 받기
int recv_count = 0;
bool recv_packet() {
	if (Socket == INVALID_SOCKET) {
		printf("[경고] 소켓이 유효하지 않습니다\n");
		return false;
	}

	// 1. 패킷 헤더 수신
	PacketHeader header;
	if (!recv(Socket, (char*)&header, sizeof(PacketHeader), 0)) {
		printf("[에러] 패킷 헤더 수신 실패\n");
		return false;
	}

	// 2.  패킷 타입에 따라 처리
	switch (header.type) {
	case PACKET_CHARACTER: {
		character received_char;

		// 캐릭터 데이터 수신
		if (!recv(Socket, (char*)&received_char, sizeof(character), 0)) {
			printf("[에러] 캐릭터 데이터 수신 실패\n");
			return false;
		}

		// 자기 자신이거나 유효하지 않은 ID면 무시
		if (received_char.ID == P1->ID || received_char.ID < 0 || received_char.ID > 2) {
			break;
		}

		// 수신된 캐릭터 정보를 올바른 위치에 저장
		StoreOtherPlayer(received_char);

		// 로그 출력
		recv_count++;
		if (recv_count % 100 == 0) {
			printf("\n[수신] 캐릭터 정보 수신 완료\n");
			printf("  받은 ID: %d\n", received_char.ID);
			printf("  Position: (%.2f, %.2f, %.2f)\n",
				received_char.position.x, received_char.position.y, received_char.position.z);
			printf("  Direction: (%.2f, %.2f, %.2f)\n",
				received_char.direction.x, received_char.direction.y, received_char.direction.z);
			printf("  ArmLegSwingAngle: %.2f\n", received_char.ArmLegSwingAngle);
			printf("  isCollision: %s\n\n", received_char.isCollision ? "true" : "false");
		}
		break;
	}

	case PACKET_OBSTACLE: {
		obstacle_Bong received_obs;

		// 장애물 데이터 수신
		if (!recv(Socket, (char*)&received_obs, sizeof(obstacle_Bong), 0)) {
			printf("[에러] 장애물 데이터 수신 실패\n");
			return false;
		}

		// 장애물 정보 저장
		g_bongObstacle = received_obs;
		break;
	}

	default:
		printf("[경고] 알 수 없는 패킷 타입: %d\n", header.type);
		return false;
	}

	return true;
}
ClientInitInfo p1{ 1, glm::vec3(-7.0f,0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f) };//1번 클라 시작위치, 색깔
ClientInitInfo p2{ 2,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f, 1.0f, 0.0f) };
ClientInitInfo p3{ 3,glm::vec3(7.0f,0.0f,0.0f),glm::vec3(0.0f, 0.0f, 1.0f) };

//번호 받고 캐릭터 번호에 따라 만들기
bool InitCharByNum() {
	// 1. 내 번호 받기
	int data = 0;
	int retval = recv(Socket, (char*)&data, sizeof(data), 0);
	if (retval <= 0) {
		printf("서버에서 내 번호 받기 실패\n");
		return false;
	}
	int order = ntohl(data); // 네트워크 엔디안 변환
	//glm::vec3 P1Color = glm::vec3(1.0f);
	//glm::vec3 P2Color = glm::vec3(1.0f);
	//glm::vec3 P3Color = glm::vec3(1.0f);

	std::cout << order << "번 째 캐릭터 입니다." << std::endl;
	//glm::vec3 RedColor = glm::vec3(1.0f, 0.0f, 0.0f);
	//glm::vec3 YellowColor = glm::vec3(1.0f, 1.0f, 0.0f);
	//glm::vec3 BlueColor = glm::vec3(0.0f, 0.0f, 1.0f);

	//순서에 따라 캐릭터 색상 설정
	switch (order) {

	case 1:
	{
		/*P1Color = YellowColor;
		P2Color = RedColor;
		P3Color = BlueColor;
		glm::vec3 P = glm::vec3(-5.0f, 0.0f, 0.0f);*/
		P1 = new Player1(p2.color);
		P1->Position = p2.startPos;
		P2 = new Player1(p1.color);
		P2->Position = p1.startPos;
		P3 = new Player1(p3.color);
		P3->Position = p3.startPos;
		break;
	}
	case 2:
	{
	/*	P1Color = RedColor;
		P2Color = YellowColor;
		P3Color = BlueColor;
		glm::vec3 P = glm::vec3(0.0f, 0.0f, 0.0f);*/
		P1 = new Player1(p1.color);
		P1->Position = p1.startPos;
		P2 = new Player1(p2.color);
		P2->Position = p2.startPos;
		P3 = new Player1(p3.color);
		P3->Position = p3.startPos;
		
		break;
	}
	case 3:
	{
		/*P1Color = BlueColor;
		P2Color = RedColor;
		P3Color = YellowColor;
		glm::vec3 P = glm::vec3(5.0f, 0.0f, 0.0f);*/
		P1 = new Player1(p3.color);
		P1->Position = p3.startPos;
		P2 = new Player1(p2.color);
		P2->Position = p2.startPos;
		P3 = new Player1(p1.color);
		P3->Position = p1.startPos;
	
		break;
	}
	default:
		std::cerr << "경고: 서버에서 잘못된 order(" << order << ")를 보냄\n";
		return false; // 초기화 중단
	}
	//컨트롤 하는 캐릭터
	//P1 = new Player1(P1Color);
	P1->ID = order - 1;
	//다른 캐릭터들
	

	return true;
}

bool recv_Start() {	//3명 다 접속했는지 확인하고 시작하기
	int recv_data = 0;             // 네트워크에서 받을 raw 데이터
	int retval = recv(Socket, (char*)&recv_data, sizeof(recv_data), 0);

	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return false; // 통신 자체가 실패
	}
	if (retval == 0) {
		printf("모두 접속하지 않음\n");
		return false;
	}

	// 엔디안 변환
	int data = ntohl(recv_data);

	// data = 1 → true, 0 → false
	bool ready = (data == 1);

	return ready;
}

// 네트워크 초기화
bool InitNetworkConnection() {
	WSADATA wsa;

	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		err_quit("WSAStartup()");
		return false;
	}

	// 소켓 생성
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET) {
		err_quit("socket()");
		return false;
	}

	// 서버 주소 설정
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	// 서버에 연결
	int retval = connect(Socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_display("connect()");
		closesocket(Socket);
		Socket = INVALID_SOCKET;
		WSACleanup();
		return false;
	}

	return true;
}

// 네트워크 정리
void CleanupNetworkConnection() {
	if (Socket != INVALID_SOCKET) {
		closesocket(Socket);
		Socket = INVALID_SOCKET;
	}
	WSACleanup();
}
// 맵
GLuint vaoBottom, vaoArrowAndPillar, vaoEndPoint, vaoPoint;
GLuint vboBottom[2], vboArrowAndPillar[2], vboEndPoint[2], vboPoint[2];
Model modelBottom, modelArrowAndPillar, modelEndPoint, modelPoint;


//checkbox
GLuint vaoCheckBoxMap1, vboCheckBoxMap1[2], vaoCheckBoxMap2, vboCheckBoxMap2[2], vaoCheckBoxMap3, vboCheckBoxMap3[2], vaoCheckBoxMap4, vboCheckBoxMap4[2], vaoCheckBoxMap5, vboCheckBoxMap5[2];


GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;


GLfloat obstacleRotation = 0.0f;
GLfloat DoorMove = 0.05f;
GLfloat MaxDoorMove = 1.7f;

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
void InitMap();


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

bool movestart = false;

// 맵
void InitMap() {
	InitPart("Map/bottom.obj", modelBottom, vaoBottom, vboBottom, glm::vec3(0.482f, 0.424f, 0.761f));
	InitPart("Map/arrowAndPillar.obj", modelArrowAndPillar, vaoArrowAndPillar, vboArrowAndPillar, glm::vec3(0.49f, 0.0f, 0.871f));
	InitPart("Map/endPoint.obj", modelEndPoint, vaoEndPoint, vboEndPoint, glm::vec3(1.0f, 0.0f, 1.0f));
	InitPart("Map/point.obj", modelPoint, vaoPoint, vboPoint, glm::vec3(1.0f, 0.0f, 0.0f));
}

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


DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = *(SOCKET*)arg;
	int recv_data = 0;             // 네트워크에서 받을 raw 데이터
	// 엔디안 변환
	while (true)
	{
		int retval = recv(sock, (char*)&recv_data, sizeof(recv_data), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			return 0; // 통신 자체가 실패
		}
		int data = ntohl(recv_data);

		switch (data) {
		case 3:
			count1_check = false;
			count2_check = false;
			count3_check = true;
			break;
		case 2:
			count1_check = false;
			count2_check = true;
			count3_check = false;
			break;
		case 1:
			count1_check = true;
			count2_check = false;
			count3_check = false;
			break;
		case -1:
			printf("카운트 종료 신호 받음, 쓰레드 종료!\n");
			count1_check = false;
			count2_check = false;
			count3_check = false;
			movestart = true;
			//glutPostRedisplay();
			return 0;  // 쓰레드 종료
		default:
			return 0;
		}
		printf("카운트: %d\n", data);
		//glutPostRedisplay();
	}

	return 0;
}

BongGroup* Bong1 = nullptr;
BongGroup* Bong2 = nullptr;
HorizontalFan* HorFan1 = nullptr;
HorizontalFan* HorFan2 = nullptr;
HorizontalFan* HorFan3 = nullptr;
BongGroup* JumpbarCenter = nullptr;
Obstacle* Jumpbar1 = nullptr;
Obstacle* Jumpbar2 = nullptr;
Obstacle* Jumpbar3 = nullptr;
VerticalFan* VerFan1 = nullptr;
VerticalFan* VerFan2 = nullptr;
VerticalFan* VerFan3 = nullptr;
VerticalFan* VerFan4 = nullptr;
VerticalFan* VerFan5 = nullptr;
Door* flogDoor = nullptr;
Obstacle* count3 = nullptr;
Obstacle* count2 = nullptr;
Obstacle* count1 = nullptr;

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
	InitMap();

	//mapcheckbox
	InitCheckBoxMap1();
	InitCheckBoxMap2();
	InitCheckBoxMap3();
	InitCheckBoxMap4();
	InitCheckBoxMap5();

	InitializeCriticalSection(&g_cs_client);


	count3 = new Obstacle(glm::vec3(0.0f, 2.0f, 0.0f));
	count2 = new Obstacle(glm::vec3(0.0f, 2.0f, 0.0f));
	count1 = new Obstacle(glm::vec3(0.0f, 2.0f, 0.0f));
	InitPart("map/3.obj", count3->model, count3->vao, count3->vbo, glm::vec3(1.0f, 0.5f, 0.3f));
	InitPart("map/2.obj", count2->model, count2->vao, count2->vbo, glm::vec3(1.0f, 0.05f, 1.f));
	InitPart("map/1.obj", count1->model, count1->vao, count1->vbo, glm::vec3(0.03f, 0.02f, 0.576f));

	//장애물
	std::cout << "장애물 생성중...." << std::endl;
	Bong1 = new BongGroup(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.1f, 1.6f, 0.0f);
	InitPart("bong/bonggroup1.obj", Bong1->model, Bong1->vao, Bong1->vbo, glm::vec3(1.0f, 0.078f, 0.576f));
	Bong1->SetAABB(bong1, bong3, bong5);

	bong3 = {
	glm::vec3(-3.169f + Bong1->Position.x, 0.0f, -33.25f), // min
	glm::vec3(-1.169f + Bong1->Position.x,  3.6f,  -31.25f)  // max
	};
	Bong2 = new BongGroup(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 0.1f, 1.6f, 0.0f);
	InitPart("bong/bonggroup2.obj", Bong2->model, Bong2->vao, Bong2->vbo, glm::vec3(1.0f, 0.078f, 0.576f));
	Bong2->SetAABB(bong2, bong4, bong6);

	HorFan1 = new HorizontalFan(glm::vec3(0.0f, -0.3f, -140.0f));
	HorFan2 = new HorizontalFan(glm::vec3(7.0f, -0.3f, -115.0f));
	HorFan3 = new HorizontalFan(glm::vec3(-7.0f, -0.3f, -115.0f));
	InitPart("horizontalFan/pink.obj", HorFan1->Pink->model, HorFan1->Pink->vao, HorFan1->Pink->vbo, glm::vec3(1.0f, 0.7f, 0.75f));
	InitPart("horizontalFan/purple.obj", HorFan1->Purple->model, HorFan1->Purple->vao, HorFan1->Purple->vbo, glm::vec3(0.5f, 0.0f, 0.5f));
	InitPart("horizontalFan/pink.obj", HorFan2->Pink->model, HorFan2->Pink->vao, HorFan2->Pink->vbo, glm::vec3(1.0f, 0.7f, 0.75f));
	InitPart("horizontalFan/purple.obj", HorFan2->Purple->model, HorFan2->Purple->vao, HorFan2->Purple->vbo, glm::vec3(0.5f, 0.0f, 0.5f));

	HorFan3->Pink = HorFan1->Pink;
	HorFan3->Purple = HorFan1->Purple;
	HorFan1->CAABB = horizontalFan1;
	HorFan2->CAABB = horizontalFan2;
	HorFan3->CAABB = horizontalFan3;


	JumpbarCenter = new BongGroup(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.1f, 1.6f, 0.0f);
	InitPart("jumpBong/centergroup.obj", JumpbarCenter->model, JumpbarCenter->vao, JumpbarCenter->vbo, glm::vec3(0.576f, 0.078f, 1.0f));
	JumpbarCenter->SetAABB(barcenter1, barcenter2, barcenter3);

	Jumpbar1 = new Obstacle(glm::vec3(9.5f, 0.0f, 94.93f));
	InitPart("jumpBong/bar1.obj", Jumpbar1->model, Jumpbar1->vao, Jumpbar1->vbo, glm::vec3(0.576f, 0.078f, 1.0f));
	Jumpbar1->SetAABB(barbar1);

	Jumpbar2 = new Obstacle(glm::vec3(-0.6f, 0.0f, 94.93f));
	InitPart("jumpBong/bar2.obj", Jumpbar2->model, Jumpbar2->vao, Jumpbar2->vbo, glm::vec3(0.576f, 0.078f, 1.0f));
	Jumpbar2->SetAABB(barbar2);

	Jumpbar3 = new Obstacle(glm::vec3(-10.5f, 0.0f, 94.93f));
	InitPart("jumpBong/bar3.obj", Jumpbar3->model, Jumpbar3->vao, Jumpbar3->vbo, glm::vec3(0.576f, 0.078f, 1.0f));
	Jumpbar3->SetAABB(barbar3);

	VerFan1 = new VerticalFan(glm::vec3(0.0f, 3.0f, -60.0f));
	VerFan2 = new VerticalFan(glm::vec3(-15.0f, 3.0f, -60.0f));
	VerFan3 = new VerticalFan(glm::vec3(15.0f, 3.0f, -60.0f));
	VerFan4 = new VerticalFan(glm::vec3(-7.5f, 3.0f, -60.0f));
	VerFan5 = new VerticalFan(glm::vec3(7.5f, 3.0f, -60.0f));
	InitPart("verticalFan/bar.obj", VerFan1->FanBar->model, VerFan1->FanBar->vao, VerFan1->FanBar->vbo, glm::vec3(0.5f, 0.5f, 0.5f));
	InitPart("verticalFan/center.obj", VerFan1->FanCenter->model, VerFan1->FanCenter->vao, VerFan1->FanCenter->vbo, glm::vec3(1.0f, 0.4f, 0.7f));
	InitPart("verticalFan/fan.obj", VerFan1->VFan->model, VerFan1->VFan->vao, VerFan1->VFan->vbo, glm::vec3(1.0f, 0.4f, 0.7f));
	InitPart("verticalFan/fan.obj", VerFan4->VFan->model, VerFan4->VFan->vao, VerFan4->VFan->vbo, glm::vec3(1.0f, 0.4f, 0.7f));

	VerticalFan* Fans[] = { VerFan2,VerFan3,VerFan4,VerFan5 };
	for (VerticalFan* fan : Fans) {
		fan->FanBar = VerFan1->FanBar;
		fan->FanCenter = VerFan1->FanCenter;
		if (fan == VerFan1 || fan == VerFan2 || fan == VerFan3)
			fan->VFan = VerFan1->VFan;
		else
			fan->VFan = VerFan4->VFan;
	}
	VerFan1->SetAABB(verticalFan1);
	VerFan2->SetAABB(verticalFan2);
	VerFan3->SetAABB(verticalFan3);
	VerFan4->SetAABB(verticalFan4);
	VerFan5->SetAABB(verticalFan5);


	glm::mat4 LeftdoorModelMatrix = glm::mat4(1.0f);
	glm::mat4 RightdoorModelMatrix = glm::mat4(1.0f);
	glm::vec3 LeftdoorGroupPosition = glm::vec3(0.0f, 0.0f, 0.0f); // 초기 위치
	glm::vec3 LeftdoorGroupDirection = glm::vec3(-1.0f, 0.0f, 0.0f); // 초기 이동 방향 (왼쪽)
	glm::vec3 RightdoorGroupPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 RightdoorGroupDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	// 개구리문

	flogDoor = new Door(glm::vec3(0.0f, 0.0f, 0.0f));
	InitPart("frogDoor/outsidegroup.obj", flogDoor->Center->model, flogDoor->Center->vao, flogDoor->Center->vbo, glm::vec3(0.576f, 0.078f, 1.0f));
	InitPart("frogDoor/leftdoorgroup.obj", flogDoor->LeftD->model, flogDoor->LeftD->vao, flogDoor->LeftD->vbo, glm::vec3(1.0f, 0.078f, 0.576f));
	InitPart("frogDoor/rightdoorgroup.obj", flogDoor->RightD->model, flogDoor->RightD->vao, flogDoor->RightD->vbo, glm::vec3(1.0f, 0.078f, 0.576f));
	flogDoor->LeftD->SetAABB(leftdoor1, leftdoor2, leftdoor3);
	flogDoor->RightD->SetAABB(rightdoor1, rightdoor2, rightdoor3);


	printf("[클라이언트] 서버 연결 시도...\n");
	if (!InitNetworkConnection()) {
		printf("[클라이언트] 서버 연결 실패! 게임을 종료합니다.\n");
		return;
	}
	printf("[클라이언트] 서버 연결 성공!\n");



	std::cout << "캐릭터 생성중...." << std::endl;
	if (!InitCharByNum())
	{
		std::cerr << "캐릭터 초기화 실패!" << std::endl;
		return;
	}

	//여기서 3,2,1받을 준비 시작함. 이렇게 쓰레드 분리해야 접속 대기 중에도 그림그려짐
	CreateThread(NULL, 0, RecvThread, &Socket, 0, NULL);

	//std::cerr << " 접속 기다리는 중~..." << std::endl;
	//while (!recv_Start()) {
	//	//std::cerr << " 안온대 ~..." << std::endl;
	//}
	//printf("[클라이언트] 3명 접속 성공!\n");





	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);

	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);
	glutTimerFunc(16, Timer, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	glutMainLoop();
	DeleteCriticalSection(&g_cs_client);
}


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
		(float)(window_Width) / (float)window_Height, // 좌우 절반의 종횡비
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
	Bong1->Draw(shaderProgramID, modelMatrixLocation);
	Bong2->Draw(shaderProgramID, modelMatrixLocation);
	P1->Draw(shaderProgramID, modelMatrixLocation);

	// p2 위치 동기화
	P2->Draw(shaderProgramID, modelMatrixLocation);
	// p3 ��ġ ����ȭ
	P3->Draw(shaderProgramID, modelMatrixLocation);


	//상대 캐릭터도 받아서 그려야함.
	HorFan1->Draw(shaderProgramID, modelMatrixLocation);
	HorFan2->Draw(shaderProgramID, modelMatrixLocation);
	HorFan3->Draw(shaderProgramID, modelMatrixLocation);
	JumpbarCenter->JumpBarCenterDraw(shaderProgramID, modelMatrixLocation);
	Jumpbar1->JumpBarDraw(shaderProgramID, modelMatrixLocation);
	Jumpbar2->JumpBarDraw(shaderProgramID, modelMatrixLocation);
	Jumpbar3->JumpBarDraw(shaderProgramID, modelMatrixLocation);
	VerFan1->Draw(shaderProgramID, modelMatrixLocation);
	VerFan2->Draw(shaderProgramID, modelMatrixLocation);
	VerFan3->Draw(shaderProgramID, modelMatrixLocation);
	VerFan4->Draw(shaderProgramID, modelMatrixLocation);
	VerFan5->Draw(shaderProgramID, modelMatrixLocation);
	flogDoor->Draw(shaderProgramID, modelMatrixLocation);

	//카운트다운 표시
	if (count3_check) {
		count3->Draw(shaderProgramID, modelMatrixLocation);
	}
	else if (count2_check) {
		count2->Draw(shaderProgramID, modelMatrixLocation);
	}
	else if (count1_check) {
		count1->Draw(shaderProgramID, modelMatrixLocation);
	}


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
void MovingCharacter() {
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
}

GLvoid Timer(int value) {
	if (movestart) {
		MovingCharacter();
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
	// 이동 처리
	P1->Position += P1->Direction;

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

	if (movestart) {
		//전송 로직
		if (Socket != INVALID_SOCKET && P1 != nullptr) {
			character myCharacter;
			myCharacter.ID = P1->ID;
			myCharacter.position = P1->Position;
			myCharacter.direction = P1->Direction;
			myCharacter.ArmLegSwingAngle = P1->ArmLegSwingAngle;
			myCharacter.isCollision = false;  // 필요시 나중에 수정

			C2S_Character(Socket, myCharacter);
		}


		if (Socket != INVALID_SOCKET) {
			int packetCount = 0;
			const int MAX_PACKETS_PER_FRAME = 2;  // 한 프레임당 최대 처리 패킷 수

			while (packetCount < MAX_PACKETS_PER_FRAME) {
				if (!recv_packet()) {
					break;  // 더 이상 받을 패킷 없음
				}
				packetCount++;
			}
		}
	}
	UpdatePlayer();


	//EnterCriticalSection(&g_cs_client);
	//if (Bong1) Bong1->Position = g_bongObstacle.pos1;
	//else std::cout << "[Warn] Bong1 is NULL\n";

	//if (Bong2) Bong2->Position = g_bongObstacle.pos2;
	//else std::cout << "[Warn] Bong2 is NULL\n";
	//LeaveCriticalSection(&g_cs_client);

	if (Bong1) {
		Bong1->ModelMatrix = glm::translate(glm::mat4(1.0f), Bong1->Position);
	}
	if (Bong2) {
		Bong2->ModelMatrix = glm::translate(glm::mat4(1.0f), Bong2->Position);
	}

	//Bong1->CAABB1.update(Bong1->Position, glm::vec3(-15.74f, 0.0f, -33.25f), glm::vec3(-13.74f, 3.6f, -31.25f));
	//Bong2->CAABB1.update(Bong2->Position, glm::vec3(-9.47f, 0.0f, -33.25f), glm::vec3(-7.47f, 3.6f, -31.25f));
	//Bong1->CAABB2.update(Bong1->Position, glm::vec3(-3.169f, 0.0f, -33.25f), glm::vec3(-1.169f, 3.6f, -31.25f));
	//Bong2->CAABB2.update(Bong2->Position, glm::vec3(3.045f, 0.0f, -33.25f), glm::vec3(5.045f, 3.6f, -31.25f));
	//Bong1->CAABB3.update(Bong1->Position, glm::vec3(9.27f, 0.0f, -33.25f), glm::vec3(11.27f, 3.6f, -31.25f));
	//Bong2->CAABB3.update(Bong2->Position, glm::vec3(14.945f, 0.0f, -33.25f), glm::vec3(16.945f, 3.6f, -31.25f));

	// 문짝 움직이기
	flogDoor->LeftD->Position.x += flogDoor->LeftD->Direction.x * DoorMove;
	if (flogDoor->LeftD->Position.x >= 0.0) {
		flogDoor->LeftD->Direction.x = -1; // 왼쪽으로 이동
	}
	else if (flogDoor->LeftD->Position.x <= -MaxDoorMove) {
		flogDoor->LeftD->Direction.x = 1;  // 오른쪽으로 이동
	}
	flogDoor->RightD->Position.x += flogDoor->RightD->Direction.x * DoorMove;
	if (flogDoor->RightD->Position.x >= MaxDoorMove) {
		flogDoor->RightD->Direction.x = -1;
	}
	else if (flogDoor->RightD->Position.x <= -0.0) {
		flogDoor->RightD->Direction.x = 1;
	}
	// 문짝 AABB 업데이트
	flogDoor->LeftD->CAABB1.update(flogDoor->LeftD->Position, glm::vec3(-8.475f, -0.76f, -159.129f), glm::vec3(-6.4f, 2.4f, -158.53f));
	flogDoor->LeftD->CAABB2.update(flogDoor->LeftD->Position, glm::vec3(-2.168f, -0.76f, -159.129f), glm::vec3(-0.09f, 2.4f, -158.53f));
	flogDoor->LeftD->CAABB3.update(flogDoor->LeftD->Position, glm::vec3(4.227f, -0.76f, -159.129f), glm::vec3(6.297f, 2.4f, -158.53f));
	flogDoor->RightD->CAABB1.update(flogDoor->RightD->Position, glm::vec3(6.408f, -0.76f, -159.129f), glm::vec3(4.38f, 2.4f, -158.53f));
	flogDoor->RightD->CAABB2.update(flogDoor->RightD->Position, glm::vec3(-0.1f, -0.76f, -159.129f), glm::vec3(1.926f, 2.4f, -158.53f));
	flogDoor->RightD->CAABB3.update(flogDoor->RightD->Position, glm::vec3(6.294f, -0.76f, -159.129f), glm::vec3(8.322f, 2.4f, -158.53f));
	//AABB leftDoors[] = { leftdoor1, leftdoor2, leftdoor3 };
	//AABB rightDoors[] = { rightdoor1, rightdoor2, rightdoor3 };


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
	HorFan1->CAABB.updateRotatedAABB(
		glm::vec3(0.0f, -0.3f, -140.0f),  // 장애물의 중심 위치
		glm::vec3(-6.1f, -0.3f, -0.49f), // 로컬 최소 오프셋
		glm::vec3(6.1f, 4.4f, 0.49f),    // 로컬 최대 오프셋
		obstacleRotation,                // 회전 각도
		glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	);

	HorFan2->CAABB.updateRotatedAABB(
		glm::vec3(7.0f, -0.3f, -115.0f),  // 장애물의 중심 위치
		glm::vec3(-6.1f, -0.3f, -0.49f), // 로컬 최소 오프셋
		glm::vec3(6.1f, 4.4f, 0.49f),    // 로컬 최대 오프셋
		-obstacleRotation,                // 회전 각도
		glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	);

	HorFan3->CAABB.updateRotatedAABB(
		glm::vec3(-7.0f, -0.3f, -115.0f), // 장애물의 중심 위치
		glm::vec3(-6.1f, -0.3f, -0.49f), // 로컬 최소 오프셋
		glm::vec3(6.1f, 4.4f, 0.49f),    // 로컬 최대 오프셋
		obstacleRotation,                // 회전 각도
		glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
	);

	HorizontalFan* horizontalFans[] = { HorFan1, HorFan2, HorFan3 };

	for (int i = 0; i < 3; i++) {
		if (i != 1) {
			horizontalFans[i]->Pink->RotationAngle = obstacleRotation;
			horizontalFans[i]->Purple->RotationAngle = obstacleRotation;
		}
		else {
			horizontalFans[i]->Pink->RotationAngle = -obstacleRotation;
			horizontalFans[i]->Purple->RotationAngle = -obstacleRotation;
		}
	}


	AABB barbars[] = { Jumpbar1->CAABB, Jumpbar2->CAABB, Jumpbar3->CAABB };
	Obstacle* Bars[] = { Jumpbar1, Jumpbar2, Jumpbar3 };
	//AABB barcenters[] = { barcenter1, barcenter2, barcenter3 };


	int i = 0;
	for (const auto& bar : Bars) {
		bar->RotationAngle += 2.0f;
		if (bar->RotationAngle >= 360.0f) {
			bar->RotationAngle -= 360.0f;
		}
		int val = (i % 2 == 0) ? 1 : -1;
		if (val == -1)bar->RotationAngle = -(Jumpbar1->RotationAngle);
		bar->CAABB.updateRotatedAABB(
			-(bar->Position),  // 장애물의 중심 위치
			glm::vec3(-0.3f, -0.36f, -0.5f), // 로컬 최소 오프셋
			glm::vec3(0.3f, 0.04f, 0.5f),    // 로컬 최대 오프셋
			bar->RotationAngle,            // 회전 각도
			glm::vec3(0.0f, 1.0f, 0.0f)      // 회전 축
		);
		i++;
	}


	// 세로팬 -------------------------------------------------------------------------------------------
	VerticalFan* verticalFans[] = { VerFan1,VerFan2,VerFan3,VerFan4,VerFan5 };

	for (int i = 0; i < 5; i++) {
		if (i < 3) {
			verticalFans[i]->VFan->RotationAngle = obstacleRotation;
		}
		else {
			verticalFans[i]->VFan->RotationAngle = -obstacleRotation;
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
	// ^ 세로팬 -------------------------------------------------------------------------------------------







	// 화면 갱신
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}