#include "tft_display.h"
#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern SPI_HandleTypeDef hspi1;

// Current rotation state
uint8_t tft_rotation = TFT_ROTATION_0;

// 12x12 font with full ASCII support (32-126)
// Each character is defined by 12 uint16_t values (12 rows)
// Each uint16_t represents 12 pixels (bits) for that row
static const uint16_t font12x12[95][12] = {
    // Space (32)
    {0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},
    // ! (33)
	{0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0x000, 0xC00, 0x000, 0x000, 0x000}, //shifted
    // " (34)
    {0x0CC, 0x0CC, 0x0CC, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},
    // # (35)
    {0x0CC, 0x0CC, 0x3FF, 0x0CC, 0x0CC, 0x3FF, 0x0CC, 0x0CC, 0x000, 0x000, 0x000, 0x000},
    // $ (36)
    {0x030, 0x0FC, 0x1B6, 0x1B0, 0x0FC, 0x036, 0x1B6, 0x0FC, 0x030, 0x000, 0x000, 0x000},
    // % (37)
    {0x183, 0x0C6, 0x06C, 0x038, 0x1C0, 0x370, 0x2D8, 0x18C, 0x306, 0x000, 0x000, 0x000},
    // & (38)
    {0x078, 0x0CC, 0x0CC, 0x078, 0x1DE, 0x0CC, 0x0CC, 0x1FE, 0x073, 0x000, 0x000, 0x000},
    // ' (39)
    {0x030, 0x030, 0x060, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},
    // ( (40)
    {0x018, 0x030, 0x060, 0x060, 0x060, 0x060, 0x060, 0x030, 0x018, 0x000, 0x000, 0x000},
    // ) (41)
    {0x060, 0x030, 0x018, 0x018, 0x018, 0x018, 0x018, 0x030, 0x060, 0x000, 0x000, 0x000},
    // * (42)
    {0x000, 0x030, 0x1FE, 0x0FC, 0x030, 0x078, 0x0CC, 0x000, 0x000, 0x000, 0x000, 0x000},
    // + (43)
    {0x000, 0x030, 0x030, 0x030, 0x1FE, 0x030, 0x030, 0x030, 0x000, 0x000, 0x000, 0x000},
    // , (44)
    {0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x030, 0x030, 0x060, 0x000, 0x000},
    // - (45)
    {0x000, 0x000, 0x000, 0x000, 0x1FE, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},
    // . (46)
    {0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x030, 0x030, 0x000, 0x000, 0x000},
    // / (47)
    {0x003, 0x006, 0x00C, 0x018, 0x030, 0x060, 0x0C0, 0x180, 0x000, 0x000, 0x000, 0x000},
    // 0 (48)
	{0x3F0, 0x618, 0xC0C, 0xCCC, 0xD8C, 0xF0C, 0xF0C, 0x618, 0x3F0, 0x000, 0x000, 0x000}, //shifted
    // 1 (49)
	{0x180, 0x380, 0x780, 0x180, 0x180, 0x180, 0x180, 0x180, 0xFF0, 0x000, 0x000, 0x000}, // shifted
    // 2 (50)
    {0x0FC, 0x186, 0x303, 0x006, 0x00C, 0x030, 0x0C0, 0x180, 0x3FF, 0x000, 0x000, 0x000},
    // 3 (51)
	{0x3F0, 0x618, 0xC0C, 0x018, 0x070, 0x018, 0xC0C, 0x618, 0x3F0, 0x000, 0x000, 0x000}, // shifted
    // 4 (52)
	{0x00C, 0x01C, 0x03C, 0x06C, 0x0CC, 0x18C, 0x3FF, 0x00C, 0x00C, 0x000, 0x000, 0x000},
    // 5 (53)
    {0x3FF, 0x300, 0x300, 0x3FC, 0x306, 0x003, 0x303, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // 6 (54)
    {0x0FC, 0x186, 0x300, 0x300, 0x3FC, 0x306, 0x303, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // 7 (55)
    {0x3FF, 0x003, 0x006, 0x00C, 0x018, 0x030, 0x060, 0x0C0, 0x0C0, 0x000, 0x000, 0x000}, // shifted
    // 8 (56)
    {0x0FC, 0x186, 0x303, 0x186, 0x0FC, 0x186, 0x303, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // 9 (57)
	{0x3F0, 0x618, 0xC0C, 0x61C, 0x3FC, 0x00C, 0x00C, 0x618, 0x3F0, 0x000, 0x000, 0x000}, //shifted
    // : (58)
	{0x000, 0x000, 0xC00, 0xC00, 0x000, 0x000, 0xC00, 0xC00, 0x000, 0x000, 0x000, 0x000}, // shifted
    // ; (59)
    {0x000, 0x000, 0x030, 0x030, 0x000, 0x000, 0x030, 0x030, 0x060, 0x000, 0x000, 0x000},
    // < (60)
    {0x006, 0x00C, 0x018, 0x030, 0x060, 0x030, 0x018, 0x00C, 0x006, 0x000, 0x000, 0x000},
    // = (61)
    {0x000, 0x000, 0x1FE, 0x000, 0x000, 0x1FE, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},
    // > (62)
    {0x060, 0x030, 0x018, 0x00C, 0x006, 0x00C, 0x018, 0x030, 0x060, 0x000, 0x000, 0x000},
    // ? (63)
    {0x0FC, 0x186, 0x303, 0x006, 0x00C, 0x018, 0x000, 0x018, 0x018, 0x000, 0x000, 0x000},
    // @ (64)
    {0x0FC, 0x186, 0x333, 0x36B, 0x36B, 0x36B, 0x37E, 0x300, 0x186, 0x0FC, 0x000, 0x000},
    // A (65)
    {0x030, 0x078, 0x0CC, 0x0CC, 0x186, 0x1FE, 0x306, 0x306, 0x306, 0x000, 0x000, 0x000},
    // B (66)
    {0x3FC, 0x306, 0x306, 0x306, 0x3FC, 0x306, 0x306, 0x306, 0x3FC, 0x000, 0x000, 0x000},
    // C (67)
    {0x0FC, 0x186, 0x300, 0x300, 0x300, 0x300, 0x300, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // D (68)
    {0x3F8, 0x30C, 0x306, 0x306, 0x306, 0x306, 0x306, 0x30C, 0x3F8, 0x000, 0x000, 0x000},
    // E (69)
    {0x3FF, 0x300, 0x300, 0x300, 0x3FC, 0x300, 0x300, 0x300, 0x3FF, 0x000, 0x000, 0x000},
    // F (70)
    {0x3FF, 0x300, 0x300, 0x300, 0x3FC, 0x300, 0x300, 0x300, 0x300, 0x000, 0x000, 0x000},
    // G (71)
    {0x0FC, 0x186, 0x300, 0x300, 0x33F, 0x303, 0x303, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // H (72)
    {0x306, 0x306, 0x306, 0x306, 0x3FF, 0x306, 0x306, 0x306, 0x306, 0x000, 0x000, 0x000},
    // I (73)
    {0x1FE, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x1FE, 0x000, 0x000, 0x000},
    // J (74)
	{0x078, 0x018, 0x018, 0x018, 0x018, 0xC18, 0xC18, 0x618, 0x3F0, 0x000, 0x000, 0x000},  //shifted
    // K (75)
    {0x306, 0x30C, 0x318, 0x330, 0x3C0, 0x330, 0x318, 0x30C, 0x306, 0x000, 0x000, 0x000},
    // L (76)
    {0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x3FF, 0x000, 0x000, 0x000},
    // M (77)
    {0x306, 0x38E, 0x3FE, 0x376, 0x366, 0x306, 0x306, 0x306, 0x306, 0x000, 0x000, 0x000},
    // N (78)
    {0x306, 0x386, 0x3C6, 0x366, 0x336, 0x31E, 0x30E, 0x306, 0x306, 0x000, 0x000, 0x000},
    // O (79)
    {0x0FC, 0x186, 0x303, 0x303, 0x303, 0x303, 0x303, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // P (80)
    {0x3FC, 0x306, 0x306, 0x306, 0x3FC, 0x300, 0x300, 0x300, 0x300, 0x000, 0x000, 0x000},
    // Q (81)
    {0x0FC, 0x186, 0x303, 0x303, 0x303, 0x333, 0x31B, 0x186, 0x0FF, 0x003, 0x000, 0x000},
    // R (82)
    {0x3FC, 0x306, 0x306, 0x306, 0x3FC, 0x318, 0x30C, 0x306, 0x303, 0x000, 0x000, 0x000},
    // S (83)
    {0x0FC, 0x186, 0x300, 0x300, 0x0FC, 0x006, 0x006, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // T (84)
    {0x1FE, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x000, 0x000, 0x000},
    // U (85)
    {0x306, 0x306, 0x306, 0x306, 0x306, 0x306, 0x306, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // V (86)
    {0x306, 0x306, 0x306, 0x186, 0x186, 0x0CC, 0x0CC, 0x078, 0x030, 0x000, 0x000, 0x000},
    // W (87)
    {0x306, 0x306, 0x306, 0x306, 0x366, 0x366, 0x3FE, 0x18C, 0x18C, 0x000, 0x000, 0x000},
    // X (88)
    {0x306, 0x306, 0x18C, 0x0CC, 0x078, 0x078, 0x0CC, 0x18C, 0x306, 0x000, 0x000, 0x000},
    // Y (89)
    {0x186, 0x186, 0x186, 0x0CC, 0x078, 0x030, 0x030, 0x030, 0x030, 0x000, 0x000, 0x000},
    // Z (90)
    {0x1FE, 0x006, 0x00C, 0x018, 0x030, 0x060, 0x0C0, 0x180, 0x1FE, 0x000, 0x000, 0x000},
    // [ (91)
    {0x07E, 0x060, 0x060, 0x060, 0x060, 0x060, 0x060, 0x060, 0x07E, 0x000, 0x000, 0x000},
    // \ (92)
    {0x180, 0x0C0, 0x060, 0x030, 0x018, 0x00C, 0x006, 0x003, 0x001, 0x000, 0x000, 0x000},
    // ] (93)
    {0x07E, 0x006, 0x006, 0x006, 0x006, 0x006, 0x006, 0x006, 0x07E, 0x000, 0x000, 0x000},
    // ^ (94)
    {0x030, 0x078, 0x0CC, 0x186, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},
    // _ (95)
    {0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x3FF, 0x000, 0x000},
    // ` (96)
    {0x030, 0x030, 0x018, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},
    // a (97)
    {0x000, 0x000, 0x000, 0x0FC, 0x006, 0x0FE, 0x186, 0x186, 0x0FE, 0x000, 0x000, 0x000},
    // b (98)
    {0x300, 0x300, 0x300, 0x3FC, 0x306, 0x306, 0x306, 0x306, 0x3FC, 0x000, 0x000, 0x000},
    // c (99)
    {0x000, 0x000, 0x000, 0x0FC, 0x186, 0x300, 0x300, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // d (100)
    {0x006, 0x006, 0x006, 0x0FE, 0x186, 0x306, 0x306, 0x186, 0x0FE, 0x000, 0x000, 0x000},
    // e (101)
    {0x000, 0x000, 0x000, 0x0FC, 0x186, 0x3FF, 0x300, 0x186, 0x0FC, 0x000, 0x000, 0x000},
    // f (102)
	{0x1E0, 0x330, 0x300, 0xFC0, 0x300, 0x300, 0x300, 0x300, 0x300, 0x000, 0x000, 0x000},  //shifted
    // g (103)
    {0x000, 0x000, 0x000, 0x0FE, 0x186, 0x306, 0x306, 0x186, 0x0FE, 0x006, 0x186, 0x0FC},
    // h (104)
    {0x300, 0x300, 0x300, 0x3FC, 0x306, 0x306, 0x306, 0x306, 0x306, 0x000, 0x000, 0x000},
    // i (105)
    {0x030, 0x000, 0x000, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x000, 0x000, 0x000},
    // j (106)
	{0x0C0, 0x000, 0x000, 0x0C0, 0x0C0, 0x0C0, 0x0C0, 0x0C0, 0x0C0, 0x0C0, 0xCC0, 0x780}, // shifted
    // k (107)
    {0x300, 0x300, 0x300, 0x30C, 0x318, 0x330, 0x3E0, 0x318, 0x30C, 0x000, 0x000, 0x000},
    // l (108)
	{0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0x000, 0x000, 0x000}, // shifted
    // m (109)
    {0x000, 0x000, 0x000, 0x3FC, 0x366, 0x366, 0x366, 0x366, 0x366, 0x000, 0x000, 0x000},
    // n (110)
    {0x000, 0x000, 0x000, 0x3FC, 0x306, 0x306, 0x306, 0x306, 0x306, 0x000, 0x000, 0x000},
    // o (111)
	{0x000, 0x000, 0x000, 0x3F0, 0x618, 0xC18, 0xC18, 0x618, 0x3F0, 0x000, 0x000, 0x000}, // shifted
    // p (112)
    {0x000, 0x000, 0x000, 0x3FC, 0x306, 0x306, 0x306, 0x306, 0x3FC, 0x300, 0x300, 0x300},
    // q (113)
    {0x000, 0x000, 0x000, 0x0FE, 0x186, 0x306, 0x306, 0x186, 0x0FE, 0x006, 0x006, 0x006},
    // r (114)
    {0x000, 0x000, 0x000, 0x1BC, 0x066, 0x060, 0x060, 0x060, 0x060, 0x000, 0x000, 0x000},
    // s (115)
    {0x000, 0x000, 0x000, 0x0FC, 0x180, 0x0FC, 0x006, 0x006, 0x3FC, 0x000, 0x000, 0x000},
    // t (116)
    {0x060, 0x060, 0x060, 0x1F8, 0x060, 0x060, 0x060, 0x066, 0x03C, 0x000, 0x000, 0x000},
    // u (117)
    {0x000, 0x000, 0x000, 0x306, 0x306, 0x306, 0x306, 0x306, 0x1FE, 0x000, 0x000, 0x000},
    // v (118)
    {0x000, 0x000, 0x000, 0x306, 0x306, 0x186, 0x186, 0x0CC, 0x078, 0x000, 0x000, 0x000},
    // w (119)
    {0x000, 0x000, 0x000, 0x366, 0x366, 0x366, 0x366, 0x366, 0x1FE, 0x000, 0x000, 0x000},
    // x (120)
    {0x000, 0x000, 0x000, 0x306, 0x186, 0x0CC, 0x0CC, 0x186, 0x306, 0x000, 0x000, 0x000},
    // y (121)
	{0x00, 0x00, 0x00, 0xC60, 0xC60, 0xC60, 0xC60, 0x860, 0x7E0, 0x060, 0x860, 0x7C0}, // shifted
    // z (122)
    {0x000, 0x000, 0x000, 0x3FF, 0x006, 0x00C, 0x030, 0x060, 0x3FF, 0x000, 0x000, 0x000},
    // { (123)
    {0x00E, 0x018, 0x018, 0x018, 0x070, 0x018, 0x018, 0x018, 0x00E, 0x000, 0x000, 0x000},
    // | (124)
    {0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x030, 0x000, 0x000, 0x000},
    // } (125)
    {0x070, 0x018, 0x018, 0x018, 0x00E, 0x018, 0x018, 0x018, 0x070, 0x000, 0x000, 0x000},
    // ~ (126)
    {0x000, 0x000, 0x000, 0x000, 0x1CE, 0x3B8, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000}
};

// Character width table (pixels each character occupies including spacing)
// Character width table - defines how much horizontal space each character occupies
// Values are in pixels and include optimal spacing for professional typography
static const uint8_t char_widths[95] = {
    4,  // Space (32)
    4,  // ! (33)
    6,  // " (34)
    8,  // # (35)
    8,  // $ (36)
    10, // % (37)
    9,  // & (38)
    3,  // ' (39)
    5,  // ( (40)
    5,  // ) (41)
    7,  // * (42)
    8,  // + (43)
    4,  // , (44)
    6,  // - (45)
    4,  // . (46)
    6,  // / (47)
    12,  // 0 (48)
    9,  // 1 (49)
    12,  // 2 (50)
    10,  // 3 (51)
    12,  // 4 (52)
    12,  // 5 (53)
    12,  // 6 (54)
    12,  // 7 (55)
    12,  // 8 (56)
    12,  // 9 (57)
    4,  // : (58)
    4,  // ; (59)
    7,  // < (60)
    8,  // = (61)
    7,  // > (62)
    8,  // ? (63)
    10, // @ (64)
    9,  // A (65)
    9,  // B (66)
    9,  // C (67)
    9,  // D (68)
    8,  // E (69)
    8,  // F (70)
    9,  // G (71)
    9,  // H (72)
    5,  // I (73)
    11,  // J (74)
    9,  // K (75)
    8,  // L (76)
    11, // M (77)
    9,  // N (78)
    9,  // O (79)
    8,  // P (80)
    9,  // Q (81)
    9,  // R (82)
    8,  // S (83)
    8,  // T (84)
    9,  // U (85)
    9,  // V (86)
    11, // W (87)
    9,  // X (88)
    9,  // Y (89)
    8,  // Z (90)
    5,  // [ (91)
    6,  // \ (92)
    5,  // ] (93)
    7,  // ^ (94)
    10, // _ (95)
    5,  // ` (96)
    8,  // a (97)
    8,  // b (98)
    7,  // c (99)
    8,  // d (100)
    8,  // e (101)
    9,  // f (102)
    8,  // g (103)
    8,  // h (104)
    4,  // i (105)
    8,  // j (106)
    8,  // k (107)
    4,  // l (108)
    11, // m (109)
    8,  // n (110)
    11,  // o (111)
    8,  // p (112)
    8,  // q (113)
    6,  // r (114)
    7,  // s (115)
    6,  // t (116)
    8,  // u (117)
    8,  // v (118)
    11, // w (119)
    8,  // x (120)
    9,  // y (121)
    7,  // z (122)
    6,  // { (123)
    3,  // | (124)
    6,  // } (125)
    9   // ~ (126)
};

// Symbol and emoji bitmaps (8x8 pixels)
const uint8_t symbol_bitmaps[][8] = {
    // Smiley face (SMILEY_FACE)
    {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C},
    // Heart (HEART)
    {0x0C, 0x1E, 0x3E, 0x7C, 0x7C, 0x3E, 0x1E, 0x0C},
    // Star (STAR)
    {0x08, 0x14, 0x22, 0x41, 0x41, 0x22, 0x14, 0x08},
    // Music note (MUSIC_NOTE)
    {0x02, 0x03, 0x02, 0x02, 0x0E, 0x1E, 0x0C, 0x00},
    // Check mark (CHECK_MARK)
    {0x00, 0x01, 0x02, 0x04, 0x88, 0x50, 0x20, 0x00},
    // Cross mark (CROSS_MARK)
    {0x00, 0x41, 0x22, 0x14, 0x08, 0x14, 0x22, 0x41},
    // Arrow up (ARROW_UP)
    {0x08, 0x1C, 0x3E, 0x7F, 0x08, 0x08, 0x08, 0x08},
    // Arrow down (ARROW_DOWN)
    {0x08, 0x08, 0x08, 0x08, 0x7F, 0x3E, 0x1C, 0x08},
    // Arrow left (ARROW_LEFT)
    {0x08, 0x0C, 0x0E, 0xFF, 0xFF, 0x0E, 0x0C, 0x08},
    // Arrow right (ARROW_RIGHT)
    {0x10, 0x30, 0x70, 0xFF, 0xFF, 0x70, 0x30, 0x10},
    // Battery 0% (BATTERY_0)
    {0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F},
    // Battery 25% (BATTERY_25)
    {0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F},
    // Battery 50% (BATTERY_50)
    {0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F},
    // Battery 75% (BATTERY_75)
    {0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F},
    // Battery 100% (BATTERY_100)
    {0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F},
    // Charging (CHARGING)
	{0x06, 0x0E, 0x1E, 0xFF, 0xFF, 0x78, 0x70, 0x60},
    // WiFi 0 bars (WIFI_0)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // WiFi 1 bar (WIFI_1)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00},
    // WiFi 2 bars (WIFI_2)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x08},
    // WiFi 3 bars (WIFI_3)
    {0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x3E, 0x1C},
    // WiFi 4 bars (WIFI_4)
    {0x00, 0x00, 0x00, 0x08, 0x1C, 0x3E, 0x7F, 0x3E},
    // Clock (CLOCK)
    {0x1C, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00},
    // Bell (BELL)
    {0x18, 0x3C, 0x3C, 0x3C, 0x7E, 0x00, 0x18, 0x00},
    // Lock (LOCK)
    {0x1C, 0x22, 0x22, 0x7F, 0x63, 0x63, 0x63, 0x7F},
    // Unlock (UNLOCK)
    {0x1C, 0x22, 0x20, 0x7F, 0x63, 0x63, 0x63, 0x7F},
    // Settings gear (SETTINGS)
    {0x18, 0x24, 0x42, 0x99, 0x99, 0x42, 0x24, 0x18}
};



/***************************************************************************/
                     /* FUNCTIONS PROTOTYPES */
/***************************************************************************/
/**
 * @brief Draw a scaled character with font size support
 * @param x X coordinate
 * @param y Y coordinate
 * @param c Character to draw
 * @param color Text color
 * @param bg Background color (0x0000 for transparent)
 * @param scale Font scale factor (1 = 5x7, 2 = 10x14, etc.)
 */
static void TFT_DrawCharScaled(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t scale);

/**
 * @brief Set address window for drawing operations
 * @param x0 Start X coordinate
 * @param y0 Start Y coordinate
 * @param x1 End X coordinate
 * @param y1 End Y coordinate
 * @note This is used internally by other functions
 */
void TFT_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * @brief Draw a single pixel at specified coordinates
 * @param x X coordinate (0 to width-1)
 * @param y Y coordinate (0 to height-1)
 * @param color 16-bit color value
 * @example TFT_DrawPixel(100, 50, TFT_BLUE);
 */
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Get current rotation
 * @return Current rotation value
 * @example uint8_t rot = TFT_GetRotation();
 */
uint8_t TFT_GetRotation(void);

/**
 * @brief Rotate to 90 degrees (convenience function)
 * @example TFT_Rotate90();
 */
void TFT_Rotate90(void);

/**
 * @brief Rotate to 180 degrees (convenience function)
 * @example TFT_Rotate180();
 */
void TFT_Rotate180(void);

/**
 * @brief Rotate to 270 degrees (convenience function)
 * @example TFT_Rotate270();
 */
void TFT_Rotate270(void);

/**
 * @brief Rotate to 0 degrees (convenience function)
 * @example TFT_Rotate0();
 */
void TFT_Rotate0(void);

/**
 * @brief Get current display width based on rotation
 * @return Current width in pixels
 * @example uint16_t width = TFT_GetWidth();
 */
uint16_t TFT_GetWidth(void);

/**
 * @brief Get current display height based on rotation
 * @return Current height in pixels
 * @example uint16_t height = TFT_GetHeight();
 */
uint16_t TFT_GetHeight(void);



/**************************************************************************/
                           /* Utility Functions */
/**************************************************************************/
void TFT_Init(void) {
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);

    TFT_Reset();

    TFT_WriteCommand(TFT_SWRESET);
    HAL_Delay(150);

    TFT_WriteCommand(TFT_SLPOUT);
    HAL_Delay(120);

    TFT_WriteCommand(TFT_COLMOD);
    TFT_WriteData(0x55);

    TFT_SetRotation(TFT_ROTATION_0);

    TFT_WriteCommand(TFT_PWCTR1);
    TFT_WriteData(0x23);

    TFT_WriteCommand(TFT_PWCTR2);
    TFT_WriteData(0x10);

    TFT_WriteCommand(TFT_VMCTR1);
    TFT_WriteData(0x3E);
    TFT_WriteData(0x28);

    TFT_WriteCommand(TFT_VMCTR2);
    TFT_WriteData(0x86);

    TFT_WriteCommand(TFT_INVON);
    TFT_WriteCommand(TFT_DISPON);
    HAL_Delay(120);
}

// Hardware-specific functions (implement these according to your setup)
void TFT_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

void TFT_WriteData(uint8_t data) {
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &data, 1, 100);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

void TFT_WriteData16(uint16_t data) {
    uint8_t buffer[2];
    buffer[0] = data >> 8;
    buffer[1] = data & 0xFF;

    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, buffer, 2, 100);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

void TFT_Reset(void) {
    HAL_GPIO_WritePin(TFT_RST_PORT, TFT_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(TFT_RST_PORT, TFT_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
}

void TFT_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	TFT_WriteCommand(TFT_CASET);
    TFT_WriteData16(x0);
    TFT_WriteData16(x1);

    TFT_WriteCommand(TFT_RASET);
    TFT_WriteData16(y0);
    TFT_WriteData16(y1);

    TFT_WriteCommand(TFT_RAMWR);
}


void TFT_InvertColors(uint8_t invert) {
    if (invert) {
        TFT_WriteCommand(TFT_INVON);  // Display inversion on
    } else {
        TFT_WriteCommand(TFT_INVOFF); // Display inversion off
    }
}

void TFT_FillScreen(uint16_t color) {
    uint8_t buffer[2];
    buffer[0] = color >> 8;
    buffer[1] = color & 0xFF;

    TFT_SetAddressWindow(0, 0, TFT_GetWidth() - 1, TFT_GetHeight() - 1);

    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);

    for(uint32_t i = 0; i < TFT_GetWidth() * TFT_GetHeight(); i++) {
        HAL_SPI_Transmit(&hspi1, buffer, 2, 1);
    }

    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief Draw a single pixel at specified coordinates with optimized performance
 * @param x X coordinate (0 to width-1)
 * @param y Y coordinate (0 to height-1)
 * @param color 16-bit color value (RGB565 format)
 * @note Includes boundary checking and optimized SPI transmission
 * @example TFT_DrawPixel(100, 150, TFT_RED);
 */
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    // Boundary checking - prevent drawing outside display area
    if (x >= TFT_GetWidth() || y >= TFT_GetHeight()) {
        return; // Pixel outside display boundaries
    }

    // Set address window to single pixel
    TFT_WriteCommand(TFT_CASET); // Column address set
    TFT_WriteData16(x);
    TFT_WriteData16(x);

    TFT_WriteCommand(TFT_RASET); // Row address set
    TFT_WriteData16(y);
    TFT_WriteData16(y);

    TFT_WriteCommand(TFT_RAMWR); // Memory write

    // Send pixel data with optimized SPI transmission
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET); // Data mode
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET); // CS low

    uint8_t color_data[2] = {color >> 8, color & 0xFF};
    HAL_SPI_Transmit(&hspi1, color_data, 2, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET); // CS high
}

void TFT_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg) {
    while(*str) {
        TFT_DrawChar(x, y, *str++, color, bg);
        x += 6;
    }
}



void TFT_DrawSymbol(uint16_t x, uint16_t y, uint8_t symbol, uint16_t color, uint8_t size) {
    if(symbol < 1 || symbol > 26) return; // Updated range check

    const uint8_t *bitmap = symbol_bitmaps[symbol - 1];

    // Special handling for battery levels with bars
    if(symbol >= BATTERY_0 && symbol <= BATTERY_100) {
        TFT_DrawBatteryWithBars(x, y, symbol, color, size);
        return;
    }

    // Special handling for WiFi signals with bars
    if(symbol >= WIFI_0 && symbol <= WIFI_4) {
        TFT_DrawWifiWithBars(x, y, symbol, color, size);
        return;
    }

    // Standard symbol drawing
    for(uint8_t row = 0; row < 8; row++) {
        for(uint8_t col = 0; col < 8; col++) {
            if(bitmap[row] & (1 << (7 - col))) {
                if(size == 1) {
                    TFT_DrawPixel(x + col, y + row, color);
                } else {
                    TFT_DrawFilledRect(x + col * size, y + row * size, size, size, color);
                }
            }
        }
    }
}

// Helper function for battery with bars
void TFT_DrawBatteryWithBars(uint16_t x, uint16_t y, uint8_t battery_level, uint16_t color, uint8_t size) {
    // Draw battery outline
    uint16_t battery_width = 8 * size;
    uint16_t battery_height = 4 * size;
    uint16_t tip_width = 1 * size;
    uint16_t tip_height = 2 * size;

    // Battery tip
    TFT_DrawFilledRect(x + battery_width, y + size, tip_width, tip_height, color);

    // Battery body
    TFT_DrawRect(x, y, battery_width, battery_height, color);

    // Calculate fill level based on battery level
    uint8_t fill_bars = 0;
    switch(battery_level) {
        case BATTERY_0:    fill_bars = 0; break;
        case BATTERY_25:   fill_bars = 1; break;
        case BATTERY_50:   fill_bars = 2; break;
        case BATTERY_75:   fill_bars = 3; break;
        case BATTERY_100:  fill_bars = 4; break;
        default:           fill_bars = 0; break;
    }

    // Draw fill bars
    if(fill_bars > 0) {
        uint16_t bar_width = (battery_width - 2 * size) / 4;
        uint16_t bar_spacing = 1 * size;
        uint16_t fill_height = battery_height - 2 * size;

        for(uint8_t i = 0; i < fill_bars; i++) {
            TFT_DrawFilledRect(x + size + i * (bar_width + bar_spacing),
                              y + size,
                              bar_width,
                              fill_height,
                              color);
        }
    }
}

// Helper function for WiFi with bars
void TFT_DrawWifiWithBars(uint16_t x, uint16_t y, uint8_t wifi_strength, uint16_t color, uint8_t size) {
    uint8_t bars = wifi_strength - WIFI_0;

    // Draw WiFi arcs based on signal strength
    if(bars >= 1) {
        // Outer arc (largest)
        TFT_DrawArc(x + 4*size, y + 4*size, 4*size, 225, 315, color);
    }
    if(bars >= 2) {
        // Middle arc
        TFT_DrawArc(x + 4*size, y + 4*size, 3*size, 225, 315, color);
    }
    if(bars >= 3) {
        // Inner arc
        TFT_DrawArc(x + 4*size, y + 4*size, 2*size, 225, 315, color);
    }
    if(bars >= 4) {
        // Center dot
        TFT_DrawFilledRect(x + 3*size, y + 3*size, 2*size, 2*size, color);
    }
}

// Simple arc drawing function for WiFi
void TFT_DrawArc(uint16_t x, uint16_t y, uint16_t radius, uint16_t start_angle, uint16_t end_angle, uint16_t color) {
    // Simplified arc drawing - in real implementation, use proper arc algorithm
    for(uint16_t angle = start_angle; angle <= end_angle; angle += 5) {
        float rad = angle * 3.14159 / 180;
        uint16_t px = x + radius * cos(rad);
        uint16_t py = y + radius * sin(rad);
        TFT_DrawPixel(px, py, color);
    }
}

// Clock drawing functions
void TFT_DrawClock(uint16_t x, uint16_t y, uint8_t hours, uint8_t minutes, uint8_t seconds,
                  uint16_t color, uint16_t bg_color, uint8_t font_scale, uint8_t show_seconds) {
    char time_str[9]; // HH:MM:SS or HH:MM

    if(show_seconds) {
        sprintf(time_str, "%02d:%02d:%02d", hours, minutes, seconds);
    } else {
        sprintf(time_str, "%02d:%02d", hours, minutes);
    }

    TFT_DrawString(x, y, time_str, color, bg_color);
}

void TFT_DrawAnalogClock(uint16_t x, uint16_t y, uint16_t radius,
                        uint8_t hours, uint8_t minutes, uint8_t seconds,
                        uint16_t color, uint16_t bg_color) {
    // Draw clock face
    TFT_DrawCircle(x, y, radius, color);
    TFT_DrawCircle(x, y, radius - 1, color);

    // Draw hour markers
    for(int i = 0; i < 12; i++) {
        float angle = i * 30 * 3.14159 / 180;
        uint16_t x1 = x + (radius - 5) * cos(angle);
        uint16_t y1 = y + (radius - 5) * sin(angle);
        uint16_t x2 = x + radius * cos(angle);
        uint16_t y2 = y + radius * sin(angle);
        TFT_DrawLine(x1, y1, x2, y2, color);
    }

    // Calculate hand angles
    float hour_angle = (hours % 12 + minutes / 60.0) * 30 * 3.14159 / 180;
    float minute_angle = minutes * 6 * 3.14159 / 180;
    float second_angle = seconds * 6 * 3.14159 / 180;

    // Draw hour hand
    TFT_DrawLine(x, y, x + radius * 0.5 * cos(hour_angle), y + radius * 0.5 * sin(hour_angle), color);

    // Draw minute hand
    TFT_DrawLine(x, y, x + radius * 0.7 * cos(minute_angle), y + radius * 0.7 * sin(minute_angle), color);

    // Draw second hand
    TFT_DrawLine(x, y, x + radius * 0.9 * cos(second_angle), y + radius * 0.9 * sin(second_angle), TFT_RED);

    // Draw center dot
    TFT_DrawFilledCircle(x, y, 3, color);
}

void TFT_DrawStringWithSymbols(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t symbol_size) {
    while(*str) {
        if((uint8_t)*str < 32) {
            TFT_DrawSymbol(x, y, (uint8_t)*str, color, symbol_size);
            x += 8 * symbol_size;
        } else {
            TFT_DrawChar(x, y, *str, color, bg);
            x += 6;
        }
        str++;
    }
}

void TFT_DrawStringCentered(uint16_t y, const char *str, uint16_t color, uint16_t bg) {
    uint16_t width = TFT_GetWidth();
    uint16_t text_width = strlen(str) * 6;
    uint16_t x = (width - text_width) / 2;
    TFT_DrawString(x, y, str, color, bg);
}

void TFT_DrawStringCenteredSize(uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t font_size) {
    uint16_t width = TFT_GetWidth();
    uint8_t spacing = TFT_GetCharSpacing(font_size);
    uint16_t text_width = strlen(str) * spacing;
    uint16_t x = (width - text_width) / 2;
    TFT_DrawStringSize(x, y, str, color, bg, font_size);
}

void TFT_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for(uint16_t i = x; i < x + w; i++) {
        TFT_DrawPixel(i, y, color);
        TFT_DrawPixel(i, y + h - 1, color);
    }
    for(uint16_t i = y; i < y + h; i++) {
        TFT_DrawPixel(x, i, color);
        TFT_DrawPixel(x + w - 1, i, color);
    }
}

void TFT_DrawFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // Boundary checking and clipping
    if (x >= TFT_GetWidth() || y >= TFT_GetHeight()) return;

    // Clip rectangle to screen boundaries
    uint16_t x_end = x + w;
    uint16_t y_end = y + h;

    if (x_end > TFT_GetWidth()) x_end = TFT_GetWidth();
    if (y_end > TFT_GetHeight()) y_end = TFT_GetHeight();

    w = x_end - x;
    h = y_end - y;

    if (w == 0 || h == 0) return;

    // Set address window for the entire rectangle
    TFT_WriteCommand(TFT_CASET); // Column address set
    TFT_WriteData16(x);
    TFT_WriteData16(x + w - 1);

    TFT_WriteCommand(TFT_RASET); // Row address set
    TFT_WriteData16(y);
    TFT_WriteData16(y + h - 1);

    TFT_WriteCommand(TFT_RAMWR); // Memory write

    // Send all pixel data at once
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET); // Data mode
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET); // CS low

    uint32_t total_pixels = w * h;
    uint8_t color_high = color >> 8;
    uint8_t color_low = color & 0xFF;

    // Send pixel data in chunks to avoid large buffer allocation
    for(uint32_t i = 0; i < total_pixels; i++) {
        HAL_SPI_Transmit(&hspi1, &color_high, 1, HAL_MAX_DELAY);
        HAL_SPI_Transmit(&hspi1, &color_low, 1, HAL_MAX_DELAY);
    }

    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET); // CS high
}

