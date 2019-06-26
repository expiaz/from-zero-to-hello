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

void    screen_init(int x, int y, text_color foreground, text_color background);

void    putchar(char c);
void    putstr(char *s);
void    color(text_color foreground, text_color background);
void    clear();

#define DECIMAL "0123456789"
#define HEXA    "0123456789ABCDEF"

int     is_printable(char c);
void    putnbr(int nb);
void    puthexa(int nb);
void    putnbr_base(int nb, char *base);

#endif