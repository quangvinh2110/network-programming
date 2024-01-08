//21020761 Le Anh Duc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void send_file(FILE *file, int socket) {
    char buffer[MAX_BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        write(socket, buffer, bytesRead);
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8888);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Waiting for incoming connections...\n");

        // Accept connection from client
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket == -1) {
            perror("Error accepting connection");
            continue;
        }

        // Communicate with the client
        char filename[256];
        FILE *file;

        // Receive filename from client
        read(clientSocket, filename, sizeof(filename));
        filename[strlen(filename)] = '\0';

        // Open the file
        file = fopen(filename, "rb");
        if (file == NULL) {
            perror("Error opening file");
            close(clientSocket);
            continue;
        }

        // Get file size
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Send file size to client
        write(clientSocket, &fileSize, sizeof(fileSize));

        // Send file to client
        send_file(file, clientSocket);

        // Close file and socket
        fclose(file);
        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}

