#Lê Anh Đức                MSV: 21020761




#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 100

struct udphdr {
    unsigned short fileSize;    // file size
    unsigned char fileType;     // file type
    char fileName[MAX_NAME_LENGTH]; // file name
};

int main() {
    // Mở file pkt_hdr_json.txt để đọc
    FILE *jsonFile = fopen("pkt_hdr_json.txt", "r");
    if (jsonFile == NULL) {
        perror("Khong the mo file JSON");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), jsonFile) != NULL) {
        // Tìm các giá trị trong chuỗi JSON
        char *fileSizeStart = strstr(line, "\"fileSize\":");
        char *fileTypeStart = strstr(line, "\"fileType\":");
        char *fileNameStart = strstr(line, "\"fileName\":");
        
        // Kiểm tra các giá trị và in nó ra màn hình
        if (fileSizeStart != NULL) {
            int fileSize;
            if (sscanf(fileSizeStart, "\"fileSize\": %d", &fileSize) == 1) {
                printf("Kich thuoc file: %d byte\n", fileSize);
            }
        }

        if (fileTypeStart != NULL) {
            int fileType;
            if (sscanf(fileTypeStart, "\"fileType\": %d", &fileType) == 1) {
                printf("Kieu file: %s\n", (fileType == 0) ? "Text" : "Binary");
            }
        }

        if (fileNameStart != NULL) {
            char fileName[256];
            if (sscanf(fileNameStart, "\"fileName\": \"%[^\"]\"", fileName) == 1) {
                printf("Ten file: %s\n", fileName);
            }
        }
    }

    fclose(jsonFile);

    return 0;
}
