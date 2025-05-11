#include "Max7219.h"
#include <stdio.h>
#include <wiringPiSPI.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// define 
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

#define SPI_CHANNEL 0
#define SPI_SPEED   1000000
#define MINUS_SIGN 0x01

// led code
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

const uint8_t led_map[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

uint8_t heart[]={0x66,0xFF,0xFF,0xFF,0x7E,0x3C,0x18,0x00};

uint8_t star[]={0x18,0x18,0xFF,0x7E,0x3C,0x7E,0xE7,0x00};

uint8_t muiten[9][8] = { {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
                         {0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x18},  // mui ten len
                         {0x18,0x18,0x18,0x18,0x18,0x7E,0x3C,0x18},  // mui ten xuong
                         {0x00,0x20,0x60,0xFF,0xFF,0x60,0x20,0x00},  // mui ten qua trai
                         {0x00,0x04,0x06,0xFF,0xFF,0x06,0x04,0x00},  // mui ten qua phai
                         {0x01,0x02,0x04,0x08,0x90,0xe0,0xe0,0xf0}, // ↙ Xuống trái
                         {0xf0,0xe0,0xe0,0x90,0x08,0x04,0x02,0x01}, // ↖ Lên trái
                         {0x0f,0x07,0x07,0x09,0x10,0x20,0x40,0x80}, // ↗ Lên phải
                         {0x80,0x40,0x20,0x10,0x09,0x07,0x07,0x0f}  // ↘ Xuống phải                        
}; 

uint8_t number[10][8] = {{0x3C,0x66,0x66,0x6E,0x76,0x66,0x66,0x3C},//0
                         {0x18,0x38,0x58,0x18,0x18,0x18,0x18,0x7E},//1
                         {0x3C,0x66,0x66,0x0C,0x18,0x30,0x7E,0x7E},//2
                         {0x7E,0x0C,0x18,0x3C,0x06,0x06,0x46,0x3C},//3
                         {0x0C,0x18,0x30,0x6C,0x6C,0x7E,0x0C,0x0C},//4
                         {0x7E,0x60,0x60,0x7C,0x06,0x06,0x46,0x3C},//5
                         {0x04,0x08,0x10,0x38,0x6C,0x66,0x66,0x3C},//6
                         {0x7E,0x46,0x0C,0x18,0x18,0x18,0x18,0x18},//7
                         {0x3C,0x66,0x66,0x3C,0x66,0x66,0x66,0x3C},//8
                         {0x3C,0x66,0x66,0x36,0x1C,0x08,0x10,0x20} //9
};

uint8_t character[27][8] = { {0x00,0x3c,0x42,0x42,0x7e,0x42,0x42,0x42}, // A
                          {0x00,0x7c,0x42,0x42,0x7e,0x42,0x42,0x7c}, // B
                          {0x00,0x3c,0x42,0x40,0x40,0x40,0x42,0x3c}, // C
                          {0x00,0x7c,0x42,0x42,0x42,0x42,0x42,0x7c}, // D
                          {0x00,0x7e,0x40,0x40,0x7c,0x40,0x40,0x7e}, // E
                          {0x00,0x7e,0x40,0x40,0x7c,0x40,0x40,0x40}, // F
                          {0x00,0x3c,0x42,0x40,0x47,0x42,0x42,0x3c}, // G
                          {0x00,0x42,0x42,0x42,0x7e,0x42,0x42,0x42}, // H
                          {0x00,0x7e,0x18,0x18,0x18,0x18,0x18,0x7e}, // I
                          {0x00,0x7e,0x18,0x18,0x18,0x18,0x58,0x30}, // J
                          {0x00,0x44,0x44,0x48,0x70,0x48,0x44,0x42}, // K
                          {0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x7e}, // L
                          {0x00,0x42,0x66,0x5a,0x5a,0x42,0x42,0x42}, // M
                          {0x00,0x42,0x62,0x52,0x4a,0x46,0x42,0x42}, // N
                          {0x00,0x3c,0x42,0x42,0x42,0x42,0x42,0x3c}, // O
                          {0x00,0x7c,0x42,0x42,0x7c,0x40,0x40,0x40}, // P
                          {0x00,0x3c,0x42,0x42,0x42,0x4a,0x46,0x3b}, // Q
                          {0x00,0x7c,0x42,0x42,0x7c,0x48,0x44,0x42}, // R
                          {0x00,0x3c,0x42,0x40,0x3c,0x02,0x42,0x3c}, // S
                          {0x00,0x7e,0x18,0x18,0x18,0x18,0x18,0x18}, // T
                          {0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x3c}, // U
                          {0x00,0x42,0x42,0x42,0x42,0x42,0x24,0x18}, // V
                          {0x00,0x42,0x5a,0x5a,0x5a,0x5a,0x5a,0x3c}, // W
                          {0x00,0x42,0x24,0x18,0x18,0x18,0x24,0x42}, // X
                          {0x00,0x62,0x36,0x1c,0x18,0x30,0x60,0x40}, // Y
                          {0x00,0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e},  // Z
                          {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 
};

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_send(uint8_t address, uint8_t data) {
    uint8_t buffer[2] = {address, data};
    wiringPiSPIDataRW(SPI_CHANNEL, buffer, 2);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_init(uint8_t decode_mode, uint8_t intensity, uint8_t scan_limit ) {
    wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
    scan_limit -= 1;
    
    Max7219_send(MAX7219_REG_SHUTDOWN, 0x01);
    Max7219_send(MAX7219_REG_DISPLAYTEST, 0x00);
    Max7219_send(MAX7219_REG_SCANLIMIT, scan_limit & 0x07);
    Max7219_send(MAX7219_REG_DECODEMODE, decode_mode);
    Max7219_set_intensity(intensity);
    Max7219_clear();
    printf("✅ MAX7219 is ready!\n");

}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_clear(void) {
    for (uint8_t i = MAX7219_REG_DIGIT0; i <= MAX7219_REG_DIGIT7; i++) {
        Max7219_send(i, 0x00);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_set_intensity(uint8_t intensity) {
    Max7219_send(MAX7219_REG_INTENSITY, intensity & 0x0F);
}

void Max7219_display_atoh(void)
{
    for(uint8_t i =0 ; i < 9 ; i++)
    {   
        for(uint8_t y =0 ; y < 8 ; y++)
        {
            Max7219_send(y+1, 0x40>>i);
            if(i == 8) Max7219_send(y+1, 0x80);
        }
        usleep(250000);
    }   
    Max7219_clear();
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_display_number(double number) {
    Max7219_clear();
    double tempglobal = number;
    int isNegative = (number < 0);
    if (isNegative) {
        number = -number; // Chuyển số âm thành dương
    }

    char str[16];
    sprintf(str, "%.10f", number);
    int digit_count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '.') {
            digit_count++;
        }
    }
    
    if((digit_count > 8)&&(tempglobal >0)){
        if((number < 10)){
            number = round(number * 10000000) / 10000000;
        }  // Làm tròn đến 5 chữ số thập phân
        else if(((number >= 10) && (number < 100))){
            number= round(number * 1000000) / 1000000;
        } 
        else if(((number >= 100) && (number < 1000))){
            number = round(number * 100000) / 100000;
        } 
        else if(((number >= 1000) && (number < 10000))){
            number = round(number * 10000) / 10000;
        } 
        else if(((number >= 10000) && (number < 100000))){
            number = round(number * 1000) / 1000;
        } 
        else if(((number >= 100000) && (number < 1000000))){
            number = round(number * 100) / 100;
        }
        else if(((number >= 1000000) && (number < 10000000))){
            number = round(number * 10) / 10;
        }
        else{
            number = round(number);
        }
    } 
    else if((tempglobal < 0)){
        if((number < 10)){
            number = round(number * 1000000) / 1000000;
        }  // Làm tròn đến 5 chữ số thập phân
        else if(((number >= 10) && (number < 100))){
            number= round(number * 100000) / 100000;
        } 
        else if(((number >= 100) && (number < 1000))){
            number = round(number * 10000) / 10000;
        } 
        else if(((number >= 1000) && (number < 10000))){
            number = round(number * 1000) / 1000;
        } 
        else if(((number >= 10000) && (number < 100000))){
            number = round(number * 100) / 100;
        } 
        else if(((number >= 100000) && (number < 1000000))){
            number = round(number * 10) / 10;
        }
        else{
            number = round(number);
        }
    } 
    else number;

    int intPart = (int)number;   // Lấy phần nguyên sau khi làm tròn
    double decPart = number - intPart; // Lấy phần thập phân

    // //Kiểm tra nếu phần nguyên có hơn 8 chữ số thì tắt LED
    
    int temp1 = intPart;
    int intDigitCount;
    if(tempglobal < 0){
        intDigitCount = 1;
    }
    else intDigitCount = 0;

    while (temp1 > 0) {
        temp1 /= 10;
        intDigitCount++;
    }
    if (intDigitCount > 8) {
        for (int i = 1; i <= 8; i++) {
            Max7219_send(i, 0x01); // Tắt tất cả LED
        }
        return;
    }

    int digits[8] = {0}; // Mảng chứa giá trị hiển thị
    int index = 0;
    int decimalPos = -1; // Vị trí đặt dấu chấm thập phân

    // Chuyển phần nguyên vào mảng digits
    int tempInt = intPart;
    do {
        digits[index++] = led_map[tempInt % 10];
        tempInt /= 10;
    } while (tempInt > 0 && index < 8);

    // Nếu là số âm và còn chỗ trống, thêm dấu trừ "-"
    if (isNegative && index < 8) {
        digits[index++] = MINUS_SIGN;
    }

    // Đảo ngược phần nguyên để hiển thị đúng thứ tự
    for (int i = 0; i < index / 2; i++) {
        int temp = digits[i];
        digits[i] = digits[index - 1 - i];
        digits[index - 1 - i] = temp;
    }

    // **Chỉ hiển thị phần thập phân nếu nó khác 0**
    if (decPart > 0) {
        decimalPos = index - 1; // Đặt dấu chấm sau phần nguyên

        // Hiển thị phần thập phân (chỉ lấy các số có ý nghĩa)
        int maxDecDigits = 8 - index; // Số chữ số thập phân có thể hiển thị
        decPart = decPart * pow(10, maxDecDigits); // Đưa phần thập phân về dạng số nguyên
        int decInt = (int)(decPart + 0.5); // Làm tròn tránh sai số

        // Xóa các số 0 vô nghĩa ở cuối phần thập phân
        while (decInt % 10 == 0 && decInt > 0) {
            decInt /= 10;
            maxDecDigits--;
        }

        for (int i = 0; i < maxDecDigits; i++) {
            int decDigit = (decInt / (int)pow(10, maxDecDigits - 1 - i)) % 10;
            digits[index++] = led_map[decDigit];
        }
    }

    // **Chỉ đặt dấu chấm nếu có phần thập phân**
    if (decimalPos >= 0 && decPart > 0) {
        digits[decimalPos] |= 0x80; // Bật bit dấu chấm
    }

    // Căn phải số hiển thị
    int startPos = 8 - index; // Tính vị trí bắt đầu để hiển thị
    int displayDigits[8] = {0}; // Mảng chứa dữ liệu thực tế

    for (int i = 0; i < index; i++) {
        displayDigits[startPos + i] = digits[i]; 
    }

    // Gửi dữ liệu lên MAX7219 theo thứ tự từ trái qua phải
    for (int i = 0; i < 8; i++) {
        Max7219_send(8 - i, displayDigits[i]);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_display_number_pos(uint8_t kytudau1, uint8_t kytudau2, uint8_t kytucuoi1, uint8_t kytucuoi2, uint8_t led_end, double number, int decimal_places){
    uint8_t so_am = 0;
    Max7219_clear();

    Max7219_send(led_end - 4, kytucuoi2);
    Max7219_send(led_end - 3, kytucuoi1);

    char str[16]; 
    sprintf(str, "%f", number); // Giới hạn 2 chữ số phần thập phân
    int pos_start = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '.') {
            pos_start++;
        }
    }    
    if(number < 0) {
        so_am = 1;
        number = -number; 
        led_end = led_end-1;
        if(number < 10){
            led_end +=1;
        }
        else if(number > 100){
            led_end -=1;
        }
    }
    else if(number < 10){
        led_end +=1;
    }
    else if(number > 100){
        led_end -= 1;
    }

    int intPart = (int)number;   // Lấy phần nguyên
    double decPart = number - intPart; // Lấy phần thập phân
    int factor = pow(10, decimal_places);
    int decDigits = (int)round(decPart * factor); // Lấy  chữ số phần thập phân

    // Hiển thị dấu chấm thập phân
    if (decPart > 0) {
        // Hiển thị phần thập phân
        for(int i = 0; i < decimal_places; i++) {
            Max7219_send(led_end, led_map[decDigits % 10]);
            decDigits /= 10;
            led_end ++;
        }

        Max7219_send(led_end, (led_map[intPart % 10]) | 0x80);
    } 
    else {
        Max7219_send(led_end, led_map[intPart % 10]);
    }
    led_end++;
    intPart /= 10;
    
    while (intPart > 0) {
        Max7219_send(led_end++, led_map[intPart % 10]);
        intPart /= 10;
    }
    
    if(so_am == 1) {
        Max7219_send(led_end++, 1); // Hiển thị dấu "-"
    }  
    if(so_am == 1 || intPart == 0){
        Max7219_send(led_end++, kytudau2);
        Max7219_send(led_end, kytudau1);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_display_clock(int hr, int min, int sec, char *period){
    // Hiển thị AM hoặc PM
    if (strcmp(period, "AM") == 0 || strcmp(period, "AM Midnight") == 0) {
        Max7219_send(1, 0x77); // Hiển thị chữ 'A' cho AM
    } else if (strcmp(period, "PM") == 0 || strcmp(period, "PM Lunch") == 0) {
        Max7219_send(1, 0x67); // Hiển thị chữ 'P' cho PM
    }
    Max7219_send(2, 0x01);
    Max7219_send(3, led_map[sec%10]);
    Max7219_send(4, led_map[sec/10]);
    Max7219_send(5, led_map[min%10]|0x80);
    Max7219_send(6, led_map[min/10]);
    Max7219_send(7, led_map[hr%10]|0x80);
    Max7219_send(8, led_map[hr/10]);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_display_flashing(int solan, int time){
    int h = 0;

    int speed = (time * 1000)/(2 * solan);
    uint8_t sang = 0x7F;
    
    while(h < solan){
        for(int i = 0; i<8; i++){
            Max7219_send(i+1, sang);
            //usleep(2000000);
        }   
        usleep(speed*1000);
        Max7219_clear();
        usleep(speed*1000);
        h = h + 1;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_display_matrix(char chucai, uint8_t *hamkytu) {
    uint8_t *hienthi;

    if ((chucai != 0) && (hamkytu == 0)) { // Hiển thị chữ cái
        if (chucai >= 'A' && chucai <= 'Z') {
            hienthi = character[chucai - 'A']; // Dùng bảng ASCII để tính chỉ mục
        }
        else if (chucai >= 0 && chucai <= 9) {
            hienthi = number[chucai - 0]; // Hiển thị số 
        }
        else {
            return; // Thoát nếu không phải ký tự hợp lệ
        }
    } else if (chucai == 0) { // Hiển thị ký tự đặc biệt
        hienthi = hamkytu;
    }

    // Gửi dữ liệu đến LED ma trận
    for (int i = 0; i < 8; i++) {
        Max7219_send(i + 1, hienthi[i]);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_display_matrix_inclined_direction(float angle_x, float angle_y) {
    uint8_t *hienthi= muiten[0]; // Mặc định (dấu chấm hoặc hình vuông)

    if (fabs(angle_x) < 10 && fabs(angle_y) < 10) {
        hienthi= muiten[0];
    }
    else if (fabs(angle_x) > fabs(angle_y)) {  
        if (angle_x > 15) {
            if (angle_y > 12) hienthi = muiten[upright]; // ↗ Lên phải
            else if (angle_y < -12) hienthi = muiten[downright]; // ↘ Xuống phải
            else hienthi = muiten[right]; // → Phải
        } 
        else if (angle_x < -15) {
            if (angle_y > 12) hienthi = muiten[upleft]; // ↖ Lên trái
            else if (angle_y < -12) hienthi = muiten[downleft]; // ↙ Xuống trái
            else hienthi = muiten[left]; // ← Trái
        }
    }
    else {
        if (angle_y > 15) {
            hienthi= muiten[up];  // Mũi tên lên ↑
        } 
        else if (angle_y < -15) {
            hienthi= muiten[down];  // Mũi tên xuống ↓
        }
    }

    Max7219_display_matrix(0, hienthi);
    usleep(10000);

}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

void Max7219_scroll_text(const char *text, int speed) {
    uint8_t display_buffer[8] = {0}; // Bộ đệm hiển thị 8 cột
    uint8_t *current_char;
    int delay_time = speed * 1000; // Chuyển speed từ ms sang µs

    // Thêm khoảng trống trước khi chữ xuất hiện
    for (int shift = 0; shift < 8; shift++) {
        for (int j = 0; j < 8; j++) {
            Max7219_send(j + 1, display_buffer[j]);
        }
        usleep(delay_time);
    }

    // Cuộn từng ký tự trong chuỗi
    for (int i = 0; i < strlen(text); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            current_char = character[text[i] - 'A'];
        } 
        else if (text[i] == ' ') {
            // Nếu là dấu cách, thêm 3 cột trống
            for (int shift = 0; shift < 3; shift++) {
                for (int j = 0; j < 8; j++) {
                    display_buffer[j] <<= 1;
                }

                for (int j = 0; j < 8; j++) {
                    Max7219_send(j + 1, display_buffer[j]);
                }

                usleep(delay_time);
            }
            continue; // Bỏ qua phần cuộn ký tự
        } 
        else {
            continue; // Bỏ qua ký tự không hợp lệ
        }

        // Dịch từng pixel của ký tự từ phải sang trái
        for (int shift = 0; shift < 7; shift++) { // Chỉ dịch 6 cột vì ký tự rộng 6 pixel
            for (int j = 0; j < 8; j++) {
                display_buffer[j] = (display_buffer[j] << 1) | ((current_char[j] >> (7 - shift)) & 1);
            }

            for (int j = 0; j < 8; j++) {
                Max7219_send(j + 1, display_buffer[j]);
            }

            usleep(delay_time);
        }

    }

    // Dịch chữ cuối cùng ra khỏi màn hình
    for (int shift = 0; shift < 7; shift++) {
        for (int j = 0; j < 8; j++) {
            display_buffer[j] <<= 1;
        }

        for (int j = 0; j < 8; j++) {
            Max7219_send(j + 1, display_buffer[j]);
        }

        usleep(delay_time);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------