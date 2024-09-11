#include <WS2tcpip.h>
#include <WinSock2.h>
#include <cstddef>
#include <cstdio>
#include <minwindef.h>
#include <time.h>
#include <winsock2.h>

#define LOGI(format, ...)                                                      \
    fprintf(stderr, "[INFO] [%s:%d]:%s() " format "\n", __FILE__, __LINE__,    \
            __func__, ##__VA_ARGS__)
#define LOGE(format, ...)                                                      \
    fprintf(stderr, "[ERROR] [%s:%d]:%s() " format "\n", __FILE__, __LINE__,   \
            __func__, ##__VA_ARGS__)

int main(int argc, const char **argv) {

    const int port = 8080;
    const char *serverIp = "127.0.0.1";

    LOGI("TcpClient start,serverIp=%s,serverPort=%d", serverIp, port);

    WSADATA WSADATA;
    if (WSAStartup(MAKEWORD(2, 2), &WSADATA) == -1) {
        LOGE("WSAStartup error");
        return -1;
    }

    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        LOGE("create socket error");
		WSACleanup();
        return -1;
    }

	// int on = 1;
	// setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));

    // client_addr
/*
    当客户端调用 `connect()` 之前没有显式调用 `bind()`，操作系统会自动为客户端分配一个临时的端口（通常是大于 1024 的随机端口），并绑定到客户端的 `IP` 地址。
    sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    // client_addr.sin_port = htons(8281);
    if (bind(fd, (const struct sockaddr *)&client_addr, sizeof(client_addr)) ==
        -1) {
		WSACleanup();
        LOGE("socket bind error");
        return -1;
    }
*/

    // server_addr
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, serverIp, &server_addr.sin_addr);

    if (connect(fd, (const struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
        LOGE("socket connect error");
        return -1;
    }

    LOGI("fd=%d connect success", fd);

    int size;
    char buf[10000];
    size_t len = 10000;
    size_t totalSize;
    time_t t1 = time(NULL);
    while (1) {
        size = recv(fd, buf, len, 0);
        if (size <= 0) {
            break;
        }
        totalSize += size;
        if (totalSize > 1024) /* 62914560=60*1024*1024=60mb*/
        {
            time_t t2 = time(NULL);
            if (t2 - t1 > 0) {
                size_t speed = totalSize / 1024 / 1024 / (t2 - t1);
                printf("fd=%d,size=%d,totalSize=%llu,speed=%lluMbps\n", fd,
                       size, totalSize, speed);
                totalSize = 0;
                t1 = time(NULL);
            }
        }
    }

    closesocket(fd);
    WSACleanup();

    return 0;
}
