#include "timer.h"
#include "ports.h"
#include "display/vga.h"
#include "lib/string.h"
#include "cpu/idt.h"
#include "process/scheduler.h"

extern void timer_isr();
char clock_str[16] = "        ";
unsigned int tick = 0;

int tz_offset_minutes = 0;   // start at UTC
int use_12_hour = 1;
int dst_offset = 0;
//1193182Hz at a 100Hz will be 11931
void timer_init(){
    unsigned int top = 11931;
    outb(0x43, 0x36);
    outb(0x40, top & 0xFF); // low byte
    outb(0x40, (top >> 8) & 0xFF); // high byte
    idt_fill(32, (unsigned int)timer_isr); // register handler
}

//handles the clock and accounts for the necessary requirements set
void timer_handler(){
    tick++;
    scheduler_tick();
    if (tick % 100 == 0) {
        unsigned char h = bcd_to_bin(read_rtc(4));
        unsigned char m = bcd_to_bin(read_rtc(2));
        unsigned char s = bcd_to_bin(read_rtc(0));
        
        // apply timezone offset
        int total_minutes = h * 60 + m + tz_offset_minutes + dst_offset;

        // wrap around 24 hours
        while (total_minutes < 0) total_minutes += 24 * 60;
        while (total_minutes >= 24 * 60) total_minutes -= 24 * 60;
        
        int hours_24 = total_minutes / 60;
        int minutes = total_minutes % 60;
        
        int display_hours = hours_24;
        const char *suffix = "";
        
        if (use_12_hour) {
            suffix = (hours_24 >= 12) ? " PM" : " AM";
            display_hours = hours_24 % 12;
            if (display_hours == 0) display_hours = 12;
        }
        //display buffer
        clock_str[0] = '0' + (display_hours / 10);
        clock_str[1] = '0' + (display_hours % 10);
        clock_str[2] = ':';
        clock_str[3] = '0' + (minutes / 10);
        clock_str[4] = '0' + (minutes % 10);
        clock_str[5] = ':';
        clock_str[6] = '0' + (s / 10);
        clock_str[7] = '0' + (s % 10);

        if (use_12_hour) {
            clock_str[8] = ' ';
            clock_str[9] = (hours_24 >= 12) ? 'P' : 'A';
            clock_str[10] = 'M';
            clock_str[11] = '\0';
        } else {
            clock_str[8] = '\0';
        }
        // clock_str[9] = '\0';
        write_pos(0, 65, clock_str);
        if (use_12_hour) {
            write_pos(0, 73, suffix);
        }
    }
    outb(0x20, 0x20);
}
//calls the inbuilt value and converts BCD to Binary int
unsigned char bcd_to_bin(unsigned char val) {
    return (val & 0x0F) + ((val >> 4) * 10);
}

//Reads a register from the CMOS Real Time Clock chip.
unsigned char read_rtc(unsigned char reg) {
    outb(0x70, reg);
    return inb(0x71);
}