#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, -1);
    if(listener != -1)
        printf("Socket created: %d\n", listener);
    else {
        printf("Failed to create socket: %d - %s\n", errno, strerror(errno));
        perror("socket() failed");
        exit(1);
    }
    close(listener);
    return 0;
}