void TFT_DrawRectCentered(uint16_t w, uint16_t h, uint16_t color) {
    uint16_t screen_width = TFT_GetWidth();
    uint16_t screen_height = TFT_GetHeight();
    uint16_t x = (screen_width - w) / 2;
    uint16_t y = (screen_height - h) / 2;
    TFT_DrawRect(x, y, w, h, color);
}


void TFT_SetRotation(uint8_t rotation) {
    tft_rotation = rotation;
    TFT_WriteCommand(TFT_MADCTL);
    TFT_WriteData(rotation);
}

uint8_t TFT_GetRotation(void) {
    return tft_rotation;
}

void TFT_Rotate90(void) {
    TFT_SetRotation(TFT_ROTATION_90);
}

void TFT_Rotate180(void) {
    TFT_SetRotation(TFT_ROTATION_180);
}

void TFT_Rotate270(void) {
    TFT_SetRotation(TFT_ROTATION_270);
}

void TFT_Rotate0(void) {
    TFT_SetRotation(TFT_ROTATION_0);
}

uint16_t TFT_GetWidth(void) {
    switch(tft_rotation) {
        case TFT_ROTATION_0:
        case TFT_ROTATION_180:
            return TFT_WIDTH_LANDSCAPE;
        case TFT_ROTATION_90:
        case TFT_ROTATION_270:
        default:
            return TFT_WIDTH_PORTRAIT;
    }
}

