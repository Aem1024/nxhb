// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <switch.h>
// Macros for console window size
#define COLUMNS 20
#define ROWS 20
// Main program entrypoint
int main(int argc, char* argv[])
{
    //initializes console
    consoleInit(NULL);
    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);
    //TODO: Refactor to struct
    // X at last frame and Y at last frame
    int movsafe = 1;
    int x = 1;
    int y = 1;
    int xlf, ylf;
    int i, j;
    int enemyAlive = 1;

    // Fixes an issue that I don't want to bother fixing rn.
    int markx = 0;
    int marky = 0;
    int health = 3;
    int score = 0;
    // Map array
    char map[ROWS][COLUMNS] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //DO NOT WRITE TO THIS LINE IT IS OVERWRITTEN BY HEALTH/SCORE
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,2,4,7,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,3,4,6,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
    // Main Loop
    while (appletMainLoop())
    {
        xlf = x;
        ylf = y;
        // Checks if moving is safe
        movsafe = 1;

         if (map[y][x] >= 1) {
            movsafe = 0;
            x = xlf;
            y = ylf;
        }
        // Iterating over the array
        for (j = 0; j < COLUMNS; j++) {
            for (i = 0; i < ROWS; i++) {
                switch (map[i][j]) {
                    case 0 :
                        // Change colour to green
                        printf("\x1b[32m");
                        printf("\x1b[%d;%dH%c", i, j, 219);
                        printf("\x1b[0m");
                        break;
                    case 1 :
                        // Change colour to blue
                        printf("\x1b[37;44m");   // Set background to blue, foreground to white
                        printf("\x1b[%d;%dH^", i, j); // Print the white tilde with blue background
                        printf("\x1b[0m");           // Revert back to default colors (black and white)
                        break;
                    case 2 :
                        // bottom-right
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 201);
                        printf("\x1b[0m");
                        break;
                    case 3 :
                        // top-right
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 200);
                        printf("\x1b[0m");
                        break;
                    case 4 :
                        // left-right
                            printf("\x1b[37;42m");
                            printf("\x1b[%d;%dH%c", i, j, 205);
                            printf("\x1b[0m");
                            break;
                    case 5 :
                        // bottom-top
                            printf("\x1b[37;42m");
                            printf("\x1b[%d;%dH%c", i, j, 186);
                            printf("\x1b[0m");
                            break;
                    case 6 :
                        // left-top
                            printf("\x1b[37;42m");
                            printf("\x1b[%d;%dH%c", i, j, 188);
                            printf("\x1b[0m");
                            break;
                    case 7 : 
                            // bottom - left
                            printf("\x1b[37;42m");
                            printf("\x1b[%d;%dH%c", i, j, 187);
                            printf("\x1b[0m");
                            break;
                    
                }
            }
                
        }
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Left && x > 1 && movsafe == 1) {
            if (map[y][x - 1] == 0) {
                x = x - 1;
            }
        }
        if (kDown & HidNpadButton_Right && x < COLUMNS && movsafe == 1) {
            if (map[y][x + 1] == 0) {
                x = x + 1;
            }
        }
        if (kDown & HidNpadButton_Up && y > 2 && movsafe == 1) {
            if (map[y - 1][x] == 0) {
                y = y - 1;
            }
        }
        if (kDown & HidNpadButton_Down && y < ROWS && movsafe == 1) {
            if (map[y + 1][x] == 0) {
                y = y + 1;
            }

        }
        if (kDown & HidNpadButton_A) {
            score++;
        }
        if (kDown & HidNpadButton_B) {
            health = health - 1;
        }
        printf("\x1b[37;42m");
        printf("\x1b[%d;%dH%c", y, x, 2);
        printf("\x1b[0m");
        if (enemyAlive == 1) {
            printf("\x1b[31;42m");
            printf("\x1b[%d;%dH%c", 10, 10, 2);
            printf("\x1b[0m");
        }
        if (marky != y || markx != x) {
            printf("\x1b[%d;%dH", marky, markx); 
        }
        if (kDown & HidNpadButton_Plus) {
            break;
        }
        printf("HEALTH %c %d ", 3, health);
        printf("SCORE  %d\n", score);
        while (health <= 0) {
            consoleClear();
            printf("Game Over!");
            consoleUpdate(NULL);
        }
        //updates and clears the console 
        consoleUpdate(NULL);
        consoleClear();
    }
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}