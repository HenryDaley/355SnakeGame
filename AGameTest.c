#include <curses.h>

int main() {
    initscr();  // Initialize curses
    noecho();   // Turns off echo of keyboard input
    keypad(stdscr, TRUE);   // Enables keypad

    // Get terminal window dimensions
    int maxY, maxX; // Variables for storing terminal sizes
    getmaxyx(stdscr, maxY, maxX);   // Receives the size of the window and stores in previous variables

    // Game variables
    int snakeX = maxX / 2 - 1, snakeY = maxY / 2;  // Initial snake position
    int directionX = 1, directionY = 0;
    int gameOver = 0;
    int score = 0;


// Game loop
while(!gameOver) {
    // Clear the screen
    clear();

    // Draw border
    for(int x = 0; x < maxX; x++) {
        mvprintw(0, x, "#");          // Top border
        mvprintw(maxY - 1, x, "#");   // Bottom border
    }
    for(int y = 0; y < maxY; y++) {
        mvprintw(y, 0, "#");          // Left border
        mvprintw(y, maxX - 1, "#");   // Right border
    }

    // Clear the previous position of the snake
    mvprintw(snakeY, snakeX, " ");

    // Handle user input
    int ch = getch();
    switch(ch) {
        // Handle arrow keys to change snake direction
        case KEY_UP:
            directionX = 0;
            directionY = -1;
            break;
        case KEY_DOWN:
            directionX = 0;
            directionY = 1;
            break;
        case KEY_LEFT:
            directionX = -1;
            directionY = 0;
            break;
        case KEY_RIGHT:
            directionX = 1;
            directionY = 0;
            break;
        case 'X':   // Emergency exit for debugging etc.
            endwin();
            return 0;
    }

    // Update snake position
    snakeX += directionX;
    snakeY += directionY;

    // Draw the snake
    mvprintw(snakeY, snakeX, "S");

    // Draw score
    mvprintw(0, 2, "Score: %d", score);

    // Refresh the screen
    refresh();
}


    // Clean up curses
    endwin();

    return 0;
}
