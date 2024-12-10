#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define ANSI_IMPLEMENTATION
#include "ansi.h"

#ifndef ANSI_VERSION
    #error "menu.h depends on ansi.h"
#endif

#define MENU_VERSION_MAJOR 0
#define MENU_VERSION_MINOR 0
#define MENU_VERSION_PATCH 1
#define MENU_VERSION "0.0.1"

#ifndef MENUAPI
    #define MENUAPI
#endif // MENUAPI

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>

    MENUAPI int _getch();
#endif

// TODO: Add more keys
typedef enum {
    // arrows (defined later)
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,

    KEY_UNKNOWN = -1,
    KEY_ENTER = 10,
    KEY_BACKSPACE = 127,
    KEY_TAB = 9,
    KEY_ESC = 27,
    KEY_SPACE = 32,

    // numbers
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',

    // uppercase
    KEY_A = 'A',
    KEY_B = 'B',
    KEY_C = 'C',
    KEY_D = 'D',
    KEY_E = 'E',
    KEY_F = 'F',
    KEY_G = 'G',
    KEY_H = 'H',
    KEY_I = 'I',
    KEY_J = 'J',
    KEY_K = 'K',
    KEY_L = 'L',
    KEY_M = 'M',
    KEY_N = 'N',
    KEY_O = 'O',
    KEY_P = 'P',
    KEY_Q = 'Q',
    KEY_R = 'R',
    KEY_S = 'S',
    KEY_T = 'T',
    KEY_U = 'U',
    KEY_V = 'V',
    KEY_W = 'W',
    KEY_X = 'X',
    KEY_Y = 'Y',
    KEY_Z = 'Z',

    // lowercase
    KEY_a = 'a',
    KEY_b = 'b',
    KEY_c = 'c',
    KEY_d = 'd',
    KEY_e = 'e',
    KEY_f = 'f',
    KEY_g = 'g',
    KEY_h = 'h',
    KEY_i = 'i',
    KEY_j = 'j',
    KEY_k = 'k',
    KEY_l = 'l',
    KEY_m = 'm',
    KEY_n = 'n',
    KEY_o = 'o',
    KEY_p = 'p',
    KEY_q = 'q',
    KEY_r = 'r',
    KEY_s = 's',
    KEY_t = 't',
    KEY_u = 'u',
    KEY_v = 'v',
    KEY_w = 'w',
    KEY_x = 'x',
    KEY_y = 'y',
    KEY_z = 'z',

    // special characters
    KEY_EXCLAMATION = '!',
    KEY_AT = '@',
    KEY_HASH = '#',
    KEY_DOLLAR = '$',
    KEY_PERCENT = '%',
    KEY_CARET = '^',
    KEY_AMPERSAND = '&',
    KEY_ASTERISK = '*',
    KEY_LEFT_PAREN = '(',
    KEY_RIGHT_PAREN = ')',
    KEY_DASH = '-',
    KEY_UNDERSCORE = '_',
    KEY_EQUAL = '=',
    KEY_PLUS = '+',
    KEY_LEFT_BRACKET = '[',
    KEY_RIGHT_BRACKET = ']',
    KEY_LEFT_BRACE = '{',
    KEY_RIGHT_BRACE = '}',
    KEY_SEMICOLON = ';',
    KEY_COLON = ':',
    KEY_SINGLE_QUOTE = '\'',
    KEY_DOUBLE_QUOTE = '"',
    KEY_COMMA = ',',
    KEY_PERIOD = '.',
    KEY_LESS = '<',
    KEY_GREATER = '>',
    KEY_SLASH = '/',
    KEY_QUESTION = '?',
    KEY_BACKSLASH = '\\',
    KEY_PIPE = '|',
    KEY_BACKTICK = '`',
    KEY_TILDE = '~',
} ClibKey;

typedef void (*MenuPrintOptionFunc)(const char* option, int is_selected, int color);

MENUAPI void menu_default_print_option(const char* option, int is_selected, int color);
MENUAPI void menu_arrow_print_option(const char* option, int is_selected, int color);
MENUAPI void menu_brackets_print_option(const char* option, int is_selected, int color);

MENUAPI void menu_enable_input_buffering();
MENUAPI void menu_disable_input_buffering();
MENUAPI int menu_getch();

