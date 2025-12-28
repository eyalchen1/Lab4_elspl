#include <stdio.h>
int count_digits(char* string){
    int count = 0;
    while (*string) {
        if (*string >= '0' && *string <= '9') {
            count++;
        }
        string++;
    }
    return count;
}


int main(int argc, char** argv){
    int count=count_digits(argv[1]);
    printf("The number of digits in the string is: %d\n",count);
}