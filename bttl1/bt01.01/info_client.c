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
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1){
        perror("socket() failed");
        return 1;
    }

    struct addrinfo *info_server, hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    char server[100];
    printf("Nhap ten server: ");
    scanf("%s", server);

    int status = getaddrinfo(server, PORT, &hints, &info_server);
    if(status == 0){
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(status));
        return 1;
    }

    if(connect(client, info_server->ai_addr, info_server->ai_addrlen)){
        perror("connect() failed");
        return 1;
    }

    char hostname[1000];
    printf("Nhap ten may tinh: ");
    scanf("%s", hostname);

    char drive[1000];
    printf("\nNhap danh sach o dia: ");
    scanf("%s", drive);

    char buf[10000];
    sprintf(buf, "%s,%s", hostname, drive);

    send(client, buf, strlen(buf), 0);

    close(client);
    return 0;
}