#include "Common.h"
#include <gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include "obstacle.h"
#pragma comment(lib, "ws2_32.lib")

#define SERVERPORT 9000
#define MAX_CLIENTS 3



/*  Àü´Ş»çÇ×¾Æ´Ñ Àü´Ş»çÇ×


g_clientCount ÀÌ°É·Î¸¸ ÆÇ´ÜÇÏ¸é ¾ÈµÉ °Í °°À½. µé¾î¿Ô´Ù°¡ ³ª°¬À»¶§ Àú ¼ö°¡ ÁÙ¾îµé°Ô ÇØ¾ßÇÒµí.
±×¸®°í ÀçÁ¢ °¡´ÉÇÏ°Ô ÇÒ °ÍÀÎÁö, ´Ù Á¢¼ÓÇßÀ»¶§ ¼¼¸í ´Ù ·¹µğ ´­·¯¾ß ½ÃÀÛÇÏ°Ô ÇÒ °ÍÀÎÁö>?
Á¢¼ÓÇÏÀÚ¸¶ÀÚ °ÔÀÓ ½ÃÀÛÀº ¾à°£ ¾Æ½¬¿î ºÎºĞÀÌ ÀÖÀ»¼öµµÀÖÀ»°Í°°À½. ±×³É ÀÇ°ßÀÓ.

*/

CRITICAL_SECTION g_cs;  // ÀÓ°è¿µ¿ª
int g_clientCount = 0;

// Ä³¸¯ÅÍ ±¸Á¶Ã¼
#pragma pack(1)
struct character {
    glm::vec3 position;
    glm::vec3 direction;
    GLfloat ArmLegSwingAngle;
    bool isCollision;
};
#pragma pack()
// Å¬¶óÀÌ¾ğÆ® Á¤º¸ ±¸Á¶Ã¼ Ãß°¡
struct ClientInfo {
    SOCKET sock;
    int id;
    character charInfo;
    bool isActive;
};
// Ä³¸¯ÅÍ Á¤º¸ ÀúÀåÇÏ±â À§ÇØ¼­ Å¬¶óÀÌ¾ğÆ® Á¤º¸ ±¸Á¶Ã¼ ¹è¿­
ClientInfo g_clients[MAX_CLIENTS];
// Ãæµ¹ Ã³¸® ÇÔ¼ö (¾ÆÁ÷ ¹Ì±¸Çö)
bool CheckCollision(const character& ch) {
    return false; // ÀÓ½Ã ¹İÈ¯
}

// Å¬¶óÀÌ¾ğÆ®·ÎºÎÅÍ Ä³¸¯ÅÍ Á¤º¸ ¹Ş±â
bool recv_character(SOCKET sock, character& ch) {
    int retval = recv(sock, (char*)&ch, sizeof(character), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv() - recv_character");
        return false;
    }
    if (retval == 0) {
        printf("Å¬¶óÀÌ¾ğÆ® ¿¬°á Á¾·á\n");
        return false;
    }
    return true;
}

// ¼­¹ö¿¡¼­ Å¬¶óÀÌ¾ğÆ®·Î Ä³¸¯ÅÍ Á¤º¸ Àü¼Û
bool S2C_Character(SOCKET sock, const character& char_info) {
    // ¼ÒÄÏÀÌ À¯È¿ÇÑÁö È®ÀÎ
    if (sock == INVALID_SOCKET) {
        printf("[°æ°í] ¼ÒÄÏÀÌ À¯È¿ÇÏÁö ¾Ê½À´Ï´Ù\n");
        return false;
    }

    // Å¬¶óÀÌ¾ğÆ®¿¡°Ô Ä³¸¯ÅÍ Á¤º¸ Àü¼Û
    int retval = send(sock, (char*)&char_info, sizeof(character), 0);

    if (retval == SOCKET_ERROR) {
        int err = WSAGetLastError();
        printf("[¿¡·¯] S2C_Character() Àü¼Û ½ÇÆĞ - ¿¡·¯ÄÚµå: %d\n", err);
        return false;
    }

    if (retval != sizeof(character)) {
        printf("[°æ°í] Àü¼ÛµÈ µ¥ÀÌÅÍ Å©±â ºÒÀÏÄ¡ (¿¹»ó: %zu, ½ÇÁ¦: %d)\n", sizeof(character), retval);
    }

    return true;
}

