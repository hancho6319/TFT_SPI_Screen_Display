#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

/********************
 *
 * TFT_VCC  → 3.3V
 * TFT_GND  → GND
 * TFT_CS   → PA4   (SPI1 Chip Select)
 * TFT_DC   → PA1   (Data/Command)
 * TFT_RST  → PA0   (Reset)
 * TFT_SDA  → PA7   (SPI1 MOSI)
 * TFT_SCL  → PA5   (SPI1 SCK)
 * TFT_LED  → 3.3V (Backlight)
 *
 * ******************/

#include "main.h"
#include "stdint.h"
#include <stdint.h>
#include <stdarg.h>

// Add character set definition
#define ASCII_START 32   // Space
#define ASCII_END 126    // Tilde (~)

// Background mode definitions
#define TRANSPARENT 0x0000   // Transparent background (no background drawn)
#define SOLID_BG    0x0001   // Solid background color (draw background rectangle)

// TFT Display Commands
#define TFT_NOP     0x00
#define TFT_SWRESET 0x01
#define TFT_SLPIN   0x10
#define TFT_SLPOUT  0x11
#define TFT_INVOFF  0x20
#define TFT_INVON   0x21
#define TFT_GAMMASET 0x26
#define TFT_DISPOFF 0x28
#define TFT_DISPON  0x29
#define TFT_CASET   0x2A
#define TFT_RASET   0x2B
#define TFT_RAMWR   0x2C
#define TFT_RAMRD   0x2E
#define TFT_MADCTL  0x36
#define TFT_COLMOD  0x3A
#define TFT_FRMCTR1 0xB1
#define TFT_FRMCTR2 0xB2
#define TFT_FRMCTR3 0xB3
#define TFT_INVCTR  0xB4
#define TFT_DFUNCTR 0xB6
#define TFT_PWCTR1  0xC0
#define TFT_PWCTR2  0xC1
#define TFT_PWCTR3  0xC2
#define TFT_PWCTR4  0xC3
#define TFT_PWCTR5  0xC4
#define TFT_VMCTR1  0xC5
#define TFT_VMCTR2  0xC7
#define TFT_GMCTRP1 0xE0
#define TFT_GMCTRN1 0xE1

// Rotation definitions
#define TFT_ROTATION_0     0x48   // Portrait, 0 degrees
#define TFT_ROTATION_90    0x28   // Landscape, 90 degrees
#define TFT_ROTATION_180   0x88   // Portrait, 180 degrees
#define TFT_ROTATION_270   0xE8   // Landscape, 270 degrees

// Color definitions (16-bit RGB565)
#define TFT_BLACK       0x0000
#define TFT_WHITE       0xFFFF
#define TFT_RED         0x001F
#define TFT_GREEN       0x07E0
#define TFT_BLUE        0xF800
#define TFT_CYAN        0xFFE0
#define TFT_MAGENTA     0xFE19
#define TFT_YELLOW      0x07FF
#define TFT_ORANGE      0x867F
#define TFT_PURPLE      0x8010
#define TFT_PINK        0xF81F
#define TFT_BROWN       0x8200
#define TFT_GRAY        0x8410
#define TFT_LIGHT_BLUE  0xFC00

// Display dimensions
#define TFT_WIDTH_PORTRAIT   240
#define TFT_HEIGHT_PORTRAIT  320
#define TFT_WIDTH_LANDSCAPE  320
#define TFT_HEIGHT_LANDSCAPE 240

// GPIO Pins
#define TFT_CS_PIN   GPIO_PIN_4
#define TFT_CS_PORT  GPIOA
#define TFT_DC_PIN   GPIO_PIN_1
#define TFT_DC_PORT  GPIOA
#define TFT_RST_PIN  GPIO_PIN_0
#define TFT_RST_PORT GPIOA

