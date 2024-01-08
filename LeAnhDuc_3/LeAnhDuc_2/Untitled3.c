#Lê Anh Đức                MSV: 21020761



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 100

struct udphdr {
    unsigned short fileSize;    // file size
    unsigned char fileType;     // file type
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

    // Tạo một chuỗi JSON từ thông tin đầu vào
    char json[256];
    snprintf(json, sizeof(json), "{\"fileSize\": %hu, \"fileType\": %hhu, \"fileName\": \"%s\"}", header.fileSize, header.fileType, header.fileName);

    // Lưu chuỗi JSON vào file pkt_hdr_json.txt
    FILE *jsonFile = fopen("pkt_hdr_json.txt", "w");
    if (jsonFile == NULL) {
        perror("Khong the mo file JSON");
        return 1;
    }
    fprintf(jsonFile, "%s\n", json);
    fclose(jsonFile);

    printf("Da luu thong tin vao file pkt_hdr_json.txt\n");

    // So sánh kích thước của pkt_hdr.bin và pkt_hdr_json.txt
    FILE *binFile = fopen("pkt_hdr.bin", "rb");
    if (binFile == NULL) {
        perror("Khong the mo file pkt_hdr.bin");
        return 1;
    }
    fseek(binFile, 0, SEEK_END);
    long binSize = ftell(binFile);
    fclose(binFile);

    FILE *jsonSizeFile = fopen("pkt_hdr_json.txt", "rb");
    if (jsonSizeFile == NULL) {
        perror("Khong the mo file pkt_hdr_json.txt");
        return 1;
    }
    fseek(jsonSizeFile, 0, SEEK_END);
    long jsonSize = ftell(jsonSizeFile);
    fclose(jsonSizeFile);
    
    // Kiểm tra độ lớn của 2 file
    if (binSize == jsonSize) {
        printf("Kich thuoc cua pkt_hdr.bin va pkt_hdr_json.txt la giong nhau (%ld byte).\n", binSize);
    } else {
        printf("Kich thuoc cua pkt_hdr.bin (%ld byte) khac voi pkt_hdr_json.txt (%ld byte).\n", binSize, jsonSize);
    }

    return 0;
}
