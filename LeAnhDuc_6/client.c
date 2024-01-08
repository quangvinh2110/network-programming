//21020761 Le Anh Duc

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
//tao socket
    int client_socket;
    char buffer[1024];
    struct sockaddr_in server_address;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // Gửi lệnh DOWNLOAD tới server
    send(client_socket, "DOWNLOAD", sizeof("DOWNLOAD"), 0);

    char filename[100];
    printf("Enter the filename to download: ");
    scanf("%s", filename);

    // Gửi tên file tới server
    send(client_socket, filename, sizeof(filename), 0);

    // Nhận phản hồi từ server
    recv(client_socket, buffer, sizeof(buffer), 0);

    if (strcmp(buffer, "File not found") == 0) {
        printf("File not found on the server.\n");
    } else if (strcmp(buffer, "OK") == 0) {
        long file_size;
        recv(client_socket, &file_size, sizeof(file_size), 0);

        FILE *file = fopen(filename, "wb");
        while (file_size > 0) {
            int n = recv(client_socket, buffer, sizeof(buffer), 0);
            fwrite(buffer, 1, n, file);
            file_size -= n;
        }
        fclose(file);
        printf("File received successfully.\n");
    }

    // Kết thúc phiên làm việc
    send(client_socket, "BYE", sizeof("BYE"), 0);

    // Đóng kết nối
    close(client_socket);

    return 0;
}

