// Le Anh Duc - 21020761
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void *handle_connection(void *client_socket) {
    int client_sock = *((int *)client_socket);
    char buffer[1024];

    // Đọc lệnh từ client
    recv(client_sock, buffer, sizeof(buffer), 0);

    if (strcmp(buffer, "DOWNLOAD") == 0) {
        char filename[100];
        // Đọc tên file từ client
        recv(client_sock, filename, sizeof(filename), 0);

        // Kiểm tra file và gửi hoặc báo lỗi
        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            send(client_sock, "File not found", sizeof("File not found"), 0);
        } else {
            send(client_sock, "OK", sizeof("OK"), 0);

            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            send(client_sock, &file_size, sizeof(file_size), 0);

            while (file_size > 0) {
                int n = fread(buffer, 1, sizeof(buffer), file);
                send(client_sock, buffer, n, 0);
                file_size -= n;
            }

            fclose(file);
        }
    }

    close(client_sock);
    free(client_socket);
    return NULL;
}

int main() {
//tao socket
    int server_socket;
    struct sockaddr_in server_address;
    socklen_t addr_size = sizeof(server_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = INADDR_ANY;
// lang nghe client neu ket noi thanh cong
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_socket, 10);
    printf("Server is listening...\n");

    while (1) {
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr *)&server_address, &addr_size);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_connection, (void *)client_socket);
    }

    close(server_socket);
    return 0;
}

