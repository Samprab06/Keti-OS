#include "wordle.h"
#include "display/vga.h"
#include "shell.h"
#include "lib/rand.h"
#include "lib/string.h"

static const char *word_list[] = {
    "apple", "grape", "crown", "stone", "hello",
    "brick", "chair", "dream", "flame", "globe",
    "house", "juice", "knife", "lemon", "magic"
};

static int game_active = 0;
static int guesses_taken = 0;
static int secret_index = 0;

static int word_length(const char *s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

static int choose_word_index(void) {
    int count = sizeof(word_list) / sizeof(word_list[0]);
    return rand() % count;
}

static int contains_char(const char *word, char c) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == c) return 1;
    }
    return 0;
}

static void print_colored_guess(const char *guess, const char *secret) {
    int i = 0;
    while (guess[i] != '\0' && secret[i] != '\0') {
        unsigned char color = 0x0F;
        if (guess[i] == secret[i]) {
            color = 0x2A; // green
        } else if (contains_char(secret, guess[i])) {
            color = 0x0E; // yellow
        } else {
            color = 0x07; // gray
        }
        print_char_vga_color(guess[i], color);
        i++;
    }
    print_char_vga('\n');
}

static void start_game(void) {
    secret_index = choose_word_index();
    guesses_taken = 0;
    game_active = 1;
    print_vga("Wordle started. Enter guesses with: wordle <5-letter-word>\n");
}

void cmd_wordle(char *args) {
    if (!game_active) {
        start_game();
    }

    if (args == 0 || args[0] == '\0') {
        return;
    }

    if (kstrcmp(args, "reset") == 0) {
        game_active = 0;
        start_game();
        return;
    }

    if (word_length(args) != 5) {
        print_vga("Guess must be 5 letters long\n");
        return;
    }

    guesses_taken++;
    const char *secret = word_list[secret_index];

    if (kstrcmp(args, secret) == 0) {
        print_vga("Correct! You win!\n");
        game_active = 0;
        return;
    }

    print_colored_guess(args, secret);

    if (guesses_taken >= 6) {
        print_vga("Game over. The word was: ");
        print_vga(secret);
        print_char_vga('\n');
        game_active = 0;
    } else {
        print_vga("Try again.\n");
    }
}
