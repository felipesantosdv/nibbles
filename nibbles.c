#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*** CONSTANTES ***/
#define WIDTH    40
#define HEIGHT   20
#define MAX_SIZE 100

typedef struct {
    int x[MAX_SIZE];
    int y[MAX_SIZE];
    int length;
    int direction_x;
    int direction_y;
} Snake;

/*** VARIÁVEIS GLOBAIS ***/ 
Snake snake;
bool game_over = false;
char screen_buffer[HEIGHT][WIDTH];

/*** MÉTODOS ***/ 
void init();
void draw();
void update();

/*** ENTRY POINT ***/ 
int main() {
    init();
    
    while (!game_over) {
        draw();
        update();
        napms(100);
    }

    endwin();

    return 0;
}

/******************************************/

void init() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);

    snake.length = 1;
    snake.x[0] = WIDTH / 2;
    snake.y[0] = HEIGHT / 2;
    snake.direction_x = 1;
    snake.direction_y = 1;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == (HEIGHT - 1) || j == 0 || j == (WIDTH - 1))
                screen_buffer[i][j] = '#';
            else
                screen_buffer[i][j] = ' ';
        }
    }
}

void draw() {
    clear();

    for (int i = 0; i < snake.length; i++) {
        int x = snake.x[i];
        int y = snake.y[i];
        screen_buffer[y][x] = '@';
    }

     for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {   
            mvaddch(i, j, screen_buffer[i][j]);
        }
    }
    refresh();
}

void update() {
    int new_x = snake.x[0] + snake.direction_x;
    int new_y = snake.y[0] + snake.direction_y;

    snake.x[0] = new_x;
    snake.y[0] = new_y;
}