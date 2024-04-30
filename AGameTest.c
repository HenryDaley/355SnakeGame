#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define INITIAL_SNAKE_LENGTH 5
#define MAX_SNAKE_LENGTH 100

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

    srand(time(0)); // Seed random number generator -> Henry

    // Get terminal window dimensions
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // Both; Setting up game variables and positioning
    Position snake[MAX_SNAKE_LENGTH];
    int currentLength = INITIAL_SNAKE_LENGTH;
    int directionX = 1, directionY = 0; // Initially moving right
    int gameOver = 0;
    int score = 0;
    int head = 0; // Index of the snake's head in the array

    // Henry; Initialize snake body positions
    for (int i = 0; i < currentLength; i++) {
        snake[i].x = maxX / 2 - i;
        snake[i].y = maxY / 2;
    }

    // Henry; Initialize trophy
    Position trophy = {rand() % (maxX - 2) + 1, rand() % (maxY - 2) + 1};
    int trophyValue = rand() % 9 + 1; // Values from 1 to 9

    int baseDelay = 100; // Base delay in milliseconds
    int speedIncreasePerPoint = 10; // Speed increase per point

    // Andrew; main loop, drawing the border
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

        // Henry; Printing the trophy
        mvprintw(trophy.y, trophy.x, "%d", trophyValue);

        // Both; Setting up the snake controls
        timeout(50); // Wait 50ms for user input
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

        // Henry; Moves the snake head
        Position next = { snake[head].x + directionX, snake[head].y + directionY };
        head = (head + 1) % MAX_SNAKE_LENGTH;  // Move head to next segment
        snake[head] = next;  // New head position

        // Andrew; Checks for self-collision
        for (int i = 1; i < currentLength; i++) {
            if (next.x == snake[(head - i + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH].x &&
                next.y == snake[(head - i + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH].y) {
                gameOver = 1;
                break;
            }
        }

        // Draw the snake
        for (int i = 0; i < currentLength; i++) {
            mvprintw(snake[(head - i + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH].y, snake[(head - i + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH].x, "S");
        }

        // Check for trophy collection and update score
        if (next.x == trophy.x && next.y == trophy.y) {
            score += trophyValue;  // Increase score by trophy value
            // Grow the snake by the trophy value
            if (currentLength + trophyValue < MAX_SNAKE_LENGTH) {
                currentLength += trophyValue;
            } else {
                currentLength = MAX_SNAKE_LENGTH;
            }
            // Respawn the trophy at a new random location
            trophy.x = rand() % (maxX - 2) + 1;
            trophy.y = rand() % (maxY - 2) + 1;
            trophyValue = rand() % 9 + 1;
        }

        // Draw score
        mvprintw(0, 2, "Score: %d", score);

        // Collision detection with game border
        if (next.x == 0 || next.x == maxX - 1 || next.y == 0 || next.y == maxY - 1) {
            gameOver = 1; // Hit the border
        }

        // Calculate current delay based on score
        int currentDelay = baseDelay - (score * speedIncreasePerPoint);
        if (currentDelay < 50) currentDelay = 50; // Minimum delay to avoid being too fast

        // Refresh the screen
        refresh();

        // Delay to slow down frame rate adjusted by score
        napms(currentDelay); // Dynamic pause based on score
    }


    // Clean up curses
    endwin();
    return 0;
}
