
#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define SCREEN_ROWS 25
#define SCREEN_COLS 80

typedef enum text_color_e {
    BLACK = 0x0,
    BLUE = 0x1,
    GREEN = 0x2,
    CYAN = 0x3,
    RED = 0x4,
    MAGENTA = 0x5,
    BROWN = 0x6,
    WHITE = 0xf
} text_color;

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void    init_screen(int x, int y, text_color foreground, text_color background);

void    putchar(char c);
void    putstr(char *s);
void    color(text_color foreground, text_color background);
void    clear();

#endif