uint16_t TFT_GetHeight(void) {
    switch(tft_rotation) {
        case TFT_ROTATION_0:
        case TFT_ROTATION_180:
            return TFT_HEIGHT_LANDSCAPE;
        case TFT_ROTATION_90:
        case TFT_ROTATION_270:
        default:
            return TFT_HEIGHT_PORTRAIT;
    }
}

void TFT_DrawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    int32_t f = 1 - radius;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * radius;
    int32_t x0 = 0;
    int32_t y0 = radius;

    TFT_DrawPixel(x, y + radius, color);
    TFT_DrawPixel(x, y - radius, color);
    TFT_DrawPixel(x + radius, y, color);
    TFT_DrawPixel(x - radius, y, color);

    for (int32_t i = 0; i < radius; i++) {
        TFT_DrawPixel(x + i, y + radius, color);
        TFT_DrawPixel(x - i, y + radius, color);
        TFT_DrawPixel(x + i, y - radius, color);
        TFT_DrawPixel(x - i, y - radius, color);
        TFT_DrawPixel(x + radius, y + i, color);
        TFT_DrawPixel(x - radius, y + i, color);
        TFT_DrawPixel(x + radius, y - i, color);
        TFT_DrawPixel(x - radius, y - i, color);
    }

    while (x0 < y0) {
        if (f >= 0) {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;

        for (int32_t i = x - x0; i <= x + x0; i++) {
            TFT_DrawPixel(i, y + y0, color);
            TFT_DrawPixel(i, y - y0, color);
        }
        for (int32_t i = x - y0; i <= x + y0; i++) {
            TFT_DrawPixel(i, y + x0, color);
            TFT_DrawPixel(i, y - x0, color);
        }
    }
}

