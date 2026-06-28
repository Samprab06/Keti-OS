/*VGA memory starts at 0xB8000
Each character is 2 bytes - ASCII then color
Screen is 80 columns x 25 rows
Color 0x0F is white on black

Everything printed goes into a tall virtual buffer (screen_buf).
VGA memory just shows a 25-row window into that buffer.
Scrolling = moving the window (view_top). No shifting of VGA memory.
*/
#include "vga.h"
#include "drivers/ports.h"
#include "lib/string.h"
#include "drivers/timer.h"

#define TOTAL_ROWS 1000     // how many rows of history we keep
#define COLS 80
#define SCREEN_ROWS 25

//the tall virtual screen
char screen_buf[TOTAL_ROWS][COLS];
unsigned char color_buf[TOTAL_ROWS][COLS];

int cursor_row = 0; //absolute row in screen_buf where cursor is
int cursor_col = 0; //column where cursor is
int view_top = 0; //which buffer row is shown at top of the screen
int viewing_live = 1; //1 = following live output, 0 = scrolled back
extern char clock_str[];
void render();

//Updates the hardware blinking cursor position via VGA controller ports
//You can controll the VGA's cursor by writing to 0x3D4 (index),and 0x3D5(data)
void update_cursor(int row, int col) {
    //only show hardware cursor when viewing live
    if (!viewing_live) {
        //park cursor off-screen when scrolled back
        outb(0x3D4, 0x0F);
        outb(0x3D5, 0xFF);
        outb(0x3D4, 0x0E);
        outb(0x3D5, 0xFF);
        return;
    }
    int screen_row = row - view_top;
    if (screen_row < 0 || screen_row >= SCREEN_ROWS) return;
    unsigned short pos = screen_row * COLS + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

//copy the 25-row window starting at view_top into VGA memory
//This is the only place that touches VGA memory directly for text output, since everthing else talks to screen_buf first
void render() {
    char *out = (char *)0xB8000;
    for (int r = 0; r < SCREEN_ROWS; r++) {
        int buf_row = view_top + r;
        for (int c = 0; c < COLS; c++) {
            int vga_index = (r * COLS + c) * 2;
            if (buf_row >= 0 && buf_row < TOTAL_ROWS) {
                out[vga_index] = screen_buf[buf_row][c];
                out[vga_index + 1] = color_buf[buf_row][c];
            } else {
                out[vga_index] = ' ';
                out[vga_index + 1] = 0x0F;
            }
        }
    }
    write_pos(0, 65, clock_str);
}

//when the cursor moves past the bottom of history, shift everything up
//(only happens once we fill all TOTAL_ROWS - rare, but keeps it safe)
static void scroll_buffer_up() {
    for (int r = 1; r < TOTAL_ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            screen_buf[r - 1][c] = screen_buf[r][c];
            color_buf[r - 1][c] = color_buf[r][c];
        }
    }
    // clear last row
    for (int c = 0; c < COLS; c++) {
        screen_buf[TOTAL_ROWS - 1][c] = ' ';
        color_buf[TOTAL_ROWS - 1][c] = 0x0F;
    }
    cursor_row--;
}

//snap the view back to live (showing the most recent 25 rows)
static void snap_to_live() {
    viewing_live = 1;
    view_top = cursor_row - (SCREEN_ROWS - 1);
    if (view_top < 0) view_top = 0;
}

// advance cursor to a new line
static void newline() {
    cursor_col = 0;
    cursor_row++;
    if (cursor_row >= TOTAL_ROWS) {
        scroll_buffer_up();
    }
}

// write one character into the buffer at the cursor
static void put_char(char c) {
    if (c == '\n') {
        newline();
        return;
    }
    screen_buf[cursor_row][cursor_col] = c;
    color_buf[cursor_row][cursor_col] = 0x0F;
    cursor_col++;
    if (cursor_col >= COLS) {
        newline();
    }
}
//print_ anything is just a push of printf in C since we dont have stdlib.

//prints a line or a string of works
void print_vga(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        put_char(str[i]);
        i++;
    }
    snap_to_live();
    render();
    update_cursor(cursor_row, cursor_col);
}

//prints a single character
void print_char_vga(char c) {
    put_char(c);
    snap_to_live();
    render();
    update_cursor(cursor_row, cursor_col);
}

//prints hex 
void print_hex_vga(unsigned int n) {
    print_vga("0x");
    for (int i = 28; i >= 0; i -= 4) {
        int digit = (n >> i) & 0xF;
        char c;
        if (digit < 10) {
            c = '0' + digit;
        } else {
            c = 'A' + (digit - 10);
        }
        print_char_vga(c);
    }
}

//backspace
void delete_char_vga() {
    if (cursor_col > 0) {
        cursor_col--;
    } else if (cursor_row > 0) {
        cursor_row--;
        cursor_col = COLS - 1;
    }
    screen_buf[cursor_row][cursor_col] = ' ';
    color_buf[cursor_row][cursor_col] = 0x0F;
    snap_to_live();
    render();
    update_cursor(cursor_row, cursor_col);
}

//clears the whole screen
void clear_vga() {
    // wipe the whole buffer and reset to the top
    for (int r = 0; r < TOTAL_ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            screen_buf[r][c] = ' ';
            color_buf[r][c] = 0x0F;
        }
    }
    cursor_row = 0;
    cursor_col = 0;
    view_top = 0;
    viewing_live = 1;
    render();
    update_cursor(cursor_row, cursor_col);
}

//write directly at a fixed screen position (for the clock).
//this writes to VGA memory directly and does NOT touch the buffer,
//so it stays pinned to the top regardless of scrolling.
//so currently
void write_pos(int row, int col, const char *str) {
    char *out = (char *)0xB8000;
    int i = 0;
    while (str[i] != '\0') {
        int index = (row * COLS + col + i) * 2;
        out[index] = str[i];
        out[index + 1] = 0x0F;
        i++;
    }
}

//scrollback navigation
void scrollback_up() {
    // move the window up (toward older history)
    if (view_top > 0) {
        view_top--;
        viewing_live = 0;
        render();
        update_cursor(cursor_row, cursor_col);
    }
}


void scrollback_down() {
    // move the window down (toward the present)
    int live_top = cursor_row - (SCREEN_ROWS - 1);
    if (live_top < 0) live_top = 0;

    if (view_top < live_top) {
        view_top++;
        if (view_top >= live_top) {
            viewing_live = 1;
        }
        render();
        update_cursor(cursor_row, cursor_col);
    }
}