#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int fdsock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(fdsock, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));

    char *buffer = "PING";
    send(fdsock, buffer, 4, MSG_NOSIGNAL);
    recv(fdsock, buffer, 4, MSG_NOSIGNAL);

    shutdown(fdsock,SHUT_RDWR);
    close(fdsock);

    printf("%s\n", buffer);

    return 0;
}
