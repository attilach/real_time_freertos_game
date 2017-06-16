/*
 * lcd.h
 *
 *  Created on: 27 févr. 2014
 *  Updated on: 28.9.2016
 *  Authors: F. Vannel, V. Pilloux
 *
 *  Note: the RGB screen colors (if coded with 16 bits) have the following format:
 *  blue:  bits 4..0
 *  green: bits 10..5
 *  red:   bits 15..11
 */

#ifndef LCD_H_
#define LCD_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdarg.h>
#include "ssp.h"
#include "fonts.h"

// LCD predefined colors
#define LCD_BLUE  (0xF<<1)
#define LCD_GREEN (0x3f<<5)
#define LCD_RED   (0x1f<<11)
#define LCD_WHITE 0xFFFF
#define LCD_BLACK 0

// screen size
#define LCD_MAX_HEIGHT	(320)
#define LCD_MAX_WIDTH 	(240)


#define DISPLAY_ON()           Write_Instruction(0xaf)   //  Display on
#define DISPLAY_OFF()          Write_Instruction(0xae)   //  Display off
#define SET_ADC()              Write_Instruction(0xa1)   //  Reverse disrect (SEG128-SEG0)
#define CLEAR_ADC()            Write_Instruction(0xa0)   //  Normal disrect (SEG0-SEG128)
#define REVERSE_DISPLAY_ON()   Write_Instruction(0xa7)   //  Reverse display : 0 illuminated
#define REVERSE_DISPLAY_OFF()  Write_Instruction(0xa6)   //  Normal display : 1 illuminated
#define ENTIRE_DISPLAY_ON()    Write_Instruction(0xa5)   //  Entire display   Force whole LCD point
#define ENTIRE_DISPLAY_OFF()   Write_Instruction(0xa4)   //  Normal display
#define SET_BIAS()             Write_Instruction(0xa3)   //  bias 1   1/7 bias
#define CLEAR_BIAS()           Write_Instruction(0xa2)   //  bias 0   1/9 bias
#define SET_MODIFY_READ()      Write_Instruction(0xe0)   //  Stop automatic increment of the column address by the read instruction
#define RESET_MODIFY_READ()    Write_Instruction(0xee)   //  Cancel Modify_read, column address return to its initial value just before the Set Modify Read instruction is started
#define RESET()                Write_Instruction(0xe2)
#define SET_SHL()              Write_Instruction(0xc8)   // SHL 1,COM33-COM0
#define CLEAR_SHL()            Write_Instruction(0xc0)   // SHL 0,COM0-COM33

#define LCD_CS_PIN  16
#define LCD_RS_PORT 1 		//  D/C    DATA/COMMAND SELECT
#define LCD_RS_PIN  30

// sck, mosi, miso is setup within ssp_init
// LCD_RS = D/Cx line. Must be 0 for a command, 1 for data.
// WARNING: LCD_CS_ENA() is called by lcd_init() only. If another peripheral uses SSP, LCD_CS_DIS() must be called.
//          If the screen is required again, LCD_CS_ENA() must be called again!
#define LCD_CS_ENA() {LPC_GPIO0->FIOCLR=(1<<LCD_CS_PIN);}
#define LCD_CS_DIS() { while ((LPC_SSP0->SR & SSPSR_BSY)); \
	                   LPC_GPIO0->FIOSET=(1<<LCD_CS_PIN);}


/* Description: LCD initialisation. Must be called before any other function. */
void init_lcd(void);

/* Description: fill the screen with the defined color in RGB format (see above)
 * Parameter: color: RGB 16 bits color (see format description above)
 */
void clear_screen(unsigned int color);

/* Description: print text (with printf equivalent formatting) at the specified position and color
 * Parameters: x: horizontal position of the text to print
 *             y: vertical position of the text to print
 *             font_size: BIGFONT or SMALLFONT
 *             color: RGB 16 bits color of the text
 *             back_color: RGB 16 bits color of the text backround
 *             st: string in "printf format" followed by any parameter
 * Return: string length [pixels]
 */
uint16_t lcd_print(int x, int y, int font_size, int color, int back_color, char *st, ...);

/* Description: scroll the screen vertically
 * Parameter: offset: scroll length [pixels]
 */
void lcd_scroll(int offset);

/* Description: print one character at any position
 * Parameter: c: character to print
 *            x: horizontal position of the character to print
 *            y: vertical position of the character to print
 *            color:RGB 16 bits color of the text
 *            font_size: BIGFONT or SMALLFONT
 */
