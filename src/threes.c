#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "input.h"
#include "array.h"

int possible_values[] = { 1, 2, 3, 6 };
int sizev = 4;
int TILE_WIDTH = 10;
int BLANK_WIDTH = 2;
int TOTAL_WIDTH = (10 * 4) + (2 * 5);

int** board;

int background[] = { 69, 207, 255 };
int foreground[] = { 255, 255, 0 };
int shadow[] = { 63, 201, 11 };

struct winsize window;

int getColor(int colors[], int number) {
    switch(number) {
        case 0:
            return 153;
        case 1:
            return colors[0];
        case 2:
            return colors[1];
        default:
            return colors[2];
    }
}

int intLength(int number) {
    if(number == 0) return 1;
    return floor(log10(abs(number))) + 1;
}

void colorReset() {
    printf("\e[m");
}

void setColorScheme(int bg, int fg) {
    printf("\e[38;5;%d;48;5;%dm", fg, bg);
}

void blanks() {
    printf("%*s", TILE_WIDTH, "");
}

void printCenter(char* str) {
    printf("%*s%s", (int) ((window.ws_col - strlen(str))/2), " ", str);
}

void divider() {
    setColorScheme(getColor(background, 0), getColor(foreground, 0));
    printf("%*s", BLANK_WIDTH, "");
    colorReset();
}

void centerBlank() {
    printf("%*s", (window.ws_col - TOTAL_WIDTH)/2, "");
    divider();
}

void printTileCenter(int number) {
    int t;
    t = TILE_WIDTH - intLength(number);
    printf("%*s%d%*s", t - t/2, "", number, t/2, "");
}

void tileLine(int i) {
    int j;
    for(j = 0; j < 4; j++) {
        if(j == 0) {
            centerBlank();
        }
        setColorScheme(getColor(background, board[i][j]), getColor(foreground, board[i][j]));
        blanks();
        colorReset();
        divider();
    }
    printf("\n");
}

void shadowLine(int i) {
    int j;
    for(j = 0; j < 4; j++) {
        if(j == 0) {
            centerBlank();
        }
        setColorScheme(getColor(shadow, board[i][j]), getColor(foreground, board[i][j]));
        blanks();
        colorReset();
        divider();
    }
    printf("\n");
}

void dummyLine() {
    int j;
    for(j = 0; j < 4; j++) {
        if(j == 0) {
            centerBlank();
        }
        setColorScheme(getColor(background, 0), getColor(foreground, 0));
        blanks();
        colorReset();
        divider();
    }
    printf("\n");
}

void clearScreen() {
    printf("\e[1;1H\e[2J");
}

int drawBoard() {
    int i, j, k, t;
    char c;
    clearScreen();

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    printf("\n");
    printCenter("^____^");
    printf("\n\n");

    for(i = 0; i < 4; i++) {
        if(i == 0) {
            dummyLine();
        }
        tileLine(i);
        tileLine(i);
        tileLine(i);
        for(j = 0; j < 4; j++) {
            if(j == 0) {
                centerBlank();
                // divider();
            }
            setColorScheme(getColor(background, board[i][j]), getColor(foreground, board[i][j]));
            printTileCenter(board[i][j]);
            colorReset();
            divider();
        }
        printf("\n");
        tileLine(i);
        tileLine(i);
        shadowLine(i);
        dummyLine();
    }

    printf("\n");
    printCenter("        a(←), w(↑), d(→), s(↓) or q(quit)");
    printf("\n");
    return 1;
}

int** allocateBoard() {
    int** board;
    int i;

    board = (int **) calloc(sizeof(int *), 4);
    for(i = 0; i < 4; i++) {
        board[i] = (int *) calloc(sizeof(int), 4);
    }

    return board;
}

int bindNumber(int i, int j) {
    return (i * 10) + j;
}

Array* getAvailableTiles() {
    int i, j;
    Array* cells = initArray(4);

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(board[i][j] == 0) {
                insertArray(cells, bindNumber(i, j));
            }
        }
    }

    return cells;
}

int addRandomTile() {
    int r;
    Array* cells;

    cells = getAvailableTiles();

    r = cells->used;

    if(r == 0) return 0;

    r = rand() % r;

    r = cells->array[r];

    if(board[r / 10][r % 10] != 0) {
        printf("Error: %d\n", r);
        return 0;
    }

    board[r / 10][r % 10] = possible_values[rand() % sizev];

    freeArray(cells);
    free(cells);

    return 1;
}

void swap(int* current, int* next) {
    if(*current == 0) {
        *current = *next;
        *next = 0;
    } else {
        if((*current + *next == 3) || ((*current == *next) && (*current != 1 && *current != 2))) {
            *current = *current + *next;
            *next = 0;
        }
    }
}

void moveUp() {
    int i, j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 4; j++) {
            swap(&(board[i][j]), &(board[i + 1][j]));
        }
    }
}

void moveDown() {
    int i, j;
    for(i = 3; i > 0; i--) {
        for(j = 0; j < 4; j++) {
            swap(&(board[i][j]), &(board[i - 1][j]));
        }
    }
}

void moveLeft() {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 3; j++) {
            swap(&(board[i][j]), &(board[i][j + 1]));
        }
    }
}

void moveRight() {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 3; j > 0; j--) {
            swap(&(board[i][j]), &(board[i][j - 1]));
        }
    }
}

int main() {
    Array* cells;
    char c;
    char scorestr[17];
    unsigned int i, j, score = 0, t;

    srand(time(NULL));

    board = allocateBoard();

    addRandomTile();
    addRandomTile();
    addRandomTile();
    addRandomTile();
    addRandomTile();
    addRandomTile();
    addRandomTile();
    addRandomTile();

    while((addRandomTile() != 0 && drawBoard()) && (c = getch()) != 'q') {
        switch(c) {
            case 'w':
                moveUp();
                break;
            case 's':
                moveDown();
                break;
            case 'a':
                moveLeft();
                break;
            case 'd':
                moveRight();
                break;
        }
    }

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            score += pow(3, log2(board[i][j]/3) + 1);
        }
    }

    sprintf(scorestr, "Score: %u", score);

    printf("\033[A\033[2K");
    printCenter(scorestr);
    printf("\n\n");

    printCenter("Threes C (https://github.com/harshjv/threes-c)\n");
    printCenter("Released under MIT license by Harsh Vakharia (@harshjv)\n");

    return 0;
}