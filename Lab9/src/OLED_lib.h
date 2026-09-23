// Borrowed from https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.h
//

/// fit into the SSD1306_ naming scheme
#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

#define ssd1306_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

// TIVA and ECE251 specific defines:
//
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define int16_t short
#define int32_t int
#define bool unsigned char
#define __IO volatile unsigned int
#define true 1
#define false 0
#define OLED_BUFFER_END 0x20008000

#define GPIO_PORTB_BASE 0x40005000
#define GPIO_DATA  0x3FC
#define GPIO_DIR   0x400
#define GPIO_AFSEL 0x420
#define GPIO_ODR   0x50C
#define GPIO_PUR   0x510
#define GPIO_DEN   0x51C
#define GPIO_PCTL  0x52C
#define GPIO_AMSEL 0x528
#define GPIO_DR2R  0x500

typedef struct {
  char _skip1[1020];
  __IO data;  // 0x3fc
  __IO dir;   // 0x400
  __IO is;    // 0x404
  __IO ibe;   // 0x408
  __IO iev;   // 0x40c
  __IO im;    // 0x410
  __IO ris;   // 0x414
  __IO mis;   // 0x418
  __IO icr;   // 0x41c
  __IO afsel; // 0x420
  char _skip2[220];
  __IO dr2r;  // 0x500
  __IO dr4r;  // 0x504
  __IO dr8r;  // 0x508
  __IO odr;   // 0x50c
  __IO pur;   // 0x510
  __IO pdr;   // 0x514
  __IO slr;   // 0x518
  __IO den;   // 0x51c
  __IO lock;  // 0x520
  __IO cr;    // 0x524
  __IO amsel; // 0x528
  __IO pctrl; // 0x52c
} GPIO_Type;
#define GPIOB ((GPIO_Type *) GPIO_PORTB_BASE)

#define I2C0_BASE   0x40020000
#define I2CMSA      0x000
#define I2CMCS      0x004
#define I2CMDR      0x008
#define I2CMTPR     0x00C
#define I2CMIMR     0x010
#define I2CMRIS     0x014
#define I2CMMIS     0x018
#define I2CMICR     0x01C
#define I2CMCR      0x020
#define I2CMCLKOCNT 0x024
#define I2CMBMON    0x2C
#define I2CMCR2     0x38
typedef struct {
  __IO msa;      // 0x000
  __IO mcs;      // 0x004
  __IO mdr;      // 0x008
  __IO mtpr;     // 0x00c
  __IO mimr;     // 0x010
  __IO mris;     // 0x014
  __IO mmis;     // 0x018
  __IO micr;     // 0x01c
  __IO mcr;      // 0x020
  __IO mclkocnt; // 0x024
  __IO mbmon;    // 0x02c
  char skip1[12];
  __IO mcr2;     // 0x038
} I2C_Type;
#define I2C0 ((I2C_Type *) I2C0_BASE)

#define RCGCGPIO 0x400FE608
#define RCGCI2C  0x400FE620