#include "touch.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

// Current orientation
static uint8_t touch_orientation = TOUCH_PORTRAIT;

// Calibration factors
static float cal_x_factor = 1.0;
static float cal_y_factor = 1.0;
static uint16_t cal_x_offset = 0;
static uint16_t cal_y_offset = 0;

/**
 * @brief SPI transfer function for touch controller
 * @param data Byte to send
 * @return Byte received
 */
static uint8_t TOUCH_SPI_Transfer(uint8_t data) {
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&hspi1, &data, &rx_data, 1, 100);
    return rx_data;
}

void TOUCH_Init(void) {
    // Initialize CS pin
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure CS pin
    GPIO_InitStruct.Pin = TOUCH_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStruct);

    // Configure IRQ pin
    GPIO_InitStruct.Pin = TOUCH_IRQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(TOUCH_IRQ_PORT, &GPIO_InitStruct);

    // Set CS high (inactive)
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);

    // Set default calibration
    TOUCH_Calibrate(TOUCH_X_MIN, TOUCH_Y_MIN, TOUCH_X_MAX, TOUCH_Y_MAX);
}

uint8_t TOUCH_ReadRaw(uint16_t *x, uint16_t *y) {
    uint8_t tx_data[3];
    uint8_t rx_data[3];
    uint16_t raw_x, raw_y;

    // Check if touch is pressed (IRQ pin low)
    if (HAL_GPIO_ReadPin(TOUCH_IRQ_PORT, TOUCH_IRQ_PIN) == GPIO_PIN_SET) {
        return 0; // No touch
    }

    // Select touch controller
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_RESET);

    // Read X coordinate
    tx_data[0] = TOUCH_CMD_X;
    tx_data[1] = 0x00;
    tx_data[2] = 0x00;

    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 3, 100);
    raw_x = ((rx_data[1] << 8) | rx_data[2]) >> 3;

    // Small delay between readings
    HAL_Delay(1);

    // Read Y coordinate
    tx_data[0] = TOUCH_CMD_Y;
    tx_data[1] = 0x00;
    tx_data[2] = 0x00;

    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 3, 100);
    raw_y = ((rx_data[1] << 8) | rx_data[2]) >> 3;

    // Deselect touch controller
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);

    // Check if readings are valid
    if (raw_x < 100 || raw_x > 4000 || raw_y < 100 || raw_y > 4000) {
        return 0;
    }

    *x = raw_x;
    *y = raw_y;
    return 1;
}

uint8_t TOUCH_Read(uint16_t *x, uint16_t *y) {
    uint16_t raw_x, raw_y;

    if (!TOUCH_ReadRaw(&raw_x, &raw_y)) {
        return 0;
    }

    // Apply calibration
    uint16_t cal_x = (uint16_t)((raw_x - cal_x_offset) * cal_x_factor);
    uint16_t cal_y = (uint16_t)((raw_y - cal_y_offset) * cal_y_factor);

    // Apply orientation transformation
    switch (touch_orientation) {
        case TOUCH_PORTRAIT:
            *x = cal_x;
            *y = cal_y;
            break;
        case TOUCH_LANDSCAPE:
            *x = cal_y;
            *y = TOUCH_SCREEN_WIDTH - cal_x;
            break;
        case TOUCH_PORTRAIT_180:
            *x = TOUCH_SCREEN_WIDTH - cal_x;
            *y = TOUCH_SCREEN_HEIGHT - cal_y;
            break;
        case TOUCH_LANDSCAPE_270:
            *x = TOUCH_SCREEN_HEIGHT - cal_y;
            *y = cal_x;
            break;
        default:
            *x = cal_x;
            *y = cal_y;
            break;
    }

    // Ensure coordinates are within screen bounds
    if (*x >= TOUCH_SCREEN_WIDTH) *x = TOUCH_SCREEN_WIDTH - 1;
    if (*y >= TOUCH_SCREEN_HEIGHT) *y = TOUCH_SCREEN_HEIGHT - 1;

    return 1;
}

void TOUCH_SetOrientation(uint8_t orientation) {
    touch_orientation = orientation;
}

uint8_t TOUCH_GetOrientation(void) {
    return touch_orientation;
}

void TOUCH_Calibrate(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    // Simple calibration - adjust these based on your display
    cal_x_factor = (float)TOUCH_SCREEN_WIDTH / (x2 - x1);
    cal_y_factor = (float)TOUCH_SCREEN_HEIGHT / (y2 - y1);
    cal_x_offset = x1;
    cal_y_offset = y1;
}

uint8_t TOUCH_IsPressed(void) {
    return (HAL_GPIO_ReadPin(TOUCH_IRQ_PORT, TOUCH_IRQ_PIN) == GPIO_PIN_RESET);
}

uint8_t TOUCH_GetState(Touch_State *state) {
    static uint8_t last_pressed = 0;
    uint8_t current_pressed = TOUCH_IsPressed();

    // Check for state change
    if (current_pressed != last_pressed) {
        last_pressed = current_pressed;
        state->pressed = current_pressed;

        if (current_pressed) {
            // Read coordinates only when pressed
            if (TOUCH_Read(&state->x, &state->y)) {
                state->orientation = touch_orientation;
                return 1; // State changed
            }
        } else {
            // Touch released
            state->x = 0;
            state->y = 0;
            return 1; // State changed
        }
    }

    return 0; // No state change
}

uint16_t TOUCH_ReadPressure(void) {
    uint8_t tx_data[3];
    uint8_t rx_data[3];
    uint16_t z1, z2;

    // Select touch controller
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_RESET);

    // Read Z1
    tx_data[0] = TOUCH_CMD_Z1;
    tx_data[1] = 0x00;
    tx_data[2] = 0x00;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 3, 100);
    z1 = ((rx_data[1] << 8) | rx_data[2]) >> 3;

    // Small delay
    HAL_Delay(1);

    // Read Z2
    tx_data[0] = TOUCH_CMD_Z2;
    tx_data[1] = 0x00;
    tx_data[2] = 0x00;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 3, 100);
    z2 = ((rx_data[1] << 8) | rx_data[2]) >> 3;

    // Deselect touch controller
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);

    // Calculate pressure (simplified)
    if (z1 == 0 || z2 == 0) return 0;
    return 4096 - z2 + z1;
}
