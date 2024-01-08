// Nguyen Quang Vinh - 19020485
#include <stdio.h>
#include <stdlib.h>

int main() {
    // reading pixel data from image file and write to another file
    FILE *input_file = fopen("images.jpeg", "rb");
    fseek(input_file, 0, SEEK_END);
    long input_size = ftell(input_file);
    rewind(input_file);
    unsigned char *pixel_data = malloc(input_size);
    fread(pixel_data, sizeof(unsigned char), input_size, input_file);

    FILE *output_file = fopen("output.jpg", "wb");
    fwrite(pixel_data, sizeof(unsigned char), input_size, output_file);
    free(pixel_data);
    
    fclose(input_file);
    fclose(output_file);

    return 0;
}