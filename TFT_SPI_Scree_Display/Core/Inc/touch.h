#ifndef TOUCH_H
#define TOUCH_H

#include "main.h"
#include "stdint.h"

//TFT_VCC     → 3.3V
//TFT_GND     → GND
//TFT_CS      → PA4   (SPI1 Chip Select - Display)
//TFT_DC      → PA1   (Data/Command)
//TFT_RST     → PA0   (Reset)
//TFT_SDA     → PA7   (SPI1 MOSI)
//TFT_SCL     → PA5   (SPI1 SCK)
//
//TOUCH_CS    → PA2   (Touch Chip Select)
//TOUCH_IRQ   → PA3   (Touch Interrupt)
//TOUCH_DIN   → PA7   (SPI1 MOSI - shared with TFT)
//TOUCH_DO    → PA6   (SPI1 MISO - this is important!)
//TOUCH_CLK   → PA5   (SPI1 SCK - shared with TFT)

// Touch controller commands
#define TOUCH_CMD_X         0xD0
#define TOUCH_CMD_Y         0x90
#define TOUCH_CMD_Z1        0xB0
#define TOUCH_CMD_Z2        0xC0

// Touch calibration values (adjust based on your display)
#define TOUCH_X_MIN         150
#define TOUCH_X_MAX         3850
#define TOUCH_Y_MIN         200
#define TOUCH_Y_MAX         3750
#define TOUCH_SCREEN_WIDTH  240
#define TOUCH_SCREEN_HEIGHT 320

// Orientation definitions
#define TOUCH_PORTRAIT      0
#define TOUCH_LANDSCAPE     1
#define TOUCH_PORTRAIT_180  2
#define TOUCH_LANDSCAPE_270 3

// GPIO Pins
#define TOUCH_CS_PIN        GPIO_PIN_2
#define TOUCH_CS_PORT       GPIOA
#define TOUCH_IRQ_PIN       GPIO_PIN_3
#define TOUCH_IRQ_PORT      GPIOA

// Touch state structure
typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t pressed;
    uint8_t orientation;
} Touch_State;

/**
 * @brief Initialize the touch screen controller
 * @note Sets up SPI and GPIO for touch communication
 * @example TOUCH_Init();
 */
void TOUCH_Init(void);

/**
 * @brief Read raw touch coordinates from controller
 * @param x Pointer to store raw X coordinate
 * @param y Pointer to store raw Y coordinate
 * @return 1 if touch detected, 0 if no touch
 * @example
 * uint16_t x, y;
 * if (TOUCH_ReadRaw(&x, &y)) { /* Touch detected *\/ }
 */
uint8_t TOUCH_ReadRaw(uint16_t *x, uint16_t *y);

/**
 * @brief Read calibrated touch coordinates
 * @param x Pointer to store calibrated X coordinate (0-239)
 * @param y Pointer to store calibrated Y coordinate (0-319)
 * @return 1 if touch detected, 0 if no touch
 * @example
 * uint16_t touch_x, touch_y;
 * if (TOUCH_Read(&touch_x, &touch_y)) {
 *     // touch_x and touch_y are screen coordinates
 * }
 */
uint8_t TOUCH_Read(uint16_t *x, uint16_t *y);

/**
 * @brief Set touch screen orientation
 * @param orientation Orientation mode (TOUCH_PORTRAIT, TOUCH_LANDSCAPE, etc.)
 * @example TOUCH_SetOrientation(TOUCH_LANDSCAPE);
 */
void TOUCH_SetOrientation(uint8_t orientation);

/**
 * @brief Get current touch orientation
 * @return Current orientation setting
 * @example uint8_t orient = TOUCH_GetOrientation();
 */
uint8_t TOUCH_GetOrientation(void);

/**
 * @brief Calibrate touch screen with known points
 * @param x1 Known X coordinate 1
 * @param y1 Known Y coordinate 1
 * @param x2 Known X coordinate 2
 * @param y2 Known Y coordinate 2
 * @note Call this with known screen coordinates for calibration
 * @example TOUCH_Calibrate(50, 50, 200, 300);
 */
void TOUCH_Calibrate(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief Check if touch is currently pressed
 * @return 1 if pressed, 0 if not pressed
 * @example if (TOUCH_IsPressed()) { /* Handle touch *\/ }
 */
uint8_t TOUCH_IsPressed(void);

/**
 * @brief Get touch state with debouncing
 * @param state Pointer to Touch_State structure to fill
 * @return 1 if touch state changed, 0 if no change
 * @example
 * Touch_State touch;
 * if (TOUCH_GetState(&touch)) {
 *     if (touch.pressed) {
 *         TFT_printf(10, 10, TFT_WHITE, TRANSPARENT, 2, "Touch at %d,%d", touch.x, touch.y);
 *     }
 * }
 */
uint8_t TOUCH_GetState(Touch_State *state);

/**
 * @brief Read touch pressure (Z value)
 * @return Pressure value (higher = more pressure)
 * @example uint16_t pressure = TOUCH_ReadPressure();
 */
uint16_t TOUCH_ReadPressure(void);

#endif
