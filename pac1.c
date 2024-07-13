#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>

#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'
#define POWERUP 'P'
#pragma comment(lib, "Winmm.lib")


int res = 0;
int score = 0;
int pacman_x, pacman_y;
char board[HEIGHT][WIDTH];
int food = 0;
int curr = 0;
int chances = 3; 
int powerupCount = 1;


void gotoxy (int x, int y) {
	COORD c;
	c.X=x;
	c.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void load_screen() {
	system("cls");
	gotoxy(50, 7);
	printf("Loading...");
	gotoxy(50, 8);
	int i,j;
	for ( i=1; i<=20;i++) {
		for ( j=0; j<=10000000; j++);
		printf("%c", 219);
	}
	system("cls");
}
/*void playBackgroundMusic() {
	if (PlaySound("Vexento - Pixel Party.wav", NULL, SND_ASYNC | SND_LOOP) == 0) {
		printf("Error playing background music!\n");
	}

}*/
void generateMaze(int startX, int startY) {
    if (startX < 1 || startY < 1 || startX >= WIDTH - 1 || startY >= HEIGHT - 1) {
        return;
    }

    if (board[startY][startX] == EMPTY) {
        board[startY][startX] = WALL;

        int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
int i;
        for ( i = 0; i < 4; ++i) {
            int newX = startX + directions[i][0];
            int newY = startY + directions[i][1];

            generateMaze(newX, newY);
        }
    }
}
void restartGame() {
    // Reset all relevant variables and restart the game
    res = 0;
    score = 0;
    food = 0;
    curr = 0;
    chances = 3;
    powerupCount =1;
}

void initialize(int level) {
   restartGame();
    int i, j;
     // Initialize the level

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i == 0 || j == WIDTH - 1 || j == 0 || i == HEIGHT - 1) {
                board[i][j] = WALL;
            } else
                board[i][j] = EMPTY;
        }
    }

    generateMaze(1, 1);
       
int powerupPlaced = 0;
while (powerupPlaced < powerupCount) {
    int i = (rand() % (HEIGHT - 1)) + 1;
    int j = (rand() % (WIDTH - 1)) + 1;

    if (board[i][j] == EMPTY) {
        board[i][j] = POWERUP;
        powerupPlaced++;
        printf("Power-up placed at (%d, %d)\n", i, j);  
    }
}



    int numDemons = 10 + level * 5; // Increase demons with each level
    int numFood = 5 + level * 2;
       int count = numDemons;

    while (count != 0) {
        int i = (rand() % (HEIGHT + 1));
        int j = (rand() % (WIDTH + 1));

        if (board[i][j] != WALL && board[i][j] != PACMAN) {
            board[i][j] = DEMON;
            count--;
        }
    }

    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;

    // Points Placed

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i % 2 == 0 && j % 2 == 0 && board[i][j] != WALL && board[i][j] != DEMON && board[i][j] != PACMAN) {
                board[i][j] = FOOD;
                food++;
            }
        }
    }
}

void moveDemons() {
    int i, j;
    // Move demons randomly
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (board[i][j] == DEMON) {
                // Clear the current position
                board[i][j] = EMPTY;

                // Move demon randomly 
                int direction = rand() % 4; // 0: up, 1: down, 2: left, 3: right

                switch (direction) {
                    case 0:
                        if (i > 0 && board[i - 1][j] != WALL && board[i - 1][j] != PACMAN) {
                            if (board[i - 1][j] == FOOD) {
                                // Do not overwrite food, do nothing
                            } else {
                                i--;
                            }
                        }
                        break;
                    case 1:
                        if (i < HEIGHT - 1 && board[i + 1][j] != WALL && board[i + 1][j] != PACMAN) {
                            if (board[i + 1][j] == FOOD) {
                                // Do not overwrite food, do nothing
                            } else {
                                i++;
                            }
                        }
                        break;
                    case 2:
                        if (j > 0 && board[i][j - 1] != WALL && board[i][j - 1] != PACMAN) {
                            if (board[i][j - 1] == FOOD) {
                                // Do not overwrite food, do nothing
                            } else {
                                j--;
                            }
                        }
                        break;
                    case 3:
                        if (j < WIDTH - 1 && board[i][j + 1] != WALL && board[i][j + 1] != PACMAN) {
                            if (board[i][j + 1] == FOOD) {
                                // Do not overwrite food, do nothing
                            } else {
                                j++;
                            }
                        }
                        break;
                }

                // Update the demon's new position
                board[i][j] = DEMON;
            }
        }
    }
}

void draw() {

    // Clear screen
    system("cls");

    // Drawing All the elements on the screen
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
    printf("Chances left: %d\n", chances); 


}

