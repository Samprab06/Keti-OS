#ifndef SHELL_H
#define SHELL_H

#define INPUT_SIZE 256

extern char input_buffer[INPUT_SIZE];
extern int input_pos;

struct command {
    const char *name;
    void (*handler)(char *args);
    const char *descp;
};

#define HISTORY_SIZE 10
extern char history[HISTORY_SIZE][INPUT_SIZE];
extern int history_count;
extern int history_pos; 

void shell_init();
void shell_handle_input(char c);
void shell_execute();

void cmd_help(char *args);
void cmd_clear(char *args);
void cmd_about(char *args);
void cmd_echo(char *args);
void cmd_timezone(char *args);
void cmd_clock(char *args);
void cmd_dst(char *args);
void cmd_wordle(char *args);
void testing (char *args);

void shell_down();
void shell_up();

#endif