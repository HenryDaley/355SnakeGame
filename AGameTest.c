#include <curses.h>

#define SNAKE_LENGTH 5

// Henry; declaring the structure and position
typedef struct {
    int x, y;
} 

Position;

// Andrew; setting up curses, getting info for border, multiple variables for future use
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

    // Henry; initial snake creation
    int head = SNAKE_LENGTH - 1; // Index of the snake's head in the array
    int tail = 0; // Index of the snake's tail in the array

    // Initialize snake body positions
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake[i].x = maxX / 2 - i;
        snake[i].y = maxY / 2;
    }

    // Andrew; printing the border
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

        // Both; Setting up the snake controls
        timeout(50); // Wait 100ms for user input
        int ch = getch();
        switch(ch) {
            // Arrow keys
            case KEY_UP:
            case 'W':
            case 'w':
                if(directionY == 0) { directionX = 0;  directionY = -1; } 
                break;
            case KEY_DOWN:
            case 'S':
            case 's':
                if(directionY == 0) { directionX = 0;  directionY = 1; } 
                break;
            case KEY_LEFT:
            case 'A':
            case 'a':
                if(directionX == 0) { directionX = -1; directionY = 0; } 
                break;
            case KEY_RIGHT:
            case 'D':
            case 'd':
                if(directionX == 0) { directionX = 1;  directionY = 0; } 
                break;

            case 'X':  // Emergency exit
                gameOver = 1;
                break;
        }

        // Henry; updating/drawing the snake position
        head = (head + 1) % SNAKE_LENGTH;
        snake[head].x = snake[(head - 1 + SNAKE_LENGTH) % SNAKE_LENGTH].x + directionX;
        snake[head].y = snake[(head - 1 + SNAKE_LENGTH) % SNAKE_LENGTH].y + directionY;

        // Draw the snake
        for (int i = 0; i < SNAKE_LENGTH; i++) {
            mvprintw(snake[i].y, snake[i].x, "S");
        }

        // Draw score
        mvprintw(0, 2, "Score: %d", score);

        // Collision detection
        if (snake[head].x == 0 || snake[head].x == maxX - 1 || 
            snake[head].y == 0 || snake[head].y == maxY - 1) {
            gameOver = 1; // Hit the border
        }

        // Refresh the screen
        refresh();

        napms(100); // Pause for 200 milliseconds
    }

    // Clean up curses
    endwin();
    return 0;
}
