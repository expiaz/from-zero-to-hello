#include "screen.h"
#include "../kernel/common.h"

int     cursor_x;
int     cursor_y;
u16     *video_memory;
u16      attribute;

void    move_cursor();
void    scroll();

void    init_screen(int x, int y, text_color foreground, text_color background) {
    cursor_x = 0;
    cursor_y = 0;
    video_memory = (u16 *) VIDEO_ADDRESS;
    color(foreground, background);
}

void    color(text_color foreground, text_color background) {
    /*
    u8      attribute_byte;
    
    attribute_byte = (background << 4) | (foreground & 0x0f);
    attribute = attribute_byte << 8;
    */
   // upper 4 bits are the background color code, lower 4 are the foreground one
   // all in 8 bits
    attribute = ((background << 4) | (foreground & 0x0f)) << 8;
}

// 0x3954
// 0011 1001 0101 0100

// 0x3872


void    putchar(char c) {
    u16     *location;
    
    // backspace
    if (c == 0x08 && cursor_x > 0) {
        cursor_x--;
        putchar(' ');
        cursor_x--;
    }
    // backspace on carriage return
    else if (c == 0x08 && cursor_y > 0) {
        cursor_y--;
        /*cursor_x = 80;
        while (
            --cursor_x &&
            ! *(location = video_memory + (cursor_y * SCREEN_COLS + cursor_x))
        );
        */
    }
    // tab
    else if (c == 0x09) {
        cursor_x += 8 - cursor_x % 8;
        //cursor_x = (cursor_x + 8) & ~(8 - 1);
    }
    // carriage return
    else if (c == '\r') {
        cursor_x = 0;
    }
    // newline
    else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    }
    // printable characters
    else if (c >= ' ') {
        // compute the position from the x and y indexs
        location = video_memory + (cursor_y * SCREEN_COLS + cursor_x);
        // 8 bits for the character code (0-7), 8 for the attribute (8-15)
        // total of 16bits per char
        *location = c | attribute;
        cursor_x++;
    }

    // go to next line if end of line reached
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    // handle scroll if y is out of screen
    scroll();
    // move the hardware cursor
    move_cursor();
}

void    putstr(char *s) {
    int     i;

    i = 0;
    while (s[i])
        putchar(s[i++]);
}

int is_printable(char c) {
    return (c > 31 && c < 127);
}

void putnbr(int nb) {
    putnbr_base(nb, DECIMAL);
}

void puthexa(int nb) {
    putnbr_base(nb, HEXA);
}

void __putnbr_base(int nb, char *base, int size)
{
    int     next;
    int     rest;
    char    c;

    next = nb / 10;
    rest = nb % 10;
    // bypass overflows
    if (rest < 0)
        rest = -rest;
    c = base[rest];
    if (next != 0)
        __putnbr_base(next, base, size);
    putchar(c);
}

void putnbr_base(int nb, char *base)
{
    int     size;

    size = 0;
    while (base[size])
    {
        if (!is_printable(base[size]))
            return;
        size++;
    }
    if (size) {
        if (nb < 0)
            putchar('-');
        __putnbr_base(nb, base, size);
    }
}

void    clear() {
    //u16     blank;
    int     i;

    // prepare the attribute for color
    color(WHITE, BLACK);
    //blank = ((BLACK << 4) | (WHITE & 0x0f)) << 8;
    // write blank chars everywhere
    for (i = 0; i < SCREEN_ROWS * SCREEN_COLS; i++)
        video_memory[i] = ' ' | attribute;

    cursor_y = 0;
    cursor_x = 0;
    move_cursor();
}

void    scroll() {
    //u16     blank;
    int     i;

    // scroll only if end of screen
    if (cursor_y < SCREEN_ROWS)
        return;

    // move on row higher every char on the screen, the first row is lost
    for (i = 0; i < (SCREEN_ROWS - 1) * SCREEN_COLS; i++)
        video_memory[i] = video_memory[i + SCREEN_COLS];
    
    // prepare the attribute for color
    color(WHITE, BLACK);
    //blank = ((BLACK << 4) | (WHITE & 0x0f)) << 8;
    // empty the last line
    for (i = (SCREEN_ROWS - 1) * SCREEN_COLS; i < SCREEN_ROWS * SCREEN_COLS; i++)
        video_memory[i] = ' ' | attribute;

    // update the cursor y index to the line before
    cursor_y--;
}

void    get_cursor() {
    // using hardware VGA control registers to get the position
    // of the cursor viewed by the chip
    // it's exprimed in number of chars and not bytes as we do

    // registers 14 and 15 represents high and low bytes
    // which combined gives the offset
    u16     location;

    // select high part
    outb(REG_SCREEN_CTRL, 14);
    // read high part
    location = inb(REG_SCREEN_DATA) << 8;
    // select low part
    outb(REG_SCREEN_CTRL, 15);
    // read low part
    location += inb(REG_SCREEN_DATA);

    cursor_y = location / SCREEN_COLS;
    cursor_x = location % SCREEN_COLS;
}

void    move_cursor() {
    u16 location = cursor_y * SCREEN_COLS + cursor_x;
    // set bytes to registers
    outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, location >> 8);
    outb(REG_SCREEN_CTRL, 15);
    outb(REG_SCREEN_DATA, location);
}