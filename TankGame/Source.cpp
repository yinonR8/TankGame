#define _CRT_SECURE_NO_WARNINGS
#define SECURITY_SIZE 2

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Struct declaration
struct Location
{
    int nRow;
    int nCol;
};

// Function prototypes
void MoveTank(char** ioppcBoard, struct Location* ioplocTank, struct Location* iarrlocDirections);
int Shoot(char** ioppcBoard, struct Location* ioplocTank, struct Location* iarrlocDirections);
int ShootChange(char** ioppcBoard, struct Location* ioplocTank, struct Location* iarrlocDirections);
int AirStrike(char** ioppcBoard, int inFullSize);
int CountWalls(char** ippcBoard, int inFullSize);
void PrintBoard(char** ippcBoard, int inSize);

//---
//
// Tank Strategy Game
//
// General : The program simulates a game between two tanks on a dynamically allocated board.
//
// Input   : Board size and user commands (move, shoot, airstrike) for each turn.
//
// Process : Manages turns updates board state handles collisions and shooting logic.
//
// Output  : The game board status after each turn and game over messages.
//
//---
// Programmer : Yinon Refaeli
// Student No : 216689711
// Date       : 12.12.2025
//---
void main()
{
    // Array definition
    // Direction array: 0=Up, 1=Right, 2=Down, 3=Left
    struct Location arrlocDirections[4] =
    {
        {-1,  0}, // Up 0
        { 0,  1}, // Rigth 1
        { 1,  0}, // Down 2
        { 0, -1}  // Left 3
    };

    // Variable definition
    int nButton = 0;
    int nSize = 0;
    int nFullSize = 0;
    int nTurn = 0;
    int nLeftTankCol = 0;
    int nRightTankCol = 0;
    int nMidRow = 0;
    int nRowIndex = 0;
    int nColIndex = 0;
    char** ppcBoard;
    struct Location* plocCurrentTank;
    struct Location locLeftTank;
    struct Location locRightTank;

    // Code section
    srand((unsigned)time(NULL));

    printf("Give The Board Length, It Have To Be Odd: ");
    scanf("%d", &nSize);

    // Validate board size
    if (nSize % 2 == 0)
    {
        printf("Board Size Is Even, Please Try Again ");
        return;
    }

    nFullSize = nSize + (SECURITY_SIZE * 2);

    // Allocate memory for board rows
    ppcBoard = (char**)malloc(nFullSize * sizeof(char*));
    if (ppcBoard == NULL)
    {
        printf("Memory Failed");
        return;
    }

    // Allocate memory for board columns
    for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
    {
        ppcBoard[nRowIndex] = (char*)malloc(nFullSize * sizeof(char));
        if (ppcBoard[nRowIndex] == NULL)
        {
            printf("Memory Failed");
            return;
        }
    }

    // Initialize board
    nLeftTankCol = SECURITY_SIZE;
    nRightTankCol = nFullSize - SECURITY_SIZE - 1;

    for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
    {
        for (nColIndex = 0; nColIndex < nFullSize; nColIndex++)
        {
            // Security borders
            if (nRowIndex < SECURITY_SIZE || nRowIndex >= nFullSize - SECURITY_SIZE ||
                nColIndex < SECURITY_SIZE || nColIndex >= nFullSize - SECURITY_SIZE)
            {
                ppcBoard[nRowIndex][nColIndex] = '*';
            }
            // Breakable walls
            else if (nRowIndex % 2 == 0 && nColIndex % 2 == 0 &&
                nColIndex > nLeftTankCol && nColIndex < nRightTankCol)
            {
                ppcBoard[nRowIndex][nColIndex] = 'X';
            }
            // Empty space
            else
            {
                ppcBoard[nRowIndex][nColIndex] = ' ';
            }
        }
    }

    // Place tanks
    nMidRow = nFullSize / 2;
    ppcBoard[nMidRow][nLeftTankCol] = 'O';
    ppcBoard[nMidRow][nRightTankCol] = 'O';

    locLeftTank.nRow = nMidRow;
    locLeftTank.nCol = nLeftTankCol;
    locRightTank.nRow = nMidRow;
    locRightTank.nCol = nRightTankCol;

    // Game loop
    while (1)
    {
        // Determine turn
        if (nTurn % 2 == 0)
        {
            printf("Left Tank Turn");
            plocCurrentTank = &locLeftTank;
        }
        else
        {
            printf("Right Tank Turn");
            plocCurrentTank = &locRightTank;
        }

        printf("\nChoose What To Do:\n");
        printf("1. Move\n");
        printf("2. Shoot\n");
        printf("3. Shoot Changer\n");
        printf("4. Airstrike\n");
        printf("999. Exit\n");
        scanf("%d", &nButton);

        switch (nButton)
        {
        case (1):
        {
            MoveTank(ppcBoard, plocCurrentTank, arrlocDirections);
            nTurn++;
            PrintBoard(ppcBoard, nFullSize);
            break;
        }
        case (2):
        {
            if (Shoot(ppcBoard, plocCurrentTank, arrlocDirections) == 1)
            {
                PrintBoard(ppcBoard, nFullSize);
                printf("Game Over, You Won");

                for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
                {
                    free(ppcBoard[nRowIndex]);
                }
                free(ppcBoard);
                return;
            }
            else
            {
                if (CountWalls(ppcBoard, nFullSize) == 0)
                {
                    PrintBoard(ppcBoard, nFullSize);
                    printf("Game Over No Walls Left Standing, Draw");

                    for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
                    {
                        free(ppcBoard[nRowIndex]);
                    }
                    free(ppcBoard);
                    return;
                }
                else
                {
                    nTurn++;
                    PrintBoard(ppcBoard, nFullSize);
                }
            }
            break;
        }
        case (3):
        {
            if (ShootChange(ppcBoard, plocCurrentTank, arrlocDirections) == 1)
            {
                PrintBoard(ppcBoard, nFullSize);
                printf("Game Over, You Won");

                for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
                {
                    free(ppcBoard[nRowIndex]);
                }
                free(ppcBoard);
                return;
            }
            else
            {
                if (CountWalls(ppcBoard, nFullSize) == 0)
                {
                    PrintBoard(ppcBoard, nFullSize);
                    printf("Game Over No Walls Left Standing, Draw");

                    for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
                    {
                        free(ppcBoard[nRowIndex]);
                    }
                    free(ppcBoard);
                    return;
                }
                else
                {
                    nTurn++;
                    PrintBoard(ppcBoard, nFullSize);
                }
            }
            break;
        }
        case (4):
        {
            if (AirStrike(ppcBoard, nFullSize) == 1)
            {
                PrintBoard(ppcBoard, nFullSize);
                printf("Game Over, You Won");

                for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
                {
                    free(ppcBoard[nRowIndex]);
                }
                free(ppcBoard);
                return;
            }
            else
            {
                if (CountWalls(ppcBoard, nFullSize) == 0)
                {
                    PrintBoard(ppcBoard, nFullSize);
                    printf("Game Over No Walls Left Standing, Draw");

                    for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
                    {
                        free(ppcBoard[nRowIndex]);
                    }
                    free(ppcBoard);
                    return;
                }
                else
                {
                    nTurn++;
                    PrintBoard(ppcBoard, nFullSize);
                }
            }
            break;
        }
        case (999):
        {
            printf("Exiting The Game");
            for (nRowIndex = 0; nRowIndex < nFullSize; nRowIndex++)
            {
                free(ppcBoard[nRowIndex]);
            }
            free(ppcBoard);
            return;
        }
        default:
        {
            printf("Invalid.\n");
            break;
        }
        }
    }
}

