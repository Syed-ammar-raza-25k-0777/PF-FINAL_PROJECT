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
    while (1) {
        system("cls");

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

        // Helper for centering
        #define CENTER_TEXT(msg) { \
            int pad = (consoleWidth - strlen(msg)) / 2; \
            if (pad < 0) pad = 0; \
            for (int i = 0; i < pad; i++) printf(" "); \
            printf("%s\n", msg); \
        }

        CENTER_TEXT("====== SNAKE GAME ======");
        CENTER_TEXT("1. Start Game");
        CENTER_TEXT("2. Instructions");
        CENTER_TEXT("3. Exit");
        CENTER_TEXT("========================");
        printf("\n");

        char prompt[] = "Choose an option: ";
        int padPrompt = (consoleWidth - strlen(prompt)) / 2;
        if (padPrompt < 0) padPrompt = 0;
        for (int i = 0; i < padPrompt; i++) printf(" ");
        printf("%s", prompt);

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                return;

            case 2:
                system("cls");
                CENTER_TEXT("=== Instructions ===");
                CENTER_TEXT("Use keys:");
                CENTER_TEXT("W - Up");
                CENTER_TEXT("A - Left");
                CENTER_TEXT("S - Down");
                CENTER_TEXT("D - Right");
                CENTER_TEXT("X - Quit the game");
                printf("\n");
                CENTER_TEXT("Press any key to return...");
                _getch();
                break;

            case 3:
                exit(0);

            default:
                CENTER_TEXT("Invalid choice! Press any key...");
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
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int boardWidth = WIDTH + 2;          // game width + borders
    int padding = (consoleWidth - boardWidth) / 2;
    if (padding < 0) padding = 0;        // safety

    setCursorPosition(0, 0);

    // Top border
    for (int p = 0; p < padding; p++) printf(" ");
    for (int i = 0; i < boardWidth; i++) printf("#");
    printf("\n");

    // Board rows
    for (int i = 0; i < HEIGHT; i++) {
        for (int p = 0; p < padding; p++) printf(" ");

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
                if (!print) printf(" ");
            }

            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }

    // Bottom border
    for (int p = 0; p < padding; p++) printf(" ");
    for (int i = 0; i < boardWidth; i++) printf("#");
    printf("\n");

    // Centered Score
    char scoreText[50];
    sprintf(scoreText, "Score: %d", score);
    int scorePadding = (consoleWidth - (int)strlen(scoreText)) / 2;
    if (scorePadding < 0) scorePadding = 0;

    for (int p = 0; p < scorePadding; p++) printf(" ");
    printf("%s\n", scoreText);
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
    getchar();
    getchar();

    return 0;
}
