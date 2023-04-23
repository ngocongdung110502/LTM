#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    int sockfd= socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("Khong the tao socket\n");
        return -1;
    }
    printf("Tao socket thanh cong. So descriptor cua socket la: %d\n", sockfd);

    close(sockfd);
    return 0;
}