void lcd_print_char(char c, int x, int y, int color, int back_color, int font_size);

/* Description: draw an empty circle
 * Parameter: x_centre: horizontal position of the center of the circle
 *            y_centre: vertical position of the center of the circle
 *            r: circle radius
 *            color:RGB 16 bits color of the text
 */
void lcd_circle(int x_centre, int y_centre, int r, unsigned int color);

/* Description: draw a filled circle
 * Parameter: x_centre: horizontal position of the center of the circle
 *            y_centre: vertical position of the center of the circle
 *            r: circle radius
 *            color:RGB 16 bits color of the circle
 */
void lcd_filled_circle(int x_centre, int y_centre, int r, unsigned int color);

/* Description: draw a filled circle
 * Parameter: x_centre: horizontal position of the center of the circle
 *            y_centre: vertical position of the center of the circle
 *            r: circle radius
 *            color:RGB 16 bits foreground color of the circle
 *            background_color: background color of the square around the circle
 */
void lcd_filled_circle_on_square(int x_centre, int y_centre, int r,
		                         unsigned int color, unsigned int background_color);

/* Description: draw a line
 * Parameter: x1: horizontal position of one line extremity
 *            y1: vertical position of one line extremity
 *            x2: horizontal position of the second line extremity
 *            y2: vertical position of the second line extremity
 *            color:RGB 16 bits color of the text
 */
void lcd_line(int x1, int y1, int x2, int y2, unsigned int color);

/* Description: draw an horizontal line
 * Parameter: x1: left horizontal position of one the line
 *            y1: vertical position of the line
 *            l: line length
 *            color:RGB 16 bits color of the text
 */
void lcd_lineH(int x, int y, int l, unsigned int color) ;

/* Description: draw an vertical line
 * Parameter: x1: left horizontal position of one the line
 *            y1: vertical position of the line
 *            l: line length
 *            color:RGB 16 bits color of the text
 */
void lcd_lineV(int x, int y, int l, unsigned int color);

/* Description: draw an empty rectangle
 * Parameter: x1: horizontal position of the top left corner
 *            y1: vertical position of the top left corner
 *            x2: horizontal position of the bottom right corner
 *            y2: vertical position of the bottom right corner
 *            color:RGB 16 bits color of the text
 */
void lcd_empty_rectangle(int x1, int y1, int x2, int y2, int color);

/* Description: draw plain rectangle
 * Parameter: x1: horizontal position of the top left corner
 *            y1: vertical position of the top left corner
 *            x2: horizontal position of the bottom right corner
 *            y2: vertical position of the bottom right corner
 *            color:RGB 16 bits color of the text
 */
void lcd_filled_rectangle(int x1, int y1, int x2, int y2, int color);

/* Description: read an image in a file of BMP format, adapts it to the LCD format and
 *              put it in memory. The number of pixel of the image width must be a multiple of 4.
 * Parameter: filename: filename of a BMP file
 *
 * Return: width:  image width [pixels]
 *         height: image height [pixels]
 *         return value: pointer on the image in memory or NULL if an error occurred
 */
uint16_t *read_bmp_file(char *filename, uint16_t *width, uint16_t *height);

/* Description: display an image stored in memory
 * Parameter: bitmap: pointer on the image in memory. Each word of 16 bits represents the
 *                    color of one pixel. The pixel are stored from left to right line by
 *                    line from top to bottom.
 *            x: horizontal position of the top left corner
 *            y: vertical position of the top left corner
 *            width: image width
 *            height: image height
 */
void display_bitmap16(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/* Description: get pixel colors in a rectangle (16  bits format)
 * Parameter: x1: horizontal position of the top left corner
 *            y1: vertical position of the top left corner
 *            x2: horizontal position of the bottom right corner
 *            y2: vertical position of the bottom right corner
 *            out: pointer on the 16 bits colors of the pixels in the rectangle
 */
void read_pixels(uint8_t x1, int16_t y1, uint8_t x2, int16_t y2, uint16_t *out);

// macro utilities
#define swap(type, i, j) {type t = i; i = j; j = t;}
#define MAX(x,y) (((x)>(y))?(x):(y))
#define MIN(x,y) (((x)<(y))?(x):(y))
#define ABS(x)  (((x)<0)?-(x):(x))


#endif /* LCD_H_ */
