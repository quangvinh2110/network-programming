#Lê Anh Đức                MSV: 21020761

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 100

struct udphdr {
    unsigned short fileSize;    // file size
    unsigned char fileType;     // file type
    unsigned char fileNameLength; // file name length
    char fileName[MAX_NAME_LENGTH]; // file name
};

int main() {
    struct udphdr header;
    
    // Nhập thông tin về kích thước file, kiểu file, tên file
    printf("Nhap kich thuoc file (byte): ");
    scanf("%hu", &header.fileSize);
    
    printf("Nhap kieu file (0 - text file, 1 - binary file): ");
    scanf("%hhu", &header.fileType);
    
    printf("Nhap ten file: ");
    scanf("%s", header.fileName);
    
    // Tính độ dài của tên file
    header.fileNameLength = (unsigned char)strlen(header.fileName);
    
    // Ghi thông tin vào file pkt_hdr.bin
    FILE *file = fopen("pkt_hdr.bin", "wb");
    if (file == NULL) {
        perror("Khong the mo file");
        return 1;
    }
    
    fwrite(&header, sizeof(struct udphdr), 1, file);
    fclose(file);
    // In kích thước file
    printf("Kich thuoc file la: %hu byte\n", sizeof(struct udphdr));
    
    return 0;
}
