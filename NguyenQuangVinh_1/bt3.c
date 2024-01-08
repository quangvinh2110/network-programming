// Nguyen Quang Vinh - 19020485
#include <stdio.h>


int main() {
    // Read strings from a file and write those to another file
    char filename_r[10] = "tmp1.txt";
    char filename_w[10] = "tmp2.txt";
    FILE *fp_r = fopen(filename_r, "r");
    FILE *fp_w = fopen(filename_w, "w");
    char string[100];
    while(fgets(string, 100, fp_r)) {
        fseek (fp_w, 0, SEEK_CUR);  
        fputs(string, fp_w);
    }
    fclose(fp_r);
    fclose(fp_w);
    return 0;
}