MENUAPI int menu(const char* title, int color, MenuPrintOptionFunc print_option, char* first_option, ...);

#ifdef MENU_IMPLEMENTATION
#ifndef _WIN32
    MENUAPI int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

MENUAPI void menu_enable_input_buffering(){
    #ifdef _WIN32
        // Enable console input buffering
        HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

        DWORD consoleMode;
        GetConsoleMode(hConsoleInput, &consoleMode);

        consoleMode |= ENABLE_LINE_INPUT;

        SetConsoleMode(hConsoleInput, consoleMode);
    #else
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

    #endif
    ANSI_SHOW_CURSOR();
}

MENUAPI void menu_disable_input_buffering(){
    #ifdef _WIN32
        // Disable console input buffering
        DWORD mode;
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(hInput, &mode);
        SetConsoleMode(hInput, mode & ~ENABLE_ECHO_INPUT & ~ENABLE_LINE_INPUT);
    #else
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

    #endif
    ANSI_HIDE_CURSOR();
}

MENUAPI int menu_getch() {
    int ch;
    #ifdef _WIN32
        ch = _getch();
        if (ch == 0 || ch == 224) {
            // Handle extended keys (arrows, function keys)
            ch = _getch();
            switch (ch) {
                case 72: return KEY_ARROW_UP;
                case 80: return KEY_ARROW_DOWN;
                case 75: return KEY_ARROW_LEFT;
                case 77: return KEY_ARROW_RIGHT;
                default: return KEY_UNKNOWN;
            }
        }
    #else
        ch = _getch();
        if (ch == 27) {
            if (getchar() == '[') {
                switch (getchar()) {
                    case 'A': return KEY_ARROW_UP;
                    case 'B': return KEY_ARROW_DOWN;
                    case 'C': return KEY_ARROW_RIGHT;
                    case 'D': return KEY_ARROW_LEFT;
                    default: return KEY_UNKNOWN;
                }
            }
            return KEY_ESC;
        }
    #endif

    switch (ch) {
        case 10: return KEY_ENTER;
        case 127: return KEY_BACKSPACE;
        case 9: return KEY_TAB;
        case 27: return KEY_ESC;
        case 32: return KEY_SPACE;

        // digits
        case '0': return KEY_0;
        case '1': return KEY_1;
        case '2': return KEY_2;
        case '3': return KEY_3;
        case '4': return KEY_4;
        case '5': return KEY_5;
        case '6': return KEY_6;
        case '7': return KEY_7;
        case '8': return KEY_8;
        case '9': return KEY_9;

        // uppercase
        case 'A': return KEY_A;
        case 'B': return KEY_B;
        case 'C': return KEY_C;
        case 'D': return KEY_D;
        case 'E': return KEY_E;
        case 'F': return KEY_F;
        case 'G': return KEY_G;
        case 'H': return KEY_H;
        case 'I': return KEY_I;
        case 'J': return KEY_J;
        case 'K': return KEY_K;
        case 'L': return KEY_L;
        case 'M': return KEY_M;
        case 'N': return KEY_N;
        case 'O': return KEY_O;
        case 'P': return KEY_P;
        case 'Q': return KEY_Q;
        case 'R': return KEY_R;
        case 'S': return KEY_S;
        case 'T': return KEY_T;
        case 'U': return KEY_U;
        case 'V': return KEY_V;
        case 'W': return KEY_W;
        case 'X': return KEY_X;
        case 'Y': return KEY_Y;
        case 'Z': return KEY_Z;

        // lowercase
        case 'a': return KEY_a;
        case 'b': return KEY_b;
        case 'c': return KEY_c;
        case 'd': return KEY_d;
        case 'e': return KEY_e;
        case 'f': return KEY_f;
        case 'g': return KEY_g;
        case 'h': return KEY_h;
        case 'i': return KEY_i;
        case 'j': return KEY_j;
        case 'k': return KEY_k;
        case 'l': return KEY_l;
        case 'm': return KEY_m;
        case 'n': return KEY_n;
        case 'o': return KEY_o;
        case 'p': return KEY_p;
        case 'q': return KEY_q;
        case 'r': return KEY_r;
        case 's': return KEY_s;
        case 't': return KEY_t;
        case 'u': return KEY_u;
        case 'v': return KEY_v;
        case 'w': return KEY_w;
        case 'x': return KEY_x;
        case 'y': return KEY_y;
        case 'z': return KEY_z;

        // special characters
        case '!': return KEY_EXCLAMATION;
        case '@': return KEY_AT;
        case '#': return KEY_HASH;
        case '$': return KEY_DOLLAR;
        case '%': return KEY_PERCENT;
        case '^': return KEY_CARET;
        case '&': return KEY_AMPERSAND;
        case '*': return KEY_ASTERISK;
        case '(': return KEY_LEFT_PAREN;
        case ')': return KEY_RIGHT_PAREN;
        case '-': return KEY_DASH;
        case '_': return KEY_UNDERSCORE;
        case '=': return KEY_EQUAL;
        case '+': return KEY_PLUS;
        case '[': return KEY_LEFT_BRACKET;
        case ']': return KEY_RIGHT_BRACKET;
        case '{': return KEY_LEFT_BRACE;
        case '}': return KEY_RIGHT_BRACE;
        case ';': return KEY_SEMICOLON;
        case ':': return KEY_COLON;
        case '\'': return KEY_SINGLE_QUOTE;
        case '"': return KEY_DOUBLE_QUOTE;
        case ',': return KEY_COMMA;
        case '.': return KEY_PERIOD;
        case '<': return KEY_LESS;
        case '>': return KEY_GREATER;
        case '/': return KEY_SLASH;
        case '?': return KEY_QUESTION;
        case '\\': return KEY_BACKSLASH;
        case '|': return KEY_PIPE;
        case '`': return KEY_BACKTICK;
        case '~': return KEY_TILDE;

        default: return ch;
    }
}


