//////////////////////////////////////////////////////////////////////////////////////////////
// Code to use SSD1306-based OLED displays on TIVA board
//
// Concepts borrowed from
//   https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.cpp
//   Attribution/License copied below
//
// Converted to non-OO (plain C) for ECE 251
//
// Stephen Undy Fall 2024
//
//////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * @file Adafruit_SSD1306.cpp
 *
 * @mainpage Arduino library for monochrome OLEDs based on SSD1306 drivers.
 *
 * @section intro_sec Introduction
 *
 * This is documentation for Adafruit's SSD1306 library for monochrome
 * OLED displays: http://www.adafruit.com/category/63_98
 *
 * These displays use I2C or SPI to communicate. I2C requires 2 pins
 * (SCL+SDA) and optionally a RESET pin. SPI requires 4 pins (MOSI, SCK,
 * select, data/command) and optionally a reset pin. Hardware SPI or
 * 'bitbang' software SPI are both supported.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a
 * href="https://github.com/adafruit/Adafruit-GFX-Library"> Adafruit_GFX</a>
 * being present on your system. Please make sure you have installed the latest
 * version before using this library.
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.
 *
 * @section license License
 *
 * BSD license, all text above, and the splash screen included below,
 * must be included in any redistribution.
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////

#include "OLED_lib.h"
#include "ascii.h"

// Global vars
uint8_t *OLED_buffer;
uint8_t OLED_addr;
uint8_t OLED_width;
uint8_t OLED_height;
uint8_t OLED_contrast;
uint8_t OLED_rotation;
uint8_t OLED_xpos; // "cursor" position
uint8_t OLED_ypos; // "cursor" position

///////////////////////////////////////////////////////////////
// INTERNAL: initialize I2C unit 0 (and GPIOB)
//
void initI2C0(uint32_t speed) {
    // start clocks
    __IO *addr = (uint32_t *) RCGCGPIO;
    *addr |= 0x2; // enable GPIOB clocks

    addr = (uint32_t *) RCGCI2C;
    *addr |= 0x1; // enable I2C0 clocks

    // GPIOB setup
    GPIOB->dir &= 0xfffffff3; // PB2 and PB3 are input
    GPIOB->amsel &= 0xfffffff3; // PB2 and PB3 not analog
    ; // FIXME: digital enable for PB2 and PB3
    GPIOB->den |= 0x0C;
    GPIOB->odr = (GPIOB->odr & 0xfffffffb) | 0x8; // PB2 is not open-drain, PB3 is
    ; // FIXEME: alt function select for PB2 and PB3
    GPIOB->afsel |= 0x0C;
    GPIOB->pctrl = (GPIOB->pctrl & 0xffff00ff) | 0x3300; // alt function = I2C for PB2 and PB3

    // I2C0 setup
    ; // FIXME: master mode
    I2C0->mcr = 0x10;
    uint32_t tpr = ((16000000 / (20 * speed)) - 1) & 0xFF; // 16Mhz system clock
    uint32_t tpr100KHz = ((16000000 / (20 * 100000)) - 1) & 0xFF; //100KHz clock Question 1
    I2C0->mtpr = tpr100KHz;
}

///////////////////////////////////////////////////////////////
// INTERNAL: Send multiple commands to the screen
//
__attribute__((optimize("O0")))
void ssd1306_commandList(uint8_t prefix, uint8_t *cmds, uint16_t size) {
    if (size == 0)
        return;                     // skip if no data

    uint32_t addr = OLED_addr << 1;
    I2C0->msa = addr;               // slave address + write
    I2C0->mdr = prefix;             // data (Co = 0, D/C = 0)
    I2C0->mcs = 0x03;               // START + TRANSMIT
    while (I2C0->mcs & 0x1);        // spin while busy

    while (size > 0) {
        uint8_t dat = *cmds;
        I2C0->mdr = (uint32_t) dat; // data
        size--;
        cmds++;
        if (size > 0)
            I2C0->mcs = 0x01;       // TRANSMIT
        else
            I2C0->mcs = 0x05;       // TRANSMIT + STOP
        while (I2C0->mcs & 0x1);    // spin while busy
    }
}