void TFT_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    int32_t f = 1 - radius;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * radius;
    int32_t x0 = 0;
    int32_t y0 = radius;

    TFT_DrawPixel(x, y + radius, color);
    TFT_DrawPixel(x, y - radius, color);
    TFT_DrawPixel(x + radius, y, color);
    TFT_DrawPixel(x - radius, y, color);

    while (x0 < y0) {
        if (f >= 0) {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;

        TFT_DrawPixel(x + x0, y + y0, color);
        TFT_DrawPixel(x - x0, y + y0, color);
        TFT_DrawPixel(x + x0, y - y0, color);
        TFT_DrawPixel(x - x0, y - y0, color);
        TFT_DrawPixel(x + y0, y + x0, color);
        TFT_DrawPixel(x - y0, y + x0, color);
        TFT_DrawPixel(x + y0, y - x0, color);
        TFT_DrawPixel(x - y0, y - x0, color);
    }
}



/***************************************************************************/
                     /* TEXT WRITING FUNCTIONS */
/***************************************************************************/

/**
 * @brief Print formatted text to the TFT display (printf-like functionality)
 * @param x X coordinate for text starting position
 * @param y Y coordinate for text starting position
 * @param color Text color (RGB565)
 * @param bg_color Background color (RGB565 or TRANSPARENT)
 * @param font_scale Font scale factor (1 = 12x12, 2 = 24x24, etc.)
 * @param format Format string with printf-style specifiers
 * @param ... Variable arguments for formatting
 *
 * This function works like printf but displays text on the TFT display.
 * It supports all printf format specifiers and handles both transparent
 * and solid backgrounds with variable character spacing.
 *
 * @example:
 * TFT_printf(10, 10, TFT_WHITE, TRANSPARENT, 2, "Score: %d", score);
 * TFT_printf(50, 50, TFT_RED, TFT_BLACK, 1, "Temp: %.1f°C", temperature);
 */
