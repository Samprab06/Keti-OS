#include "string.h"

//compares two strings
int kstrcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) {
        a++;
        b++;
    }
    return *a - *b;
}

//conversion
void int_to_str(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char temp[12];
    int i = 0;
    while (n > 0) {
        temp[i++] = '0' + (n % 10);
        n /= 10;
    }
    // temp is in reverse; copy backwards into buf
    int j;
    for (j = 0; j < i; j++) {
        buf[j] = temp[i - 1 - j];
    }
    buf[j] = '\0';
}

// string length function 
int kstrlen (const char *str){
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}