// Symbol and emoji definitions
#define SMILEY_FACE       0x01
#define HEART             0x02
#define STAR              0x03
#define MUSIC_NOTE        0x04
#define CHECK_MARK        0x05
#define CROSS_MARK        0x06
#define ARROW_UP          0x07
#define ARROW_DOWN        0x08
#define ARROW_LEFT        0x09
#define ARROW_RIGHT       0x0A
#define BATTERY_0         0x0B  // 0%
#define BATTERY_25        0x0C  // 25%
#define BATTERY_50        0x0D  // 50%
#define BATTERY_75        0x0E  // 75%
#define BATTERY_100       0x0F  // 100%
#define BATTERY_CHARGING  0x10  // Charging symbol
#define WIFI_0            0x11  // No signal
#define WIFI_1            0x12  // 1 bar
#define WIFI_2            0x13  // 2 bars
#define WIFI_3            0x14  // 3 bars
#define WIFI_4            0x15  // 4 bars (full)
#define CLOCK             0x16  // Clock symbol
#define BELL              0x17  // Notification bell
#define LOCK              0x18  // Lock symbol
#define UNLOCK            0x19  // Unlock symbol
#define SETTINGS          0x1A  // Gear/settings symbol

// Font size definitions
#define FONT_SMALL  0
#define FONT_MEDIUM 1
#define FONT_LARGE  2
#define FONT_XLARGE  3


#define FALSE    0
#define TRUE    1


/**************************************************************************/
                           /* Utility Functions */
/**************************************************************************/

/**
 * @brief Initialize the TFT display
 * @note This function must be called before any other display functions
 * @example TFT_Init();
 */
void TFT_Init(void);



/**
 * @brief Invert the colors of the entire display
 * @param invert TRUE to invert colors, FALSE to restore normal colors
 * @example TFT_InvertColors(TRUE);  // Invert entire display
 * @example TFT_InvertColors(FALSE); // Restore normal colors
 */
void TFT_InvertColors(uint8_t invert);

/**
 * @brief Clear the entire screen with black color
 * @example TFT_ClearScreen();
 */
void TFT_ClearScreen(void);

/**
 * @brief Fill the entire screen with specified color
 * @param color 16-bit color value (RGB565 format)
 * @example TFT_FillScreen(TFT_RED);
 * @example TFT_FillScreen(0xF800); // Red
 */
void TFT_FillScreen(uint16_t color);

/**
 * @brief Draw a 16-bit RGB565 background image
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param background_image Pointer to 16-bit background image data (RGB565 format)
 * @param w Width of background image
 * @param h Height of background image
 * @example TFT_BackgroundImage(0, 0, thumbbig_1358785, 240, 320);
 */
void TFT_BackgroundImage(uint16_t x, uint16_t y, const uint16_t *background_image, uint16_t w, uint16_t h);

/**
 * @brief Draw an icon with transparent background (black/0x0000 is transparent)
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param icon Pointer to icon data (RGB565 format)
 * @param w Width of icon
 * @param h Height of icon
 * @example
 * // Draw icon with black pixels as transparent
 * TFT_AddIcon(50, 50, my_icon, 32, 32);
 */
void TFT_AddIcon(uint16_t x, uint16_t y, const uint16_t *icon, uint16_t w, uint16_t h);




/***********************************************************************/
                       /* Symbol Drawing Functions */
/***********************************************************************/
/**
 * @brief Draw a symbol with scaling support
 * @param x X coordinate
 * @param y Y coordinate
 * @param symbol Symbol code
 * @param color Symbol color
 * @param size Size multiplier (1 = 8x8, 2 = 16x16, etc.)
 */
void TFT_DrawSymbol(uint16_t x, uint16_t y, uint8_t symbol, uint16_t color, uint8_t size);

/**
 * @brief Draw a digital clock
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param hours Hours (0-23)
 * @param minutes Minutes (0-59)
 * @param seconds Seconds (0-59, optional)
 * @param color Text color
 * @param bg_color Background color
 * @param font_scale Font scale factor
 * @param show_seconds TRUE to show seconds, FALSE to hide
 */
void TFT_DrawClock(uint16_t x, uint16_t y, uint8_t hours, uint8_t minutes, uint8_t seconds,
                  uint16_t color, uint16_t bg_color, uint8_t font_scale, uint8_t show_seconds);

