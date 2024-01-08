//21020761 Lê Anh Đức
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 2

typedef struct {
    int socket;
    char id[10];
} Client;

int findClientIndex(Client clients[], int clientCount, char *targetId) {
    for (int i = 0; i < clientCount; ++i) {
        if (strcmp(clients[i].id, targetId) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    int master_socket, addrlen, new_socket, activity, valread;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[1025] = {0};

    Client clients[MAX_CLIENTS];

    // Initialize client sockets
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i].socket = 0;
        strcpy(clients[i].id, "");
    }

    // Create master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(master_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    printf("Server listening on port %d...\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);

        int max_sd = master_socket;

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int sd = clients[i].socket;

            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            perror("Select error");
        }

        if (FD_ISSET(master_socket, &readfds)) {
            if ((new_socket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, ip is : %s, port : %d\n",
                   new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            // Lưu thông tin client mới vào mảng clients
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (clients[i].socket == 0) {
                    clients[i].socket = new_socket;

                    // Đọc ID của client từ kết nối mới
                    if ((valread = read(new_socket, buffer, sizeof(buffer))) > 0) {
                        buffer[valread] = '\0';
                        strcpy(clients[i].id, buffer);
                        printf("Client %s connected\n", clients[i].id);
                    } else {
                        perror("Failed to read client ID");
                        close(new_socket);
                        clients[i].socket = 0;
                    }

                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int sd = clients[i].socket;

            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, sizeof(buffer))) == 0) {
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("Client %s disconnected, ip %s, port %d\n", clients[i].id, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    close(sd);

                    // Xóa thông tin client ra khỏi mảng clients
                    clients[i].socket = 0;
                    strcpy(clients[i].id, "");
                } else {
                    buffer[valread] = '\0';

                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("Received message from %s, port %d: %s\n", clients[i].id, ntohs(address.sin_port), buffer);

                    // Phân tích tin nhắn để xác định mục tiêu và chuyển tiếp cho mục tiêu
                    char targetId[10];
                    sscanf(buffer, "%s", targetId);
                    int targetIndex = findClientIndex(clients, MAX_CLIENTS, targetId);

                    if (targetIndex != -1) {
                        int dest_sd = clients[targetIndex].socket;
                        if (dest_sd > 0 && dest_sd != sd) {
                            send(dest_sd, buffer, strlen(buffer), 0);
                        }
                    } else {
                        printf("Target client %s not found\n", targetId);
                    }
                }
            }
        }
    }

    return 0;
}

