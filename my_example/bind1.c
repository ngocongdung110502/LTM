#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener == -1){
        printf("Failed to create socket: %d - %s\n", errno, strerror(errno));
        perror("socket() failed");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if(bind(listener, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        printf("Failed to bind socket: %d - %s\n", errno, strerror(errno));
        perror("bind() failed");
        close(listener);
        exit(1);
    } 

    printf("Socket bound successfully.\n");
    close(listener);
    return 0;
}