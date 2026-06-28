#include "rand.h"
#include "drivers/timer.h"

extern unsigned int tick;

static unsigned int rand_state = 0;

static void rand_seed(void) {
    unsigned int seconds = bcd_to_bin(read_rtc(0));
    unsigned int minutes = bcd_to_bin(read_rtc(2));
    unsigned int hours = bcd_to_bin(read_rtc(4));
    rand_state = (hours << 16) | (minutes << 8) | seconds;
    rand_state ^= tick;
    if (rand_state == 0) {
        rand_state = 1;
    }
}

int rand(void) {
    if (rand_state == 0) {
        rand_seed();
    }
    rand_state = rand_state * 1103515245u + 12345u;
    return (int)((rand_state >> 16) & 0x7FFF);
}