//---
//
// Count Walls
//
// General    : Counts the number of breakable walls remaining on the board.
//
// Parameters :
// ippcBoard
// The game board (In)
// inFullSize
// The size of the board (In)
//
// Return Value : The number of walls ('X') found.
//
//---
int CountWalls(char** ippcBoard, int inFullSize)
{
    // Variable definition
    int nCount = 0;
    int nRow = 0;
    int nCol = 0;

    // Code section
    for (nRow = 0; nRow < inFullSize; nRow++)
    {
        for (nCol = 0; nCol < inFullSize; nCol++)
        {
            if (ippcBoard[nRow][nCol] == 'X')
            {
                nCount++;
            }
        }
    }

    return (nCount);
}

//---
//
// Move Tank
//
// General    : Moves a tank on the board based on user input direction and steps.
//
// Parameters :
// ioppcBoard
// The game board to update (In/Out)
// ioplocTank
// The current location of the tank (In/Out)
// iarrlocDirections
// Array of direction offsets (In)
//
// Return Value : None.
//
//---
void MoveTank(char** ioppcBoard, struct Location* ioplocTank, struct Location* iarrlocDirections)
{
    // Variable definition
    int nDirection = 0;
    int nSteps = 0;
    int nStepIndex = 0;
    int nNextRow = 0;
    int nNextCol = 0;

    // Code section
    printf("How Many Steps Do You Want To Take: ");
    scanf("%d", &nSteps);
    printf("Enter The Direction: \n1: Up\n2: Right\n3: Down\n4: Left ");
    scanf("%d", &nDirection);
    nDirection--; // Convert 1-4 to 0-3

    if (nDirection < 0 || nDirection > 3)
    {
        printf("Direction Is Invalid");
        return;
    }

    for (nStepIndex = 0; nStepIndex < nSteps; nStepIndex++)
    {
        nNextRow = ioplocTank->nRow + iarrlocDirections[nDirection].nRow;
        nNextCol = ioplocTank->nCol + iarrlocDirections[nDirection].nCol;

        if (ioppcBoard[nNextRow][nNextCol] == ' ')
        {
            ioppcBoard[ioplocTank->nRow][ioplocTank->nCol] = ' ';
            ioplocTank->nRow = nNextRow;
            ioplocTank->nCol = nNextCol;
            ioppcBoard[ioplocTank->nRow][ioplocTank->nCol] = 'O';
        }
        else
        {
            printf("Direction Blocked ");
            break;
        }
    }
}

