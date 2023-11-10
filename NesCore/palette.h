#ifndef PALETTE_H
#define PALETTE_H

#include <QtGlobal>

// http://wiki.nesdev.com/w/index.php/PPU_palettes
const quint8 RGBColorMap[64][3] = {
    { 0x7F, 0x7F, 0x7F}, { 0x20, 0x00, 0xB0}, { 0x28, 0x00, 0xB8}, { 0x60, 0x10, 0xA0},
    { 0x98, 0x20, 0x78}, { 0xB0, 0x10, 0x30}, { 0xA0, 0x30, 0x00}, { 0x78, 0x40, 0x00},
    { 0x48, 0x58, 0x00}, { 0x38, 0x68, 0x00}, { 0x38, 0x6C, 0x00}, { 0x30, 0x60, 0x40},
    { 0x30, 0x50, 0x80}, { 0x00, 0x00, 0x00}, { 0x00, 0x00, 0x00}, { 0x00, 0x00, 0x00},

    { 0xBC, 0xBC, 0xBC}, { 0x40, 0x60, 0xF8}, { 0x40, 0x40, 0xFF}, { 0x90, 0x40, 0xF0},
    { 0xD8, 0x40, 0xC0}, { 0xD8, 0x40, 0x60}, { 0xE0, 0x50, 0x00}, { 0xC0, 0x70, 0x00},
    { 0x88, 0x88, 0x00}, { 0x50, 0xA0, 0x00}, { 0x48, 0xA8, 0x10}, { 0x48, 0xA0, 0x68},
    { 0x40, 0x90, 0xC0}, { 0x00, 0x00, 0x00}, { 0x00, 0x00, 0x00}, { 0x00, 0x00, 0x00},

    { 0xFF, 0xFF, 0xFF}, { 0x60, 0xA0, 0xFF}, { 0x50, 0x80, 0xFF}, { 0xA0, 0x70, 0xFF},
    { 0xF0, 0x60, 0xFF}, { 0xFF, 0x60, 0xB0}, { 0xFF, 0x78, 0x30}, { 0xFF, 0xA0, 0x00},
    { 0xE8, 0xD0, 0x20}, { 0x98, 0xE8, 0x00}, { 0x70, 0xF0, 0x40}, { 0x70, 0xE0, 0x90},
    { 0x60, 0xD0, 0xE0}, { 0x60, 0x60, 0x60}, { 0x00, 0x00, 0x00}, { 0x00, 0x00, 0x00},

    { 0xFF, 0xFF, 0xFF}, { 0x90, 0xD0, 0xFF}, { 0xA0, 0xB8, 0xFF}, { 0xC0, 0xB0, 0xFF},
    { 0xE0, 0xB0, 0xFF}, { 0xFF, 0xB8, 0xE8}, { 0xFF, 0xC8, 0xB8}, { 0xFF, 0xD8, 0xA0},
    { 0xFF, 0xF0, 0x90}, { 0xC8, 0xF0, 0x80}, { 0xA0, 0xF0, 0xA0}, { 0xA0, 0xFF, 0xC8},
    { 0xA0, 0xFF, 0xF0}, { 0xA0, 0xA0, 0xA0}, { 0x00, 0x00, 0x00}, { 0x00, 0x00, 0x00}
};
#endif // PALETTE_H
