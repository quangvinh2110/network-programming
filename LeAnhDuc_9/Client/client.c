//21020761 Le Anh Duc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void receive_file(FILE *file, int socket, long fileSize) {
    char buffer[MAX_BUFFER_SIZE];
    size_t bytesRead;
    long totalBytesRead = 0;

    while (totalBytesRead < fileSize) {
        bytesRead = read(socket, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            perror("Error reading from socket");
            exit(EXIT_FAILURE);
        }

        fwrite(buffer, 1, bytesRead, file);
        totalBytesRead += bytesRead;
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;

    // Input server IP address
    char serverIP[16];
    printf("Enter server IP address: ");
    scanf("%s", serverIP);

    if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_port = htons(8888);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    while (1) {
        char filename[256];
        FILE *file;
        long fileSize;

        // Input filename from user
        printf("Enter filename to download (or 'exit' to quit): ");
        scanf("%s", filename);

        if (strcmp(filename, "exit") == 0) {
            break;
        }

        // Send filename to server
        write(clientSocket, filename, strlen(filename));

        // Receive file size from server
        read(clientSocket, &fileSize, sizeof(fileSize));

        // Open the file
        file = fopen(filename, "wb");
        if (file == NULL) {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }

        // Receive file from server
        receive_file(file, clientSocket, fileSize);

        printf("File '%s' downloaded successfully. Size: %ld bytes\n", filename, fileSize);

        // Close the file
        fclose(file);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}

