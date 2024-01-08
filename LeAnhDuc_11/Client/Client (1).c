//21020761 Lê Anh Đức
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024];
    char client_id[10];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter your client ID: ");
    fgets(client_id, sizeof(client_id), stdin);
    strtok(client_id, "\n");

    // Gửi ID của client đến server
    send(sock, client_id, strlen(client_id), 0);

    while (1) {
        printf("Enter message (format: TargetID message): ");
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n");

        // Gửi tin nhắn có kèm ID của client
        send(sock, buffer, strlen(buffer), 0);

        // Đọc tin nhắn từ server
        valread = read(sock, buffer, sizeof(buffer));
        printf("Server: %s\n", buffer);
    }

    close(sock);

    return 0;
}

