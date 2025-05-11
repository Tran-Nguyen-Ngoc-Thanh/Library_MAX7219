#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>
#include <stdbool.h>

#define A 'A'
#define B 'B'
#define C 'C'
#define D 'D'
#define E 'E'
#define F 'F'
#define G 'G'
#define H 'H'
#define I 'I'
#define J 'J'
#define K 'K'
#define L 'L'
#define M 'M'
#define N 'N'
#define O 'O'
#define P 'P'
#define Q 'Q'
#define R 'R'
#define S 'S'
#define T 'T'
#define U 'U'
#define V 'V'
#define W 'W'
#define X 'X'
#define Y 'Y'
#define Z 'Z'

#define up 1
#define down 2
#define left 3
#define right 4
#define downleft 5
#define upleft 6
#define upright 7
#define downright 8

extern const uint8_t led_map[10]; 
extern uint8_t character[27][8];
extern uint8_t heart[];
extern uint8_t star[];
extern uint8_t muiten[9][8];
extern uint8_t number[10][8];

// MAX7219 Register Addresses and Modes
#define MAX7219_REG_NOOP        0x00 // No operation
#define MAX7219_REG_DIGIT0      0x01 // Digit 0 data
#define MAX7219_REG_DIGIT1      0x02 // Digit 1 data
#define MAX7219_REG_DIGIT2      0x03 // Digit 2 data
#define MAX7219_REG_DIGIT3      0x04 // Digit 3 data
#define MAX7219_REG_DIGIT4      0x05 // Digit 4 data
#define MAX7219_REG_DIGIT5      0x06 // Digit 5 data
#define MAX7219_REG_DIGIT6      0x07 // Digit 6 data
#define MAX7219_REG_DIGIT7      0x08 // Digit 7 data
#define MAX7219_REG_DECODEMODE  0x09 // Decode mode: 0x00 = No decode, 0xFF = BCD decode for all digits
#define MAX7219_REG_INTENSITY   0x0A // Brightness control: 0x00 (min) - 0x0F (max)
#define MAX7219_REG_SCANLIMIT   0x0B // Display scan limit: 0x00 (Digit 0) - 0x07 (All digits)
#define MAX7219_REG_SHUTDOWN    0x0C // Shutdown mode: 0x00 = Shutdown, 0x01 = Normal operation
#define MAX7219_REG_DISPLAYTEST 0x0F // Display test: 0x00 = Normal mode, 0x01 = Display test mode

void Max7219_send(uint8_t address, uint8_t data);
void Max7219_clear(void);
void Max7219_display_matrix_inclined_direction(float angle_x, float angle_y);
void Max7219_display_clock(int hr, int min, int sec, char *period);

/**
 * @brief  
 * @param  decode_mode (on: 0xFF, off = 0x00 = 0)
 * @param  intensity (0 to 15, corresponding 0% to 100%)
 * @param  scan_limit (1 to 8)
 */
void Max7219_init(uint8_t decode_mode, uint8_t intensity, uint8_t scan_limit);

/**
 * @brief  
 * @param  intensity (0 to 15, corresponding 0% to 100%)
 */
void Max7219_set_intensity(uint8_t intensity);

/**
 * @brief  
 * @param  number (+-Int or +-Float)
 */
void Max7219_display_number(double number);

/**
 * @brief  
 * if display A - Z then hamkytu = 0;
 * if display hinh then chucai = 0
 * @param  chucai Enter A - Z
 * @param  hamkytu Enter the function name your create, available heart, star, muiten
 */
void Max7219_display_matrix(char chucai, uint8_t *hamkytu);

/**
 * @brief  
 * @param  speed don vị là ms
 */
void Max7219_scroll_text(const char *text, int speed);

/**
 * @brief  
 * @param  solan trong giây
 * @param  time thời gian led nháy (s) giây
 */
void Max7219_display_flashing(int solan, int time);

/**
 * @brief  
 * @param  kytu.... mã HEX
 * @param  led_end vị trí led cuối hiển thị
 * @param  number số nguyên hoặc số thực
 * @param  decimal_places số thập phân sau dấu phẩy
 */
void Max7219_display_number_pos(uint8_t kytudau1, uint8_t kytudau2, uint8_t kytucuoi1, uint8_t kytucuoi2, uint8_t led_end, double number, int decimal_places);

#endif // MAX7219_H