// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <switch.h>
// Macros for console window size
#define COLUMNS 21
#define ROWS 21
// Enemy Structure
struct enemy {
        int enemyX;
        int enemyY;
        int enemyAlive;
};

// DisplayPixel function
void pixlOut(int x, int y, int foreground, int background, char tile) {


	printf("\x1b[%i;%im",background,foreground);
	// Sets colour to specified colours
	printf("\x1b[%i;%iH", x, y);
	printf("%c", tile);
	// Sets back to white and black background
	printf("\x1b[0m"); 
}	


// Main program entrypoint
int main()
{
    //initializes console
    consoleInit(NULL);
    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    consoleSetWindow(NULL, 0, 0, COLUMNS, ROWS);
    struct enemy en;
    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);
    padUpdate(&pad);
    int movsafe = 1;
    int x = 1;
    int y = 2;
    int xlf, ylf;
    // Health and Score (Obviously)
    int health = 3;
    int score = 0;
    // Map array
    char map[ROWS][COLUMNS] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //DO NOT WRITE TO THIS LINE IT IS OVERWRITTEN BY HEALTH/SCORE
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,2,4,4,4,7,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,1,5,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,1,5,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,3,4,4,4,6,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
    // Main Loop
    while (appletMainLoop())
    {
        xlf = x;
        ylf = y;
        en.enemyAlive = 1;
        en.enemyX = 10;
        en.enemyY = 10;
        // Checks if moving is safe
        movsafe = 1;

         if (map[x][y] >= 1) {
            movsafe = 0;
            x = xlf;
            y = ylf;
        }
        // Iterating over the array
        for (int j = 0; j < COLUMNS; j++) {
            for (int i = 0; i < ROWS; i++) {
                switch (map[i][j]) {
                    case 0 :
			// grass
			pixlOut(i, j, 32, 0, 219);
                        break;
                    case 1 :
			// water

			pixlOut(i, j, 37, 44, 94);
                        break;
                    case 2 :
			// Top-Left
			pixlOut(i, j, 37, 42, 201);
                        break;
                    case 3 :
			pixlOut(i, j, 37, 42, 200);
                        // top-right
                        break;
                    case 4 :
                        // left-right
			pixlOut(i, j, 37, 42, 205);
                        break;
                    case 5 :
                        // bottom-top
			pixlOut(i, j, 37, 42, 186);
                        break;
                    case 6 :
                        // left-top
			pixlOut(i, j, 37, 42, 188);
                        break;
                    case 7 : 
                        // bottom - left
			pixlOut(i, j, 37, 42, 187);
                        break;
                    
                }
            }
                
        }
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Left && x > 1 && movsafe == 1) {
            if (map[y][x - 1] == 0) {
                x -= 1;
            }
        }
        if (kDown & HidNpadButton_Right && x < COLUMNS && movsafe == 1) {
            if (map[y][x + 1] == 0) {
                x += 1;
            }
        }
        if (kDown & HidNpadButton_Up && y > 2 && movsafe == 1) {
            if (map[y - 1][x] == 0) {
                y -= 1;
            }
        }
        if (kDown & HidNpadButton_Down && y < ROWS && movsafe == 1) {
            if (map[y + 1][x] == 0) {
                y += 1;
            }

        }
        if (kDown & HidNpadButton_A) {
            score++;
        }
        if (kDown & HidNpadButton_B) {
            health = health - 1;
        }

	pixlOut(y, x, 37, 42, 2);

        if (en.enemyAlive == 1) {
	    pixlOut(en.enemyY, en.enemyX, 31, 42, 2);
        }
        if (kDown & HidNpadButton_Plus) {
            break;
        }
        printf("\x1b[0;0HHEALTH %c %d ", 3, health);
        printf("SCORE  %d \n", score);
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
