#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER];
    FILE *file;

    // Kiểm tra đầu vào
    if (argc != 3) {
        printf("Sử dụng: %s <Địa chỉ IP> <Tên file>\n", argv[0]);
        return 1;
    }

    // Tạo socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Không thể tạo socket");
        return 1;
    }

    // Định nghĩa địa chỉ server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    
    if (inet_aton(argv[1], &server_addr.sin_addr) == 0) {
        perror("Địa chỉ IP không hợp lệ");
        return 1;
    }

   // Gửi tên file cho server
   if (sendto(sockfd, argv[2], strlen(argv[2]) + 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
       perror("Lỗi khi gửi tên file");
       return 1;
   }

   // Nhận và lưu file
   if ((file = fopen(argv[2], "wb")) == NULL) {
       perror("Không thể mở file");
       return 1;
   }

   int nread;
   while ((nread = recvfrom(sockfd, buffer, MAX_BUFFER, 0, NULL, NULL)) > 0) {
       if (fwrite(buffer, nread, 1, file) != nread) {
           perror("Lỗi khi ghi file");
           break;
       }
   }

   fclose(file);
   
   printf("Đã nhận file thành công.\n");

   close(sockfd);
   return 0;
}