void TFT_printf(uint16_t x, uint16_t y, uint16_t color, uint16_t bg_color, uint8_t font_scale, const char *format, ...) {
    char buffer[128];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (bg_color != TRANSPARENT) {
        uint16_t text_width, text_height;
        TFT_CalculateTextSize(buffer, font_scale, &text_width, &text_height);
        TFT_DrawFilledRect(x, y, text_width, text_height, bg_color);
    }

    uint16_t current_x = x;
    const char *current_char = buffer;

    while (*current_char) {
        if (*current_char >= ASCII_START && *current_char <= ASCII_END) {
            uint8_t char_index = *current_char - ASCII_START;
            TFT_DrawCharScaled(current_x, y, *current_char, color, bg_color, font_scale);
            current_x += char_widths[char_index] * font_scale;
        } else {
            current_x += 4 * font_scale; // Skip unknown chars
        }
        current_char++;
    }
}

/**
 * @brief Calculate the dimensions of text for background drawing
 * @param text Text string to measure
 * @param font_scale Font scale factor
 * @param width Pointer to store calculated width (output)
 * @param height Pointer to store calculated height (output)
 *
 * This function calculates the total width and height needed to display
 * the given text, considering variable character widths and scaling.
 *
 * @example:
 * uint16_t text_width, text_height;
 * TFT_CalculateTextSize("Hello", 2, &text_width, &text_height);
 * // text_width = total width in pixels, text_height = 24 pixels
 */