int S2C_ClientOrder(SOCKET sock, int order) {   //Å¬¶ó¿¡°Ô ¸î¹øÂ° Å¬¶óÀÎÁö º¸³»ÁÖ´Â ÇÔ¼ö
    int retval;
    int data = htonl(order); // ¿£µğ¾È º¯È¯(ÇÊ¼ö)

    retval = send(sock, (char*)&data, sizeof(data), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        return -1;
    }

    return retval;
}

bool IsAllPlayersReady()    //3¸í ¸ğµÎ Á¢¼ÓÇß´Ï?       
{
    bool ready = false;

    EnterCriticalSection(&g_cs);
    if (g_clientCount >= 3)   // ¿øÇÏ´Â ÃÖ¼Ò ÀÎ¿ø
        ready = true;
    LeaveCriticalSection(&g_cs);

    return ready;
}

bool S2C_isPlayerReady(SOCKET sock) {  // ÇÃ·¹ÀÌ¾î ¸ğµÎ Á¢¼Ó ¿Ï·á µÆ´ÂÁö Àü¼Û ÇÔ¼ö 
    bool ready = IsAllPlayersReady();
    int data = ready ? 1 : 0;

    // ¿£µğ¾È º¯È¯
    int send_data = htonl(data);

    int retval = send(sock, (char*)&send_data, sizeof(send_data), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return false;
    }

    return true;
}

// Å¬¶óÀÌ¾ğÆ® ½º·¹µå ÇÔ¼ö
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = *(SOCKET*)arg;
    free(arg);
    int client_id;

    EnterCriticalSection(&g_cs);
    client_id = ++g_clientCount;
	g_clients[client_id - 1].sock = client_sock;
	g_clients[client_id - 1].id = client_id;
	g_clients[client_id - 1].isActive = true;
    LeaveCriticalSection(&g_cs);

    printf("Å¬¶óÀÌ¾ğÆ® %d¹ø Á¢¼Ó ¿Ï·á\n", client_id);
	S2C_ClientOrder(client_sock, client_id);   //¸î¹øÂ° Å¬¶óÀÎÁö º¸³»ÁÖ±â
    //ÀÌ ºÎºĞ ¼öÁ¤ ÇÊ¿äÇÒ °Í °°À½------ while µ¹¸±¿¹Á¤.
    S2C_isPlayerReady(client_sock);//3¸í Á¢¼ÓÇß´ÂÁö È®ÀÎÇÏ°í ¸ÂÀ¸¸é Å¬¶ó¿¡°Ô º¸³»±â
    //----------

    // TODO
    int receive_count = 0;
    int send_count[MAX_CLIENTS];
    while (true) {
        character received_char;

        // Å¬¶óÀÌ¾ğÆ®·ÎºÎÅÍ Ä³¸¯ÅÍ Á¤º¸ ¼ö½Å
        if (!recv_character(client_sock, received_char)) {
            break;  // ¼ö½Å ½ÇÆĞ ½Ã ·çÇÁ Á¾·á
        }

        receive_count++;

        // ¼ö½ÅÇÑ µ¥ÀÌÅÍ Ãâ·Â
        if (receive_count % 100 == 0) {
            printf("\n=== [Å¬¶óÀÌ¾ğÆ® %d] ¼ö½Å %dÈ¸ ===\n", g_clients[client_id - 1].id, receive_count);
            printf("  Position: (%.2f, %.2f, %.2f)\n",
                received_char.position.x, received_char.position.y, received_char.position.z);
            printf("  Direction: (%.2f, %.2f, %.2f)\n",
                received_char.direction.x, received_char.direction.y, received_char.direction.z);
            printf("  ArmLegSwingAngle: %.2f\n", received_char.ArmLegSwingAngle);
            printf("  isCollision: %s\n", received_char.isCollision ? "true" : "false");
            printf("\n");
        }
        // ÀÓ°è¿µ¿ª ÁøÀÔ - µ¥ÀÌÅÍ ÀúÀå
        EnterCriticalSection(&g_cs);
<<<<<<< HEAD:Server/Server/main.cpp
        // ¿©±â¿¡ Å¬¶óÀÌ¾ğÆ® Á¤º¸ ÀúÀå (³ªÁß¿¡ ±¸Çö)

        g_rotatingObstacle.angle += 1.0f; // È¸Àü °¢µµ 1µµ Áõ°¡ (¿¹½Ã)
        g_movingObstacle.position += g_movingObstacle.direction;
        LeaveCriticalSection(&g_cs);

        if (!S2C_MovingObstacle(client_sock, g_movingObstacle)) {
            printf("[°æ°í] Å¬¶óÀÌ¾ğÆ® %d¿¡°Ô MovingObstacle Àü¼Û ½ÇÆĞ\n", client_id);
        }

        if (!S2C_RotatingObstacle(client_sock, g_rotatingObstacle)) {
            printf("[°æ°í] Å¬¶óÀÌ¾ğÆ® %d¿¡°Ô RotatingObstacle Àü¼Û ½ÇÆĞ\n", client_id);
        }
=======
		g_clients[client_id - 1].charInfo = received_char;
        LeaveCriticalSection(&g_cs);

        EnterCriticalSection(&g_cs);
		// ´Ù¸¥ Å¬¶óÀÌ¾ğÆ®µé¿¡°Ô Ä³¸¯ÅÍ Á¤º¸ Àü¼Û
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if (i != client_id - 1 && g_clients[i].isActive) { // ÀÚ±â ÀÚ½Å Á¦¿Ü
                if (!S2C_Character(g_clients[i].sock, received_char)) {
                    printf("Å¬¶óÀÌ¾ğÆ® %d¹ø¿¡°Ô Ä³¸¯ÅÍ Á¤º¸ Àü¼Û ½ÇÆĞ\n", g_clients[i].id);
                }
                else {
                    if (send_count[i] % 100 == 0) {
                        printf("[¼­¹ö] Å¬¶óÀÌ¾ğÆ® %d Ä³¸¯ÅÍ Á¤º¸ Àü¼Û ¿Ï·á ¼Û½Å %dÈ¸ \n", g_clients[client_id - 1].id , send_count);
                        send_count[i]++;
                    }
                }
            }
		}
        LeaveCriticalSection(&g_cs);