///////////////////////////////////////////////////////////////
// INTERNAL: Send a single command to the screen
//
__attribute__((optimize("O0")))
void ssd1306_command(uint8_t prefix, uint8_t cmd) {
    uint32_t addr = OLED_addr << 1;
    I2C0->msa = addr;           // slave address + write
    I2C0->mdr = prefix;         // data (Co = 0, D/C = 0)
    I2C0->mcs = 0x03;           // START + TRANSMIT
    while (I2C0->mcs & 0x1);    // spin while busy

    I2C0->mdr = (uint32_t) cmd; // data
    I2C0->mcs = 0x05;           // TRANSMIT + STOP
    while (I2C0->mcs & 0x1);    // spin while busy
}

///////////////////////////////////////////////////////////////
// INTERNAL: allocate frame buffer at end of SRAM based on display size
//
uint8_t *allocateBuffer() {
    uint32_t size = OLED_width * (OLED_height + 7) / 8;
    return (uint8_t *) (OLED_BUFFER_END - size);
}

///////////////////////////////////////////////////////////////
/*!
    @brief  clears the frame buffer
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_clear() {
    for (uint8_t *ptr = OLED_buffer; ptr < (uint8_t *) OLED_BUFFER_END; ptr++)
        *ptr = 0;
}

///////////////////////////////////////////////////////////////
/*!
    @brief  fill the frame buffer with the specified color
    @param  color
            Fill color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_fill(uint16_t color) {
    for (uint8_t *ptr = OLED_buffer; ptr < (uint8_t *) OLED_BUFFER_END; ptr++)
        switch (color) {
            case SSD1306_BLACK:
                *ptr = 0x0;
                break;
            case SSD1306_WHITE:
                *ptr = 0xff;
                break;
            case SSD1306_INVERSE:
                *ptr = ~(*ptr);
                break;
            default:
                *ptr = 0x0;
        }
}

///////////////////////////////////////////////////////////////
/*!
    @brief Initializes I2C SSD1306 display using I2C Unit 0
    @param w
            Display width in pixels
    @param h
            Display height in pixels
    @param rot
            Rotation
    @param speed
            I2C clock speed
    @param addr
            I2C address of SSD1306
*/

void OLED_Init(uint8_t w, uint8_t h, uint8_t rot, uint8_t addr) {
    OLED_width = w;
    OLED_height = h;
    OLED_rotation = rot;
    OLED_addr = addr;
    OLED_buffer = allocateBuffer();
    OLED_clear();
    initI2C0(400000);

    static uint8_t init1[] = {SSD1306_DISPLAYOFF,               // 0xAE
                                    SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
                                    0x80,   // the suggested ratio 0x80
                                    SSD1306_SETMULTIPLEX};      // 0xA8
    ssd1306_commandList(0x0, init1, sizeof(init1));
    ssd1306_command(0x0, OLED_height - 1);

    static uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET,         // 0xD3
                                    0x0,                        // no offset
                                    SSD1306_SETSTARTLINE | 0x0, // line #0
                                    SSD1306_CHARGEPUMP};        // 0x8D
    ssd1306_commandList(0x0, init2, sizeof(init2));

    ssd1306_command(0x0, 0x14); // Internal VCC

    static uint8_t init3[] = {SSD1306_MEMORYMODE, // 0x20
                                    0x00,         // 0x0 act like ks0108
                                    SSD1306_SEGREMAP | 0x1,
                                    SSD1306_COMSCANDEC};
    ssd1306_commandList(0x0, init3, sizeof(init3));

    uint8_t comPins = 0x02;
    OLED_contrast = 0x8F;
    if ((OLED_width == 128) && (OLED_height == 32)) {
        comPins = 0x02;
        OLED_contrast = 0x8F;
    } else if ((OLED_width == 128) && (OLED_height == 64)) {
        comPins = 0x12;
        OLED_contrast = 0xCF;
    } else if ((OLED_width == 96) && (OLED_height == 16)) {
        comPins = 0x2; // ada x12
        OLED_contrast = 0xAF;
    } else {
        // Other screen varieties -- TBD
    }
    ssd1306_command(0x0, SSD1306_SETCOMPINS);
    ssd1306_command(0x0, comPins);
    ssd1306_command(0x0, SSD1306_SETCONTRAST);
    ssd1306_command(0x0, OLED_contrast);

    ssd1306_command(0x0, SSD1306_SETPRECHARGE); // 0xd9
    ssd1306_command(0x0, 0xF1);
    static uint8_t init5[] = {
                                    SSD1306_SETVCOMDETECT,       // 0xDB
                                    0x40,
                                    SSD1306_DISPLAYALLON_RESUME, // 0xA4
                                    SSD1306_NORMALDISPLAY,       // 0xA6
                                    SSD1306_DEACTIVATE_SCROLL,
                                    SSD1306_DISPLAYON};          // Main screen turn on
    ssd1306_commandList(0x0, init5, sizeof(init5));
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Push data currently in RAM to SSD1306 display.
    @return None (void).
    @note   Drawing operations are not visible until this function is
            called. Call after each graphics command, or after a whole set
            of graphics commands, as best needed by one's own application.
