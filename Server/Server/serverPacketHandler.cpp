#include "serverPacketHandler.h"
#include <winsock2.h>

bool SendPacket(SOCKET sock, const void* pkt, int size) {
    int sent = send(sock, (char*)pkt, size, 0);
    return sent == size;
}

bool RecvPacket(SOCKET sock, void* buf, int size) {
    int recved = 0;
    char* ptr = (char*)buf;

    while (recved < size) {
        int r = recv(sock, ptr + recved, size - recved, 0);
        if (r <= 0) return false;
        recved += r;
    }
    return true;
}