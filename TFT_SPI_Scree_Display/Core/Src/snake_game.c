#include "snake_game.h"
#include "main.h"

Snake snake;
Food food;
uint8_t gameRunning = 0;
uint32_t lastUpdateTime = 0;
uint32_t gameSpeed = 200; // milliseconds between moves

Direction GetOppositeDirection(Direction dir) {
    switch (dir) {
        case DIRECTION_UP: return DIRECTION_DOWN;
        case DIRECTION_RIGHT: return DIRECTION_LEFT;
        case DIRECTION_DOWN: return DIRECTION_UP;
        case DIRECTION_LEFT: return DIRECTION_RIGHT;
    }
    return DIRECTION_RIGHT;
}

void SnakeGame_Init(void) {
    TFT_Init();
    TFT_SetRotation(TFT_ROTATION_90); // Landscape mode

    // Initialize snake
    snake.length = 3;
    snake.direction = DIRECTION_RIGHT;
    snake.score = 0;

    // Initial snake position (centered)
    for (int i = 0; i < snake.length; i++) {
        snake.segments[i].x = GRID_WIDTH / 2 - i;
        snake.segments[i].y = GRID_HEIGHT / 2;
    }

    // Generate first food
    SnakeGame_GenerateFood();

    gameRunning = 1;
    lastUpdateTime = HAL_GetTick();
}

void SnakeGame_Run(void) {
    SnakeGame_Init();
    TFT_FillScreen(TFT_BLACK);

    while (gameRunning) {
        uint32_t currentTime = HAL_GetTick();

        if (currentTime - lastUpdateTime >= gameSpeed) {
            SnakeGame_Update();
            SnakeGame_Draw();
            lastUpdateTime = currentTime;

            // Increase speed as score increases
            if (snake.score > 0 && snake.score % 5 == 0) {
                gameSpeed = 200 - (snake.score / 5) * 20;
                if (gameSpeed < 50) gameSpeed = 50;
            }
        }

        HAL_Delay(10);
    }

    // Game over screen
    TFT_FillScreen(TFT_BLACK);
    TFT_DrawStringCentered(100, "GAME OVER", TFT_RED, TFT_BLACK);

    char scoreStr[20];
    sprintf(scoreStr, "Score: %d", snake.score);
    TFT_DrawStringCentered(120, scoreStr, TFT_WHITE, TFT_BLACK);
    TFT_DrawStringCentered(140, "Press Reset", TFT_WHITE, TFT_BLACK);
}

void SnakeGame_Update(void) {
    SnakeGame_AutoMove();

    // Check for food collision
    if (snake.segments[0].x == food.position.x &&
        snake.segments[0].y == food.position.y) {
        // Increase snake length
        if (snake.length < SNAKE_MAX_LENGTH) {
            snake.length++;
            snake.score += 10;

            // Generate new food
            SnakeGame_GenerateFood();
        }
    }

    // Check for collisions
    if (SnakeGame_CheckCollision()) {
        gameRunning = 0;
    }
}

void SnakeGame_Draw(void) {
    TFT_FillScreen(TFT_BLACK);

    // Draw grid (optional)
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            TFT_DrawRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, TFT_DARKGRAY);
        }
    }

    // Draw snake
    for (int i = 0; i < snake.length; i++) {
        uint16_t color;
        if (i == 0) {
            color = TFT_GREEN; // Head
        } else {
            color = TFT_DARKGREEN; // Body
        }

        TFT_DrawFilledRect(
            snake.segments[i].x * GRID_SIZE + 1,
            snake.segments[i].y * GRID_SIZE + 1,
            GRID_SIZE - 2,
            GRID_SIZE - 2,
            color
        );
    }

    // Draw food
    TFT_DrawFilledRect(
        food.position.x * GRID_SIZE + 1,
        food.position.y * GRID_SIZE + 1,
        GRID_SIZE - 2,
        GRID_SIZE - 2,
        TFT_RED
    );

    // Draw score
    char scoreStr[20];
    sprintf(scoreStr, "Score: %d", snake.score);
    TFT_DrawString(5, 5, scoreStr, TFT_WHITE, TFT_BLACK);
}

void SnakeGame_GenerateFood(void) {
    uint8_t validPosition = 0;

    while (!validPosition) {
        food.position.x = rand() % GRID_WIDTH;
        food.position.y = rand() % GRID_HEIGHT;

        validPosition = 1;
        // Check if food overlaps with snake
        for (int i = 0; i < snake.length; i++) {
            if (food.position.x == snake.segments[i].x &&
                food.position.y == snake.segments[i].y) {
                validPosition = 0;
                break;
            }
        }
    }

    food.active = 1;
}

void SnakeGame_AutoMove(void) {
    // Simple AI: move towards food
    int16_t dx = food.position.x - snake.segments[0].x;
    int16_t dy = food.position.y - snake.segments[0].y;

    // Store current direction for decision making
    Direction currentDir = snake.direction;
    Direction preferredDir;

    // Prefer horizontal movement if food is farther in x direction
    if (abs(dx) > abs(dy)) {
        preferredDir = (dx > 0) ? DIRECTION_RIGHT : DIRECTION_LEFT;
    } else {
        preferredDir = (dy > 0) ? DIRECTION_DOWN : DIRECTION_UP;
    }

    // Check if preferred direction is valid (won't cause immediate collision)
    Direction newDir = currentDir;

    // Try preferred direction first
    if (preferredDir != GetOppositeDirection(currentDir)) {
        newDir = preferredDir;
    }
    // If preferred direction is opposite, choose perpendicular direction
    else if (abs(dx) > abs(dy)) {
        newDir = (dy > 0) ? DIRECTION_DOWN : DIRECTION_UP;
    } else {
        newDir = (dx > 0) ? DIRECTION_RIGHT : DIRECTION_LEFT;
    }

    snake.direction = newDir;

    // Move snake body
    for (int i = snake.length - 1; i > 0; i--) {
        snake.segments[i] = snake.segments[i - 1];
    }

    // Move head based on direction
    switch (snake.direction) {
        case DIRECTION_UP:
            snake.segments[0].y--;
            break;
        case DIRECTION_RIGHT:
            snake.segments[0].x++;
            break;
        case DIRECTION_DOWN:
            snake.segments[0].y++;
            break;
        case DIRECTION_LEFT:
            snake.segments[0].x--;
            break;
    }
}

uint8_t SnakeGame_CheckCollision(void) {
    Point head = snake.segments[0];

    // Wall collision
    if (head.x >= GRID_WIDTH || head.x < 0 ||
        head.y >= GRID_HEIGHT || head.y < 0) {
        return 1;
    }

    // Self collision (skip first segment)
    for (int i = 1; i < snake.length; i++) {
        if (head.x == snake.segments[i].x && head.y == snake.segments[i].y) {
            return 1;
        }
    }

    return 0;
}
