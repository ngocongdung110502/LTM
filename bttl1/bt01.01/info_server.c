#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT "9000"

int main(){
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server == -1){
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(PORT));

    if(bind(server, (struct sockaddr*)&addr, sizeof(addr))){
        perror("bind() failed");
        return 1;
    }

    if(listen(server, 5)){
        perror("listen() failed");
        return 1;
    }

    printf("Waiting for incoming connections...\n");

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int client = accept(server, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (client == -1)
    {
        perror("accept() failed");
        return 1;
    }

    char buf[10000];
    int len = recv(client, buf, sizeof(buf), 0);
    if (len == -1)
    {
        perror("recv() failed");
        return 1;
    }
    buf[len] = '\0';

    char *hostname = strtok(buf, ";");
    char *drives = strtok(NULL, ";");

    printf("Hostname: %s\n", hostname);
    printf("Drives: %s\n", drives);

    close(server);
    close(client);
    return 0;
}