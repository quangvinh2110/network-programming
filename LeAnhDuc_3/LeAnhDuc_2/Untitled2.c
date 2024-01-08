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

    // Mở file pkt_hdr.bin để đọc
    FILE *file = fopen("pkt_hdr.bin", "rb");
    if (file == NULL) {
        perror("Khong the mo file");
        return 1;
    }

    // Đọc thông tin từ file
    fread(&header, sizeof(struct udphdr), 1, file);
    fclose(file);

    // Xuất thông tin
    printf("Kich thuoc file: %hu byte\n", header.fileSize);
    printf("Kieu file: %s\n", (header.fileType == 0) ? "Text" : "Binary");
    printf("Do dai ten file: %hhu\n", header.fileNameLength);
    printf("Ten file: %s\n", header.fileName);

    return 0;
}