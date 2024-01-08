//21020761 Le Anh Duc

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
// ham gui file

void receive_file(int server_socket, const char* file_name) {
    char file_info[256];
    recv(server_socket, file_info, sizeof(file_info), 0);

    char* token = strtok(file_info, " ");
    char* file_path = token;
    token = strtok(NULL, " ");
    long file_size = atol(token);

    FILE* file = fopen(file_name, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    long bytes_received = 0;
    size_t bytes_read;

    while (bytes_received < file_size) {
        bytes_read = recv(server_socket, buffer, sizeof(buffer), 0);
        fwrite(buffer, 1, bytes_read, file);
        bytes_received += bytes_read;
    }

    fclose(file);
    printf("Downloaded file: %s, size: %ld bytes\n", file_path, file_size);
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // tao socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);  // cong port
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP server

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error in connection");
        exit(1);
    }

    char command[256];
    char file_name[256];
// vong lap cho den khi client ket thuc yeu cau
    while (1) {
        printf("1. Download file\n");
        printf("2. Exit\n");
        printf("Choose an option: ");
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';  // xoa ki tu dong moi

        if (strcmp(command, "1") == 0) {
            send(client_socket, "download", sizeof("download"), 0);
            printf("Enter the file path: ");
            fgets(file_name, sizeof(file_name), stdin);
            file_name[strlen(file_name) - 1] = '\0';  // xoa ki tu dong moi
            send(client_socket, file_name, sizeof(file_name), 0);
            receive_file(client_socket, file_name);
        } else if (strcmp(command, "2") == 0) {
            send(client_socket, "exit", sizeof("exit"), 0);
            break;
        }
    }

    close(client_socket);

    return 0;
}

