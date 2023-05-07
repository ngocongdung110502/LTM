#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }

    fd_set fdread;

    int clients[64];
    int check_Id[64];
    memset(check_Id, 0, sizeof(check_Id));
    char clients_Id[64][100];
    memset(clients_Id, '*', sizeof(clients_Id));
    int num_clients = 0;

    char buf[256];

    while (1)
    {
        // Xóa t?t c? socket trong t?p fdread
        FD_ZERO(&fdread);

        // Thêm socket listener vào t?p fdread
        FD_SET(listener, &fdread);
        int maxdp = listener + 1;

        // Thêm các socket client vào t?p fdread
        for (int i = 0; i < num_clients; i++)
        {
            FD_SET(clients[i], &fdread);
            if (maxdp < clients[i] + 1)
                maxdp = clients[i] + 1;
        }

        // Ch? ð?n khi s? ki?n x?y ra
        int ret = select(maxdp, &fdread, NULL, NULL, NULL);

        if (ret < 0)
        {
            perror("select() failed");
            return 1;
        }

        // Ki?m tra s? ki?n có yêu c?u k?t n?i
        if (FD_ISSET(listener, &fdread))
        {
            int client = accept(listener, NULL, NULL);
            printf("Ket noi moi: %d\n", client);
            clients[num_clients++] = client;
            char *mess = "Nhap id : ";
            send(client, mess, strlen(mess), 0);
        }

        // Ki?m tra s? ki?n có d? li?u truy?n ð?n socket client
        for (int i = 0; i < num_clients; i++)
            if (FD_ISSET(clients[i], &fdread))
            {
                int ret = recv(clients[i], buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                    // TODO: Client ð? ng?t k?t n?i, xóa client ra kh?i m?ng
                    continue;
                }
                buf[ret] = 0;
                printf("Du lieu nhan tu %d: %s\n", clients[i], buf);

                // ki?m tra xem nh?p ID ðúng cú pháp chýa
                if (check_Id[i] == 0)
                {
                    while (strncmp(buf, "client_id: ", 11) != 0 || strchr(buf + 11, ' ') != NULL)
                    {
                        char *error = "Nhap sai cu phap! Vui long nhap lai : ";
                        send(clients[i], error, strlen(error), 0);
                        recv(clients[i], buf, sizeof(buf), 0);
                    }

                    // Ki?m tra xem id có b? trùng v?i các client ðang có không
                    int k = 1;
                    for (int j = 0; j < num_clients; j++)
                    {
                        if (strncmp(clients_Id[j], buf + 11, strlen(clients_Id[j])) == 0)
                        {
                            k = 0;
                            break;
                        }
                    }

                    if (k)
                    {
                        // khi client ð? nh?p ðúng
                        check_Id[i] = 1;
                        strcpy(clients_Id[i], buf + 11);
                        clients_Id[i][strlen(clients_Id[i]) - 1] = 0;
                    }
                    else
                    {
                        char *mess = "ID da ton tai! Vui long nhap lai: ";
                        send(clients[i], mess, strlen(mess), 0);
                    }
                }
                else
                {
                    // G?i tin nh?n cho các client c?n l?i
                    char tmp[2100];
                    sprintf(tmp, "%s: %s\n", clients_Id[i], buf);
                    tmp[strlen(tmp)] = 0;
                    for (int j = 0; j < num_clients; j++)
                    {
                        if (j != i)
                        {
                            send(clients[j], tmp, strlen(tmp), 0);
                        }
                    }
                }
            }
    }

    close(listener);

    return 0;
}