//---
//
// Shoot
//
// General    : Simulates a straight shot from the tank.
//
// Parameters :
// ioppcBoard
// The game board (In/Out)
// ioplocTank
// The location of the shooting tank (In/Out)
// iarrlocDirections
// Array of direction offsets (In)
//
// Return Value : 1 if a tank is destroyed (Win), 0 otherwise.
//
//---
int Shoot(char** ioppcBoard, struct Location* ioplocTank, struct Location* iarrlocDirections)
{
    // Variable definition
    int nBulletRow = ioplocTank->nRow;
    int nBulletCol = ioplocTank->nCol;
    int nDirection = 0;

    // Code section
    printf("Enter The Direction: \n1: Up\n2: Right\n3: Down\n4: Left ");
    scanf("%d", &nDirection);
    nDirection--; // Convert 1-4 to 0-3

    if (nDirection < 0 || nDirection > 3)
    {
        printf("Direction Is Invalid");
        return (0);
    }

    while (1)
    {
        nBulletRow += iarrlocDirections[nDirection].nRow;
        nBulletCol += iarrlocDirections[nDirection].nCol;

        if (ioppcBoard[nBulletRow][nBulletCol] == 'X')
        {
            ioppcBoard[nBulletRow][nBulletCol] = ' ';
            printf("A Wall Has Been Hit");
            return (0);
        }
        else if (ioppcBoard[nBulletRow][nBulletCol] == 'O')
        {
            ioppcBoard[nBulletRow][nBulletCol] = ' ';
            printf("Tank Destroyed You Won");
            return (1);
        }
        else if (ioppcBoard[nBulletRow][nBulletCol] == '*')
        {
            printf("Wall Hit Border");
            return (0);
        }
    }
}

