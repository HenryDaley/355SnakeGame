#include <curses.h>

#define SNAKE_LENGTH 5

typedef struct {
    int x;
    int y;
} Position;

int main() {
    initscr();  // Initialize curses
    noecho();   // Turns off echo of keyboard input
    keypad(stdscr, TRUE);   // Enables keypad
    curs_set(0); // Hide cursor

    // Get terminal window dimensions
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // Game variables
    Position snake[SNAKE_LENGTH];
    int directionX = 1, directionY = 0;
    int gameOver = 0;
    int score = 0;
    int head = 0; // Index of the snake's head in the array

    // Initialize snake body positions
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake[i].x = maxX / 2 - i;
        snake[i].y = maxY / 2;
    }

    // Game loop
    while(!gameOver) {
        clear(); // Clear the screen

        // Draw border
        for(int x = 0; x < maxX; x++) {
            mvprintw(0, x, "#");
            mvprintw(maxY - 1, x, "#");
        }
        for(int y = 0; y < maxY; y++) {
            mvprintw(y, 0, "#");
            mvprintw(y, maxX - 1, "#");
        }

        // Handle user input
        timeout(100); // Wait 100ms for user input
        int ch = getch();
        switch(ch) {
            case KEY_UP:    if(directionY == 0) { directionX = 0;  directionY = -1; } break;
            case KEY_DOWN:  if(directionY == 0) { directionX = 0;  directionY = 1; } break;
            case KEY_LEFT:  if(directionX == 0) { directionX = -1; directionY = 0; } break;
            case KEY_RIGHT: if(directionX == 0) { directionX = 1;  directionY = 0; } break;
            case 'X':  // Emergency exit
                gameOver = 1;
                break;
        }

        // Move snake head
        Position next = { snake[head].x + directionX, snake[head].y + directionY };
        head = (head + 1) % SNAKE_LENGTH;  // Move head to next segment
        snake[head] = next;  // New head position

        // Draw the snake
        for (int i = 0; i < SNAKE_LENGTH; i++) {
            mvprintw(snake[(head - i + SNAKE_LENGTH) % SNAKE_LENGTH].y, snake[(head - i + SNAKE_LENGTH) % SNAKE_LENGTH].x, "#");
        }

        // Draw score
        mvprintw(0, 2, "Score: %d", score);

        // Collision detection
        if (next.x == 0 || next.x == maxX - 1 || next.y == 0 || next.y == maxY - 1) {
            gameOver = 1; // Hit the border
        }

        // Refresh the screen
        refresh();

         napms(200); // Pause for 200 milliseconds
    }

    // Clean up curses
    endwin();
    return 0;
}