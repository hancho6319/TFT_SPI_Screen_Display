#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "tft_display.h"
#include "stdlib.h"
#include "stdio.h"  // Add this for sprintf
#include <limits.h> // For INT_MAX

#define SNAKE_MAX_LENGTH 100
#define GRID_SIZE 10
#define GRID_WIDTH (TFT_WIDTH_LANDSCAPE / GRID_SIZE)
#define GRID_HEIGHT (TFT_HEIGHT_LANDSCAPE / GRID_SIZE)

// Add these color definitions
#define TFT_DARKGREEN    0x0320
#define TFT_DARKGRAY     0x4208

typedef enum {
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT
} Direction;

typedef struct {
    uint16_t x;
    uint16_t y;
} Point;

typedef struct {
    Point segments[SNAKE_MAX_LENGTH];
    uint16_t length;
    Direction direction;
    uint16_t score;
} Snake;

typedef struct {
    Point position;
    uint8_t active;
} Food;

// Function prototypes
void SnakeGame_Init(void);
void SnakeGame_Run(void);
void SnakeGame_Update(void);
void SnakeGame_Draw(void);
void SnakeGame_GenerateFood(void);
void SnakeGame_AutoMove(void);
uint8_t SnakeGame_CheckCollision(void);
Direction GetOppositeDirection(Direction dir); // Add this prototype

#endif