void TFT_CalculateTextSize(const char *text, uint8_t font_scale, uint16_t *width, uint16_t *height) {
    *width = 0;
    const char *current_char = text;

    while (*current_char) {
        if (*current_char >= ASCII_START && *current_char <= ASCII_END) {
            uint8_t char_index = *current_char - ASCII_START;
            *width += char_widths[char_index] * font_scale;
        } else {
            *width += 4 * font_scale; // Default width for unknown chars
        }
        current_char++;
    }

    *height = 12 * font_scale;
}

/**
 * @brief Draw a scaled character with variable width spacing
 * @param x X coordinate for character drawing
 * @param y Y coordinate for character drawing
 * @param c Character to draw (ASCII value)
 * @param color Text color (RGB565)
 * @param bg_color Background color (RGB565 or TRANSPARENT)
 * @param scale Font scale factor (1 = 12x12, 2 = 24x24, etc.)
 *
 * This function draws a single character from the 12x12 font array.
 * It handles both transparent and solid backgrounds.
 *
 * @example TFT_DrawCharScaled(100, 100, 'A', TFT_WHITE, TRANSPARENT, 2);
 */
static void TFT_DrawCharScaled(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color, uint8_t scale) {
    // Only handle printable ASCII characters (32-126)
    if (c < ASCII_START || c > ASCII_END) {
        // Unsupported character - draw as space
        if (bg_color != TRANSPARENT) {
            TFT_DrawFilledRect(x, y, 12 * scale, 12 * scale, bg_color);
        }
        return;
    }

    uint8_t char_index = c - ASCII_START;

    for (uint8_t row = 0; row < 12; row++) {
        uint16_t line = font12x12[char_index][row];
        for (uint8_t col = 0; col < 12; col++) {
            // Read bits from MSB to LSB (left to right)
            if (line & (1 << (11 - col))) {
                // Draw scaled pixel for the character
                if (scale == 1) {
                    TFT_DrawPixel(x + col, y + row, color);
                } else {
                    TFT_DrawFilledRect(x + col * scale, y + row * scale, scale, scale, color);
                }
            } else if (bg_color != TRANSPARENT) {
                // Draw scaled background pixel
                if (scale == 1) {
                    TFT_DrawPixel(x + col, y + row, bg_color);
                } else {
                    TFT_DrawFilledRect(x + col * scale, y + row * scale, scale, scale, bg_color);
                }
            }
        }
    }
}