/**
 * @brief Draw an analog clock (simplified)
 * @param x Center X coordinate
 * @param y Center Y coordinate
 * @param radius Clock radius
 * @param hours Hours (0-23)
 * @param minutes Minutes (0-59)
 * @param seconds Seconds (0-59)
 * @param color Clock color
 * @param bg_color Background color
 */
void TFT_DrawAnalogClock(uint16_t x, uint16_t y, uint16_t radius,
                        uint8_t hours, uint8_t minutes, uint8_t seconds,
                        uint16_t color, uint16_t bg_color);

/**
 * @brief Draw a string with embedded symbols using default small font
 * @param x X coordinate
 * @param y Y coordinate
 * @param str String with embedded symbol codes (use \x01 for smiley, etc.)
 * @param color Text color
 * @param bg Background color
 * @param symbol_size Size of symbols
 * @example char text[] = {SMILEY_FACE, ' ', 'H', 'e', 'l', 'l', 'o', ' ', HEART, 0};
 * @example TFT_DrawStringWithSymbols(50, 50, text, TFT_WHITE, TFT_BLACK, 2);
 */
void TFT_DrawStringWithSymbols(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t symbol_size);

/**
 * @brief Draw a string with embedded symbols and specified font size
 * @param x X coordinate
 * @param y Y coordinate
 * @param str String with embedded symbol codes
 * @param color Text color
 * @param bg Background color
 * @param symbol_size Size of symbols
 * @param font_size Font size for text characters
 * @example char text[] = {SMILEY_FACE, ' ', 'H', 'e', 'l', 'l', 'o', 0};
 * @example TFT_DrawStringWithSymbolsSize(50, 50, text, TFT_WHITE, TFT_BLACK, 2, FONT_MEDIUM);
 */
void TFT_DrawStringWithSymbolsSize(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t symbol_size, uint8_t font_size);



/*************************************************************************/
                       /* Shape Drawing Functions */
/*************************************************************************/
/**
 * @brief Draw a rectangle outline
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Width of rectangle
 * @param h Height of rectangle
 * @param color Border color
 * @example TFT_DrawRect(10, 10, 100, 50, TFT_WHITE);
 */
void TFT_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief Draw a filled rectangle
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Width of rectangle
 * @param h Height of rectangle
 * @param color Fill color
 * @example TFT_DrawFilledRect(10, 10, 100, 50, TFT_BLUE);
 */
void TFT_DrawFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief Draw a centered rectangle
 * @param w Width of rectangle
 * @param h Height of rectangle
 * @param color Border color
 * @example TFT_DrawRectCentered(200, 100, TFT_WHITE);
 */
void TFT_DrawRectCentered(uint16_t w, uint16_t h, uint16_t color);



/***************************************************************************/
                     /* TEXT WRITING PROTOTYPES */
/***************************************************************************/

/**
 * @brief Calculate scaled text dimensions
 * @param text Text string to measure
 * @param font_scale Font scale factor
 * @param width Pointer to store calculated width
 * @param height Pointer to store calculated height
 */
void TFT_printf(uint16_t x, uint16_t y, uint16_t color, uint16_t bg_color, uint8_t font_scale, const char *format, ...);

/**
 * @brief Calculate text dimensions for given text and font scale
 * @param text Text string to measure
 * @param font_scale Font scale factor
 * @param width Pointer to store calculated width
 * @param height Pointer to store calculated height
 * @example uint16_t width, height;
 *          TFT_CalculateTextSize("Hello", 2, &width, &height);
 */
void TFT_CalculateTextSize(const char *text, uint8_t font_scale, uint16_t *width, uint16_t *height);





/***********************************************************************/
                          /* Rotation Functions */
/***********************************************************************/
/**
 * @brief Set display rotation
 * @param rotation Rotation value (TFT_ROTATION_0, TFT_ROTATION_90, etc.)
 * @example TFT_SetRotation(TFT_ROTATION_90); // Landscape mode
 */
void TFT_SetRotation(uint8_t rotation);




#endif
