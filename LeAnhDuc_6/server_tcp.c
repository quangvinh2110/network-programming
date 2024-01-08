//21020761 Le Anh Duc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


// ham gui file
void send_file(int client_socket, const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("File not found: %s\n", file_path);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char file_info[256];
    snprintf(file_info, sizeof(file_info), "%s %ld", file_path, file_size);
    send(client_socket, file_info, sizeof(file_info), 0);

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_addr_len = sizeof(client_address);

    // tao socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);  // cong port
    server_address.sin_addr.s_addr = INADDR_ANY;

    // lien ket den socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    // lang nghe toi da 5 client
    if (listen(server_socket, 5) < 0) {
        perror("Error in listening");
        exit(1);
    }

    printf("Server is waiting for connections...\n");

    // chap nhan ket noi client
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);

    char command[256];
    char file_path[256];
// vong lap cho den khi client ket thuc yeu cau
    while (1) {
        recv(client_socket, command, sizeof(command), 0);

        if (strcmp(command, "download") == 0) {
            recv(client_socket, file_path, sizeof(file_path), 0);
            send_file(client_socket, file_path);
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    close(client_socket);
    close(server_socket);

    return 0;
}