*/
void OLED_display(void) {
    static uint8_t dlist1[] = {
                                    SSD1306_PAGEADDR,
                                    0,                      // Page start address
                                    0xFF,                   // Page end (not really, but works here)
                                    SSD1306_COLUMNADDR, 0}; // Column start address
    ssd1306_commandList(0x0, dlist1, sizeof(dlist1));
    ssd1306_command(0x0, OLED_width - 1); // Column end address

    uint16_t count = OLED_width * ((OLED_height + 7) / 8);

    ssd1306_commandList(0x40, OLED_buffer, count);
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Set/clear/invert a single pixel.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or
            SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x >= 0) && (x < OLED_width) && (y >= 0) && (y < OLED_height)) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (OLED_rotation) {
    case 1:
      ssd1306_swap(x, y);
      x = OLED_width - x - 1;
      break;
    case 2:
      x = OLED_width - x - 1;
      y = OLED_height - y - 1;
      break;
    case 3:
      ssd1306_swap(x, y);
      y = OLED_height - y - 1;
      break;
    }
    switch (color) {
    case SSD1306_WHITE:
      OLED_buffer[x + (y / 8) * OLED_width] |= (1 << (y & 7));
      break;
    case SSD1306_BLACK:
      OLED_buffer[x + (y / 8) * OLED_width] &= ~(1 << (y & 7));
      break;
    case SSD1306_INVERSE:
      OLED_buffer[x + (y / 8) * OLED_width] ^= (1 << (y & 7));
      break;
    }
  }
}

///////////////////////////////////////////////////////////////
// INTERNAL: Used by OLED_drawFastHLine
void OLED_drawFastHLineInternal(int16_t x, int16_t y, int16_t w,
                                             uint16_t color) {

  if ((y >= 0) && (y < OLED_height)) { // Y coord in bounds?
    if (x < 0) {                  // Clip left
      w += x;
      x = 0;
    }
    if ((x + w) > OLED_width) { // Clip right
      w = (OLED_width - x);
    }
    if (w > 0) { // Proceed only if width is positive
      uint8_t *pBuf = &OLED_buffer[(y / 8) * OLED_width + x], mask = 1 << (y & 7);
      switch (color) {
      case SSD1306_WHITE:
        while (w--) {
          *pBuf++ |= mask;
        };
        break;
      case SSD1306_BLACK:
        mask = ~mask;
        while (w--) {
          *pBuf++ &= mask;
        };
        break;
      case SSD1306_INVERSE:
        while (w--) {
          *pBuf++ ^= mask;
        };
        break;
      }
    }
  }
}

