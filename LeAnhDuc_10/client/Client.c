// client.c
// 21020761 Le Anh Duc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void receive_file(int server_socket, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    long file_size;
    recv(server_socket, &file_size, sizeof(file_size), 0);

    char buffer[BUFFER_SIZE];
    size_t bytesReceived;
    while (file_size > 0 && (bytesReceived = recv(server_socket, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytesReceived, file);
        file_size -= bytesReceived;
    }

    fclose(file);
}

void send_files(int server_socket, char **filenames, int file_count) {
    send(server_socket, &file_count, sizeof(file_count), 0);

    for (int i = 0; i < file_count; ++i) {
        send(server_socket, filenames[i], sizeof(filenames[i]), 0);
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    char server_ip[16];
    printf("Enter server IP address: ");
    scanf("%s", server_ip);
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    server_address.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    int file_count;
    printf("Enter the number of files to download: ");
    scanf("%d", &file_count);

    char **filenames = (char **)malloc(file_count * sizeof(char *));
    for (int i = 0; i < file_count; ++i) {
        filenames[i] = (char *)malloc(BUFFER_SIZE);
        printf("Enter file name %d: ", i + 1);
        scanf("%s", filenames[i]);
    }

    send_files(client_socket, filenames, file_count);

    for (int i = 0; i < file_count; ++i) {
        receive_file(client_socket, filenames[i]);
        printf("File received successfully: %s\n", filenames[i]);
    }

    // Free allocated memory
    for (int i = 0; i < file_count; ++i) {
        free(filenames[i]);
    }
    free(filenames);

    close(client_socket);

    return 0;
}