//---
//
// Shoot Change
//
// General    : Simulates a shot that bounces right when hitting a border.
//
// Parameters :
// ioppcBoard
// The game board (In/Out)
// ioplocTank
// The location of the shooting tank (In/Out)
// iarrlocDirections
// Array of direction offsets (In)
//
// Return Value : 1 if a tank is destroyed (Win), 0 otherwise.
//
//---
int ShootChange(char** ioppcBoard, struct Location* ioplocTank, struct Location* iarrlocDirections)
{
    // Variable definition
    int nBulletRow = ioplocTank->nRow;
    int nBulletCol = ioplocTank->nCol;
    int nBounces = 0;
    int nDirection = 0;
    int nNextRow = 0;
    int nNextCol = 0;

    // Code section
    printf("Enter The Direction: \n1: Up\n2: Right\n3: Down\n4: Left ");
    scanf("%d", &nDirection);
    nDirection--; // Convert 1-4 to 0-3

    if (nDirection < 0 || nDirection > 3)
    {
        printf("Direction Is Invalid");
        return (0);
    }

    while (1)
    {
        nNextRow = nBulletRow + iarrlocDirections[nDirection].nRow;
        nNextCol = nBulletCol + iarrlocDirections[nDirection].nCol;

        if (ioppcBoard[nNextRow][nNextCol] == 'X')
        {
            ioppcBoard[nNextRow][nNextCol] = ' ';
            printf("A Wall Has Been Hit");
            return (0);
        }
        else if (ioppcBoard[nNextRow][nNextCol] == 'O')
        {
            ioppcBoard[nNextRow][nNextCol] = ' ';
            printf("Tank Destroyed You Won");
            return (1);
        }
        else if (ioppcBoard[nNextRow][nNextCol] == '*')
        {
            if (nBounces < 5)
            {
                nDirection = (nDirection + 1) % 4;
                nBounces++;
                printf("Bullet Bounced %d times ", nBounces);
            }
            else
            {
                printf("Bullet Hit Maximum Bounces And Have Hit Border");
                return (0);
            }
        }
        else
        {
            nBulletRow = nNextRow;
            nBulletCol = nNextCol;
        }
    }
}

//---
//
// Air Strike
//
// General    : Performs a random area attack (3x3) around target coordinates.
//
// Parameters :
// ioppcBoard
// The game board (In/Out)
// inFullSize
// The size of the board (In)
//
// Return Value : 1 if a tank is destroyed, 0 otherwise.
//
//---
int AirStrike(char** ioppcBoard, int inFullSize)
{
    // Variable definition
    int nTargetRow = 0;
    int nTargetCol = 0;
    int nVictory = 0;
    int nRowOffset = 0;
    int nColOffset = 0;
    int nCurrRow = 0;
    int nCurrCol = 0;

    // Code section
    printf("Enter Target Coordinates To AirStrike: Row, Col: ");
    scanf("%d %d", &nTargetRow, &nTargetCol);

    for (nRowOffset = -1; nRowOffset <= 1; nRowOffset++)
    {
        for (nColOffset = -1; nColOffset <= 1; nColOffset++)
        {
            nCurrRow = nTargetRow + nRowOffset;
            nCurrCol = nTargetCol + nColOffset;

            if (nCurrRow >= 0 && nCurrRow < inFullSize &&
                nCurrCol >= 0 && nCurrCol < inFullSize &&
                ioppcBoard[nCurrRow][nCurrCol] != '*')
            {
                if (rand() % 2 == 1)
                {
                    if (ioppcBoard[nCurrRow][nCurrCol] == 'X')
                    {
                        ioppcBoard[nCurrRow][nCurrCol] = ' ';
                        printf("Wall Destroyed At %d,%d", nCurrRow, nCurrCol);
                    }
                    else if (ioppcBoard[nCurrRow][nCurrCol] == 'O')
                    {
                        ioppcBoard[nCurrRow][nCurrCol] = ' ';
                        nVictory = 1;
                        printf("Tank Destroyed At %d,%d", nCurrRow, nCurrCol);
                    }
                }
            }
        }
    }

    return (nVictory);
}

//---
//
// Print Board
//
// General    : Prints the current state of the game board to the console.
//
// Parameters :
// ippcBoard
// The game board to print (In)
// inSize
// The size of the board (In)
//
// Return Value : None.
//
//---
void PrintBoard(char** ippcBoard, int inSize)
{
    // Variable definition
    int nRowIndex = 0;
    int nColIndex = 0;

    // Code section
    printf("\n"); // Aesthetic space before board

    for (nRowIndex = 0; nRowIndex < inSize; nRowIndex++)
    {
        for (nColIndex = 0; nColIndex < inSize; nColIndex++)
        {
            printf("%c ", ippcBoard[nRowIndex][nColIndex]);
        }
        printf("\n");
    }

    printf("\n"); // Aesthetic space after board
}