///////////////////////////////////////////////////////////////
// INTERNAL: Used by OLED_drawFastVLine
void OLED_drawFastVLineInternal(int16_t x, int16_t __y,
                                int16_t __h, uint16_t color) {

  if ((x >= 0) && (x < OLED_width)) { // X coord in bounds?
    if (__y < 0) {               // Clip top
      __h += __y;
      __y = 0;
    }
    if ((__y + __h) > OLED_height) { // Clip bottom
      __h = (OLED_height - __y);
    }
    if (__h > 0) { // Proceed only if height is now positive
      // this display doesn't need ints for coordinates,
      // use local byte registers for faster juggling
      uint8_t y = __y, h = __h;
      uint8_t *pBuf = &OLED_buffer[(y / 8) * OLED_width + x];

      // do the first partial byte, if necessary - this requires some masking
      uint8_t mod = (y & 7);
      if (mod) {
        // mask off the high n bits we want to set
        mod = 8 - mod;
        // note - lookup table results in a nearly 10% performance
        // improvement in fill* functions
        // uint8_t mask = ~(0xFF >> mod);
        static const uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0,
                                           0xF0, 0xF8, 0xFC, 0xFE};
        uint8_t mask = premask[mod];
        // adjust the mask if we're not going to reach the end of this byte
        if (h < mod)
          mask &= (0XFF >> (mod - h));

        switch (color) {
        case SSD1306_WHITE:
          *pBuf |= mask;
          break;
        case SSD1306_BLACK:
          *pBuf &= ~mask;
          break;
        case SSD1306_INVERSE:
          *pBuf ^= mask;
          break;
        }
        pBuf += OLED_width;
      }

      if (h >= mod) { // More to go?
        h -= mod;
        // Write solid bytes while we can - effectively 8 rows at a time
        if (h >= 8) {
          if (color == SSD1306_INVERSE) {
            // separate copy of the code so we don't impact performance of
            // black/white write version with an extra comparison per loop
            do {
              *pBuf ^= 0xFF; // Invert byte
              pBuf += OLED_width; // Advance pointer 8 rows
              h -= 8;        // Subtract 8 rows from height
            } while (h >= 8);
          } else {
            // store a local value to work with
            uint8_t val = (color != SSD1306_BLACK) ? 255 : 0;
            do {
              *pBuf = val;   // Set byte
              pBuf += OLED_width; // Advance pointer 8 rows
              h -= 8;        // Subtract 8 rows from height
            } while (h >= 8);
          }
        }

        if (h) { // Do the final partial byte, if necessary
          mod = h & 7;
          // this time we want to mask the low bits of the byte,
          // vs the high bits we did above
          // uint8_t mask = (1 << mod) - 1;
          // note - lookup table results in a nearly 10% performance
          // improvement in fill* functions
          static const uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07,
                                              0x0F, 0x1F, 0x3F, 0x7F};
          uint8_t mask = postmask[mod];
          switch (color) {
          case SSD1306_WHITE:
            *pBuf |= mask;
            break;
          case SSD1306_BLACK:
            *pBuf &= ~mask;
            break;
          case SSD1306_INVERSE:
            *pBuf ^= mask;
            break;
          }
        }
      }
    } // endif positive height
  }   // endif x in bounds
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Draw a horizontal line.
    @param  x
            Leftmost column -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @param  w
            Width of line, in pixels.
    @param  color
            Line color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_drawFastHLine(int16_t x, int16_t y, int16_t w,
                                     uint16_t color) {
  bool bSwap = false;
  switch (OLED_rotation) {
  case 1:
    // 90 degree rotation, swap x & y for rotation, then invert x
    bSwap = true;
    ssd1306_swap(x, y);
    x = OLED_width - x - 1;
    break;
  case 2:
    // 180 degree rotation, invert x and y, then shift y around for height.
    x = OLED_width - x - 1;
    y = OLED_height - y - 1;
    x -= (w - 1);
    break;
  case 3:
    // 270 degree rotation, swap x & y for rotation,
    // then invert y and adjust y for w (not to become h)
    bSwap = true;
    ssd1306_swap(x, y);
    y = OLED_height - y - 1;
    y -= (w - 1);
    break;
  }

  if (bSwap)
    OLED_drawFastVLineInternal(x, y, w, color);
  else
    OLED_drawFastHLineInternal(x, y, w, color);
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Draw a vertical line.
    @param  x
            Column of display -- 0 at left to (screen width -1) at right.
    @param  y
            Topmost row -- 0 at top to (screen height - 1) at bottom.
    @param  h
            Height of line, in pixels.
    @param  color
            Line color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_drawFastVLine(int16_t x, int16_t y, int16_t h,
                                     uint16_t color) {
  bool bSwap = false;
  switch (OLED_rotation) {
  case 1:
    // 90 degree rotation, swap x & y for rotation,
    // then invert x and adjust x for h (now to become w)
    bSwap = true;
    ssd1306_swap(x, y);
    x = OLED_width - x - 1;
    x -= (h - 1);
    break;
  case 2:
    // 180 degree rotation, invert x and y, then shift y around for height.
    x = OLED_width - x - 1;
    y = OLED_height - y - 1;
    y -= (h - 1);
    break;
  case 3:
    // 270 degree rotation, swap x & y for rotation, then invert y
    bSwap = true;
    ssd1306_swap(x, y);
    y = OLED_height - y - 1;
    break;
  }

  if (bSwap)
    OLED_drawFastHLineInternal(x, y, h, color);
  else
    OLED_drawFastVLineInternal(x, y, h, color);
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Draws an image aligned to byte address
    @param  w
            Width of image.
    @param  h
            Height of image.
    @param  image
            Pointer to image array.
    @param  invert
            0: no effect, 1: invert image
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_fastImage(uint32_t w, uint32_t h, uint8_t *image, int invert) {
    // center image (but aligned to nearest byte vertically)
    uint16_t start_x = (OLED_width - w) / 2;
    uint16_t start_y = ((OLED_height - h) / 2) & 0xFFF8;

    uint16_t end_y = start_y + h;
    uint16_t end_x = start_x + w;

    for (uint16_t y = start_y; y < end_y; y += 8) {
        for (uint16_t x = start_x; x < end_x; x++) {
            if (invert)
                OLED_buffer[x + (y / 8) * OLED_width] = ~(*image);
            else
                OLED_buffer[x + (y / 8) * OLED_width] = *image;
            image++;
        }
    }
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Sets text cursor to given position
    @param  x
            Column of display -- 0 at left to (screen width -1) at right.
    @param  y
            Row of display -- 0 at top to (screen height - 1) at bottom.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_setCursor(uint8_t x, uint8_t y) {
    OLED_xpos = x;
    OLED_ypos = y;
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Writes 6x8 character at cursor position
    @param  c
            ascii character.
    @param  invert
            0: normal, 1: inverted.
    @return None (void).
    @note   Updates cursor position.
            Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_writeChr(char c, int invert) {
    uint8_t *data = &(Ascii[(c - 0x20) * 5]);
    for (int x=0; x<5; x++) {
        for (int y=0; y<7; y++) {
            if (*data & (1 << y))
                OLED_drawPixel(OLED_xpos + x, OLED_ypos + y, invert ? SSD1306_BLACK : SSD1306_WHITE);
        }
        data++;
    }
    OLED_xpos += 6;
}

///////////////////////////////////////////////////////////////
/*!
    @brief  Writes string at cursor position
    @param  str
            array of non-terminated ascii characters.
    @param  invert
            0: normal, 1: inverted.
    @return None (void).
    @note   Updates cursor position.
            Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void OLED_writeStr(char *str, int invert) {
    while (*str != '\0') {
        OLED_writeChr(*str, invert);
        str++;
    }
}

void OLED_drawRectangle(int16_t x, int16_t y, int16_t w, int16_t h) {
  uint16_t color = SSD1306_WHITE;
  OLED_drawFastHLine(x, y, w, color);             // Top edge
  OLED_drawFastHLine(x, y + h - 1, w, color);     // Bottom edge
  OLED_drawFastVLine(x, y, h, color);             // Left edge
  OLED_drawFastVLine(x + w - 1, y, h, color);     // Right edge
}
