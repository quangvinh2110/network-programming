#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER 1024

int main() {
    int sockfd, nread;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[MAX_BUFFER];
    FILE *file;

    // Tạo socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Không thể tạo socket");
        return 1;
    }

    // Định nghĩa địa chỉ server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Gán địa chỉ cho socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        perror("Không thể gán địa chỉ cho socket");
        return 1;
    }

    while (1) {
        printf("Đang chờ kết nối từ client...\n");

        // Nhận tên file từ client
        recvfrom(sockfd, buffer, MAX_BUFFER, 0, (struct sockaddr*)&client_addr, &addr_len);
        printf("Client yêu cầu file: %s\n", buffer);

        // Mở file
        if ((file = fopen(buffer, "rb")) == NULL) {
            perror("Không thể mở file");
            continue; // Nếu không mở được file, quay lại vòng lặp chính để chờ yêu cầu khác
        }

        // Gửi file cho client
        while ((nread = fread(buffer, 1, MAX_BUFFER, file)) > 0) {
            if (sendto(sockfd, buffer, nread, 0, (struct sockaddr*)&client_addr, addr_len) == -1) {
                perror("Lỗi khi gửi dữ liệu");
                break;
            }
        }

        fclose(file);
        printf("Đã gửi file thành công.\n");
    }

    close(sockfd);
    return 0;
}

