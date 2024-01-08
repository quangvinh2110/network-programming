// Nguyen Quang Vinh - 19020485
#include <stdio.h>
#include <string.h>
#include <ctype.h> 

int main() {
    // Read the input from the standard input stream and print out the number of characters, digits and symbols
    char str[100];
    int char_count=0;
    int digit_count=0;
    int symbol_count=0;

    printf("Enter a value :");
    scanf("%s", str);

    for(int i=0; i<strlen(str); i++) {
        if(isalpha(str[i])) char_count++;
        else if (isdigit(str[i])) digit_count++;
        else symbol_count++;
    }
    printf("\nInput string: %s", str);
    printf("\nNumber of characters: %d", char_count);
    printf("\nNumber of digits: %d", digit_count);
    printf("\nNumber of symbols: %d", symbol_count);
    return 0;
}