void TFT_BackgroundImage(uint16_t x, uint16_t y, const uint16_t *background_image, uint16_t w, uint16_t h) {
    TFT_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);

    for(uint32_t i = 0; i < w * h; i++) {
        uint16_t color = background_image[i];
        uint8_t buffer[2] = {color >> 8, color & 0xFF};
        HAL_SPI_Transmit(&hspi1, buffer, 2, 1);
    }

    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

// Add this function to your tft_display.c

/**
 * @brief Draw an icon with black pixels (0x0000) treated as transparent
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param icon Pointer to icon data (RGB565 format)
 * @param w Width of icon
 * @param h Height of icon
 */
void TFT_AddIcon(uint16_t x, uint16_t y, const uint16_t *icon, uint16_t w, uint16_t h) {
    for(uint16_t row = 0; row < h; row++) {
        uint16_t start_col = 0;
        uint16_t current_col = 0;

        while(current_col < w) {
            // Find the start of a non-transparent segment (skip black pixels)
            while(current_col < w && icon[row * w + current_col] == 0x0000) {
                current_col++;
            }
            start_col = current_col;

            // Find the end of this non-transparent segment
            while(current_col < w && icon[row * w + current_col] != 0x0000) {
                current_col++;
            }

            // Draw the non-transparent segment
            if(start_col < current_col) {
                uint16_t segment_width = current_col - start_col;

                // Set address window for this segment
                TFT_SetAddressWindow(x + start_col, y + row, x + start_col + segment_width - 1, y + row);

                HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);

                // Send all pixels in this segment
                for(uint16_t seg_col = start_col; seg_col < current_col; seg_col++) {
                    uint16_t color = icon[row * w + seg_col];
                    uint8_t buffer[2] = {color >> 8, color & 0xFF};
                    HAL_SPI_Transmit(&hspi1, buffer, 2, 1);
                }

                HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
            }
        }
    }
}
