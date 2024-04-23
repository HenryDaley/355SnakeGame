//Put what works here
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define INITIAL_SNAKE_LENGTH 5
#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x;
    int y;
} Position;

int main() {
    initscr();  // Initialize curses
    noecho();   // Turns off echo of keyboard input
    keypad(stdscr, TRUE);   // Enables keypad
    curs_set(0); // Hide cursor

    srand(time(0)); // Seed random number generator

    // Get terminal window dimensions
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // Game variables
    Position snake[MAX_SNAKE_LENGTH];
    int currentLength = INITIAL_SNAKE_LENGTH;
    int directionX = 1, directionY = 0; // Initially moving right
    int gameOver = 0;
    int score = 0;
    int head = 0; // Index of the snake's head in the array

    // Initialize snake body positions
    for (int i = 0; i < currentLength; i++) {
        snake[i].x = maxX / 2 - i;
        snake[i].y = maxY / 2;
    }

    // Initialize trophy
    Position trophy = {rand() % (maxX - 2) + 1, rand() % (maxY - 2) + 1};
    int trophyValue = rand() % 9 + 1; // Values from 1 to 9

    int baseDelay = 400; // Base delay in milliseconds
    int speedIncreasePerPoint = 10; // Speed increase per point

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

        // Draw the trophy
        mvprintw(trophy.y, trophy.x, "%d", trophyValue);

        // Handle user input
        timeout(100); // Wait 100ms for user input
        int ch = getch();
        switch(ch) {
            case KEY_UP:    
                if (directionY == 1) gameOver = 1; // Moving down, up is opposite
                else { directionX = 0; directionY = -1; }
                break;
            case KEY_DOWN:  
                if (directionY == -1) gameOver = 1; // Moving up, down is opposite
                else { directionX = 0; directionY = 1; }
                break;
            case KEY_LEFT:  
                if (directionX == 1) gameOver = 1; // Moving right, left is opposite
                else { directionX = -1; directionY = 0; }
                break;
            case KEY_RIGHT: 
                if (directionX == -1) gameOver = 1; // Moving left, right is opposite
                else { directionX = 1; directionY = 0; }
                break;
            case 'X':  // Emergency exit
                gameOver = 1;
                break;
        }

        // Move snake head
        Position next = { snake[head].x + directionX, snake[head].y + directionY };
        head = (head + 1) % MAX_SNAKE_LENGTH;  // Move head to next segment
        snake[head] = next;  // New head position

        // Draw the snake
        for (int i = 0; i < currentLength; i++) {
            mvprintw(snake[(head - i + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH].y, snake[(head - i + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH].x, "S");
        }

        // Check for trophy collection
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

        // Collision detection
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