MENUAPI void menu_default_print_option(const char* option, int is_selected, int color){
    char c[32];
    ANSI_COLOR_BG(c, color);
    is_selected ? printf("%s%s%s", c, option, ANSI_RESET) : printf("%s", option);
}

MENUAPI void menu_arrow_print_option(const char* option, int is_selected, int color){
    char c[32];
    ANSI_COLOR_FG(c, color);
    is_selected ? printf("%s>%s %s", c, ANSI_RESET, option) : printf("  %s", option);
}

MENUAPI void menu_brackets_print_option(const char* option, int is_selected, int color)
{
    char c[32];
    ANSI_COLOR_FG(c, color);
    is_selected ? printf("%s[%s%s%s]%s", c, ANSI_RESET, option, c, ANSI_RESET) : printf(" %s ", option);
}

MENUAPI int menu(const char* title, int color, MenuPrintOptionFunc print_option, char* first_option, ...)
{
    if (first_option == NULL) return -1;

    menu_disable_input_buffering();

    int selected = 0;
    size_t option_count = 0;

    va_list args;
    va_start(args, first_option);
    for (char* opt = first_option; opt != NULL; opt = va_arg(args, char*)) {
        option_count++;
    }
    va_end(args);

    char** options = (char**) malloc(option_count * sizeof(char*));
    if (options == NULL) {
        menu_enable_input_buffering();
        return -1; // Allocation failed
    }

    va_start(args, first_option);
    options[0] = first_option; // Add first option
    for (size_t i = 1; i < option_count; ++i) {
        options[i] = va_arg(args, char*);
    }
    va_end(args);

    while (1) {
        if (title != NULL) {
            char color_code[32];
            ANSI_COLOR_FG(color_code, color);
            printf("%s%s%s\n", color_code, title, ANSI_RESET);
        }

        for (size_t i = 0; i < option_count; ++i) {
            print_option(options[i], selected == i, color);
            printf("\n");
        }

        int pressed = menu_getch();
        switch (pressed) {
            case KEY_ARROW_UP:
                selected = (selected + option_count - 1) % option_count; // Wrap around
                break;
            case KEY_ARROW_DOWN:
                selected = (selected + 1) % option_count; // Wrap around
                break;
            case KEY_ENTER:
                free(options);
                menu_enable_input_buffering();
                return selected;
            default:
                break;
        }

        ANSI_MOVE_CURSOR_UP(option_count + (title != NULL));
        ANSI_CLEAR_BELOW_CURSOR();
    }
}

#endif // MENU_IMPLEMENTATION

#endif // MENU_H

