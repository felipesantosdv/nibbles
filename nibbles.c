#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*** CONSTANTES ***/
#define WIDTH           40
#define HEIGHT          20
#define MAX_SIZE        100
#define DEFAULT_WAIT    100
#define WALL            '#'
#define SNAKE           '@'
#define FOOD            '$'
#define SNAKE_COLOR     1
#define FOOD_COLOR      2
#define WALL_COLOR      3


/*** STRUCTS ***/
typedef struct {
    int x[MAX_SIZE];
    int y[MAX_SIZE];
    int length;
    int direction_x;
    int direction_y;
} Snake;

typedef struct {
    int x;
    int y;
} Food;

/*** VARIÁVEIS GLOBAIS ***/ 
Snake snake;
Food food;
bool game_over = false;
char screen_buffer[HEIGHT][WIDTH];
int score = 0;

/*** MÉTODOS ***/ 
void generate_food();
void init();
void draw();
void update();
void process_input();

/*** ENTRY POINT ***/ 
int main() {
    init();
    
    while (!game_over) {
        draw();
        process_input();
        update();
        napms(DEFAULT_WAIT);
    }

    endwin();

    return 0;
}

/******************************************/
void generate_food() {
    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;
}

void init() {
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    start_color();

    init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);
    init_pair(FOOD_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair(WALL_COLOR, COLOR_BLUE, COLOR_BLACK);

    snake.length = 1;
    snake.x[0] = WIDTH / 2;
    snake.y[0] = HEIGHT / 2;
    snake.direction_x = 1;
    snake.direction_y = 0;

    generate_food();

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == (HEIGHT - 1) || j == 0 || j == (WIDTH - 1))
                screen_buffer[i][j] = WALL;
            else
                screen_buffer[i][j] = ' ';
        }
    }
}

void draw() {
    //clear();

    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            screen_buffer[i][j] = ' ';
        }
    }

    screen_buffer[food.y][food.x] = FOOD;

    for (int i = 0; i < snake.length; i++) {
        int x = snake.x[i];
        int y = snake.y[i];
        screen_buffer[y][x] = SNAKE;
    }

     for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (screen_buffer[i][j] == WALL) {
                attron(COLOR_PAIR(WALL_COLOR));
                mvaddch(i, j, screen_buffer[i][j]);
                attroff(COLOR_PAIR(WALL_COLOR));
            } else if (screen_buffer[i][j] == SNAKE) {
                attron(COLOR_PAIR(SNAKE_COLOR));
                mvaddch(i, j, screen_buffer[i][j]);
                attroff(COLOR_PAIR(SNAKE_COLOR));
            } else if (screen_buffer[i][j] == FOOD) {
                attron(COLOR_PAIR(FOOD_COLOR));
                mvaddch(i, j, screen_buffer[i][j]);
                attroff(COLOR_PAIR(FOOD_COLOR));
            } else {
                mvaddch(i, j, screen_buffer[i][j]);
            }
            
        }
    }

    mvprintw(HEIGHT, 0, "SCORE: %d", score);
    refresh();
}

void update() {
    int new_x = snake.x[0] + snake.direction_x;
    int new_y = snake.y[0] + snake.direction_y;

    if (new_x <= 0 || new_x >= WIDTH - 1 || new_y <= 0 || new_y >= HEIGHT - 1) {
        game_over = true;
    }

    if (new_x == food.x && new_y == food.y) {
        snake.length++;
        score += 10;
        generate_food();
    }

    for (int i = snake.length - 1; i > 0; i--) {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    snake.x[0] = new_x;
    snake.y[0] = new_y;
}

void process_input() {
    int key = getch();

    switch (key) {
        case 'w':
        case KEY_UP:
            if (snake.direction_y == 0) {
                snake.direction_x = 0;
                snake.direction_y = -1;
            }
        break;

        case 's':
        case KEY_DOWN:
            if (snake.direction_y == 0) {
                snake.direction_x = 0;
                snake.direction_y = 1;
            }
        break;

        case 'a':
        case KEY_LEFT:
            if (snake.direction_x == 0) {
                snake.direction_x = -1;
                snake.direction_y = 0;
            }
        break;

        case 'd':
        case KEY_RIGHT:
            if (snake.direction_x == 0) {
                snake.direction_x = 1;
                snake.direction_y = 0;
            }
        break;

        case 'q':
            game_over = true;
        break;
    }
}