>>>>>>> develop/S2C_Char:Server/Server/ì†ŒìŠ¤.cpp
    }

    closesocket(client_sock);
    printf("Å¬¶óÀÌ¾ğÆ® %d¹ø ¿¬°á Á¾·á\n", client_id);
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET listen_sock, client_sock;
    struct sockaddr_in serveraddr, clientaddr;
    int addrlen = sizeof(clientaddr);

    // Winsock ÃÊ±âÈ­
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup ½ÇÆĞ\n");
        return 1;
    }

    InitializeCriticalSection(&g_cs);

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        printf("¼ÒÄÏ »ı¼º ½ÇÆĞ\n");
        return 1;
    }

    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    if (bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("bind ½ÇÆĞ: %d\n", WSAGetLastError());
        closesocket(listen_sock);
        return 1;
    }

    if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen ½ÇÆĞ\n");
        closesocket(listen_sock);
        return 1;
    }

    printf("¼­¹ö°¡ Æ÷Æ® %d¿¡¼­ ´ë±â Áß...\n", SERVERPORT);

    while (1) {
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            printf("accept ½ÇÆĞ\n");
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientaddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        printf("Å¬¶óÀÌ¾ğÆ® Á¢¼Ó: IP=%s, PORT=%d\n", clientIP, ntohs(clientaddr.sin_port));

        // ½º·¹µå ÀÎÀÚ·Î Àü´ŞÇÒ ¼ÒÄÏ º¹»çº» µ¿Àû ÇÒ´ç
        SOCKET* pClientSock = (SOCKET*)malloc(sizeof(SOCKET));
        *pClientSock = client_sock;

        HANDLE hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)pClientSock, 0, NULL);
        if (hThread == NULL) {
            printf("½º·¹µå »ı¼º ½ÇÆĞ\n");
            closesocket(client_sock);
            free(pClientSock);
        }
        else {
            CloseHandle(hThread);
        }

        if (g_clientCount >= MAX_CLIENTS) {
            printf("ÃÖ´ë Á¢¼ÓÀÚ ¼ö µµ´Ş\n");
            break;
        }
    }

    DeleteCriticalSection(&g_cs);
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}