/*VGA memory starts at 0xB8000
Each character is 2 bytes - ASCII then color
Screen is 80 columns × 25 rows
Color 0x0F is white on black*/
#include "vga.h"
int cursor_row = 0;
int cursor_col = 0;

//so this function will be used to display anything string we want to print onto the OS for now
//old function jsut statically printed regardless of anything and overwrote, now we will implement cursor technique
void print_vga(char *str){
    // int rows = 25;
    // int col = 80;
    
    char *out = (char *)0xB8000;
    int i=0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            cursor_col = 0;
            cursor_row++;
            i++;
            continue;
        }
        int index = (cursor_row*80 + cursor_col)*2;
        out[index] = str[i];
        out[index + 1] = 0x0F;
        i++;
        cursor_col++;
        if (cursor_col==80){
            cursor_col=0;
            cursor_row++;
        }

    }
}

void print_char_vga(char c){
    char *out = (char *)0xB8000;
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        return;
    }
    int index = (cursor_row*80 + cursor_col)*2;
    out[index] = c;
    out[index + 1] = 0x0F;
    cursor_col++;
    if (cursor_col==80){
        cursor_col=0;
        cursor_row++;
    }
}

void print_hex_vga(unsigned int n){
    print_vga("0x");
    for (int i = 28; i>=0; i-=4){
        //keeps digit between 0 to 15
        int digit = (n >> i) & 0xF;
        char c;
        if (digit<10){
            c = '0' + digit;
        }
        else{
            c = 'A' + (digit - 10);
        }
        print_char_vga(c);
    }
}


void delete_char_vga(){
    if (cursor_col > 0) {
        cursor_col--;
    }
    char *out = (char *)0xB8000;
    int index = (cursor_row*80 + cursor_col)*2;
    out[index] = ' ';
    out[index + 1] = 0x0F;
    if (cursor_col == 0) {
        cursor_row--;
        cursor_col = 79; //Now, being the smart person you are, you would have noticed that this will take you to the end of the line regardless of if you typed till the end of the line (so you are deleting nothing)
        //fix? i dont know \(*.*)/
        //ill come back to it
    } // a quick thought since i cant just keep pressing the cursor for each row, when you reach beginning of the line, a delete will take you up.
}

//self explanatory, clears it
void clear_vga(){
    char *out = (char *)0xB8000;
    int i = 0;
    //2000 because thats the total available characters (4000 bytes, cuz each is 2bytes)
    while (i<2000) {
        out[i*2] = ' ';
        out[i*2 + 1] = 0x0F;
        i++;
    }
}