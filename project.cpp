#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

int gameOver;
int x, y;
int fruitX, fruitY;
int score;

int tailX[100], tailY[100];
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

// Hide cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Move cursor
void setCursorPosition(int xPos, int yPos) {
    COORD coord = { xPos, yPos };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// ****** MENU FUNCTION ******
void ShowMenu() {
    int choice;
    while (1) {
        system("cls");
        printf("====== SNAKE GAME ======\n");
        printf("1. Start Game\n");
        printf("2. Instructions\n");
        printf("3. Exit\n");
        printf("========================\n");
        printf("Choose an option: ");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            return;
        case 2:
            system("cls");
            printf("=== Instructions ===\n");
            printf("Use keys:\n");
            printf("W - Up\nA - Left\nS - Down\nD - Right\n");
            printf("X - Quit the game\n\n");
            printf("Press any key to return...");
            _getch();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice! Press any key...");
            _getch();
        }
    }
}

void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
    hideCursor();
}

void Draw() {
    setCursorPosition(0, 0);

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#");

            if (i == y && j == x)
                printf("O");
            else if (i == fruitY && j == fruitX)
                printf("*");
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        print = 1;
                        break;
                    }
                }
                if (!print)
                    printf(" ");
            }

            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    printf("Score: %d\n", score);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': if (dir != RIGHT) dir = LEFT; break;
        case 'd': if (dir != LEFT) dir = RIGHT; break;
        case 'w': if (dir != DOWN) dir = UP; break;
        case 's': if (dir != UP) dir = DOWN; break;
        case 'x': gameOver = 1; break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    if (x >= WIDTH) x = 0;
    else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0;
    else if (y < 0) y = HEIGHT - 1;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    ShowMenu();  // Show menu before the game

    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(50);
    }

    printf("Game Over! Final Score: %d\n", score);
    return 0;
}