void move(int move_x, int move_y) {
    int x = pacman_x + move_x;
    int y = pacman_y + move_y;

    if (board[y][x] != WALL) {
        if (board[y][x] == FOOD) {
            score++;
            food--;
            curr++;
            if (food == 0) {
                res = 2;
                return;
            }
        } else if (board[y][x] == DEMON) {
            chances--; 
            if (chances == 0) {
                res = 1; 
                return;
            }
        } else if (board[y][x] == POWERUP) {
            
            board[y][x] = EMPTY; 
            powerupCount--;

            if (powerupCount == 0) {
                
                int numDemonsToReduce = 2;
                int count = 0;

                while (count < numDemonsToReduce) {
                    int i = rand() % HEIGHT;
                    int j = rand() % WIDTH;

                    if (board[i][j] == DEMON) {
                        board[i][j] = EMPTY;
                        count++;
                    }
                }
            }
        }

        board[pacman_y][pacman_x] = EMPTY;
        pacman_x = x;
        pacman_y = y;
        board[pacman_y][pacman_x] = PACMAN;
    }
}
void saveScore(const char* playerName, int score) {
    FILE* file = fopen("scores.txt", "a"); // Open file in append mode
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s %d\n", playerName, score); // Write the player name and score to the file

    fclose(file); // Close the file
}

void displayWelcomeMessage(int level) {
    system("cls");
    printf("Welcome to Level %d!\n", level);
  
    Sleep(2000);
}
void printmadeby() {
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                 MADE BY                                                                          *");
	printf("*                                                            REAL WORLD                                                            *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                MUHAMMAD TAMSEEL                             ZARYAB                                             ZUBAIR            *");
	printf("*                   23K-2063                                 23K-2044                                             23K-2071         *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
    printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	printf("*                                                                                                                                  *");
	
}
void printBigP() {
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("**************                  *********              *********                   ************ **     ** **************\n");
    printf("**************    **********    *********   ********   *********    *************************** ** *** ** **************\n");
    printf("**************    **********    *********   ********   *********    *************************** **  *  ** **************\n");
    printf("**************    **********    *********   ********   *********    ****************************************************\n");
    printf("**************    **********    *********   ********   *********    ****************************************************\n");
    printf("**************    **********    *********   ********   *********    ***************************** *** ******************\n");
    printf("**************                  *********              *********    **************************** ** ** *****************\n");
    printf("**************    ***********************   ********   *********    *************************** ******* ****************\n");
    printf("**************    ***********************   ********   *********    ************************** **     ** ***************\n");
    printf("**************    ***********************   ********   *********    ****************************************************\n");
    printf("**************    ***********************   ********   *********                   *************************************\n");
    printf("*********************************************************************************************** **     ** **************\n");
    printf("*********************************************************************************************** ** **  ** **************\n");
    printf("*********************************************************************************************** **  ** ** **************\n");
    printf("*********************************************************************************************** **    *** **************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
    printf("************************************************************************************************************************\n");
}

int main() {
	load_screen();
	char playerName[50];
//	playBackgroundMusic();

	printmadeby();
	Sleep(3000);
    
    system("cls");

	load_screen();
	printBigP();
	

    // Sleep for 4 seconds
    Sleep(4000);
    
    system("cls");
    load_screen();
    
    int count = 0;
    
int level=1;
    initialize(level);
  
    char ch;
    int totalFood = food;

    printf("  \n\n                    GAME RULE\n\n     'W' FOR UP\n     'A' FOR LEFT\n     'D' FOR RIGHT\n     'S' FOR DOWN\n\n                    Enter 'S' TO START\n");
    printf("                    ENTER 'Q' TO QUIT ");

    ch = getch();
    if (ch != 's' && ch != 's') {
        printf("Exit Game!\n");
        return 1;
    }
    load_screen();
 printf("Enter your name: ");
   fgets(playerName, sizeof(playerName), stdin);
playerName[strcspn(playerName, "\n")] = '\0';

    while (1){
	
        draw();
        printf("Total Food count: %d\n", totalFood);
        printf("Total Food eaten: %d\n", curr);

        if (res == 1) {
            count++;
            if (count == 3) {
                system("cls");
                printf("Game Over! Dead by Demon\nYour Score: %d\n", score);
                 saveScore(playerName, score);
                return 1;
            } else {
                printf("Chances left: %d\n", chances);
            }
        }

       if (res == 2) {
            system("cls");
            printf("You Win!\nYour Score: %d\n", score);
              saveScore(playerName, score);
            // Increase the level and restart the game
               displayWelcomeMessage(level + 1);
            level++;
            restartGame();
            initialize(level);
    }
        // Process player's move
        ch = getch();
        switch (ch) {
    
            case 'w':
                move(0, -1);
                break;
            case 's':
                move(0, 1);
                break;
            case 'a':
                move(-1, 0);
                break;
            case 'd':
                move(1, 0);
                break;
            case 'q':
                printf("Game Over! Your Score: %d\n", score);
            
                return 0;
        }

    
        moveDemons();
}

}
