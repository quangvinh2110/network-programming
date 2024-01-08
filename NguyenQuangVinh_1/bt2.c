// Nguyen Quang Vinh - 19020485
#include <stdio.h>

long int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    fclose(fp); 
  
    return res; 
} 

int main() {
    // First, read the file name, then read an input string until "\n"
    char filename[100];
    char input_str[100];

    printf("Enter filename: ");
    scanf("%s", filename);
    getchar();
    printf("Enter input string: ");
    scanf("%[^\n]s", input_str);

    printf("Filename: %s", filename);
    printf("\nInput string: %s", input_str);

    // Write that string into a file named file name and print out the size of that file
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening the file %s", filename);
        return -1;
    }
    fprintf(fp, "%s", input_str);
    fclose(fp);

    // Find the size of file
    long int res = findSize(filename); 
    if (res != -1) 
        printf("\nSize of the file is %ld bytes \n", res); 
    else printf("\nError!!!!!!!!!!");
    return 0;
}