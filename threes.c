#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "./includes/input.h"
#include "./includes/array.h"

int possible_values[] = { 1, 2, 3, 6 };
int sizev = 4;

int** board;

int background[] = { 69, 207, 255 };
int foreground[] = { 255, 255, 0 };
int shadow[] = { 63, 201, 11 };

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

void colorReset() {
    printf("\e[m");
}

void setColorScheme(int bg, int fg) {
    printf("\e[38;5;%d;48;5;%dm", fg, bg);
}

void divider() {
    setColorScheme(getColor(background, 0), getColor(foreground, 0));
    printf("  ");
    colorReset();
}

void blanks() {
    printf("         ");
}

void tileLine(int i) {
    int j;
    for(j = 0; j < 4; j++) {
        if(j == 0) {
            divider();
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
            divider();
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
            divider();
        }
        setColorScheme(getColor(background, 0), getColor(foreground, 0));
        blanks();
        colorReset();
        divider();
    }
    printf("\n");
}

int intLength(int number) {
    int i = 1;
    while(number / 10 != 0) {
        number /= 10;
        i++;
    }
    return i;
}

int drawBoard() {
    int i, j, k, t;
    char c;
    char color[40], reset[] = "\e[m";
    printf("\e[H");

    printf("                    ^____^                    \n\n");

    for(i = 0; i < 4; i++) {
        if(i == 0) {
            dummyLine();
        }
        tileLine(i);
        tileLine(i);
        tileLine(i);
        for(j = 0; j < 4; j++) {
            if(j == 0) {
                divider();
            }
            setColorScheme(getColor(background, board[i][j]), getColor(foreground, board[i][j]));
            t = 9 - intLength(board[i][j]);
            printf("%*s%d%*s", t - (t/2), "", board[i][j], t/2, "");
            colorReset();
            divider();
        }
        printf("\n");
        tileLine(i);
        tileLine(i);
        shadowLine(i);
        dummyLine();
    }

    printf("    a(←), w(↑), d(→), s(↓) or q(quit)\n");
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

    free(cells);

    return 1;
}

int printBoard() {
    int i, j;

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            printf("%4d\t", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");

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
    int r, i, j, score = 0, t;

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

    while(((r = addRandomTile()) != 0 && drawBoard()) && (c = getch()) != 'q') {
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

    t = (41-7) - intLength(score);
    printf("%*s%s%d%*s", t - (t/2), "", "Score: ", score, t/2, "");

    printf("\n");

    return 0;
}