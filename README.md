# MODULE MAX7219

- [Giới thiệu](#giới-thiệu)
- [Thông số kỹ thuật](#thông-số-kỹ-thuật)
- [Sơ đồ kết nối](#Sơ-đồ-kết-nối)
- [Hướng dẫn sử dụng](#hướng-dẫn-sử-dụng)
  - [Bật chế độ SPI](#bật-chế-độ-SPI)
  - [Cài đặt và sử dụng](#cài-đặt-và-sử-dụng)
- [Tính năng của thư viện](#tính-năng-của-thư-viện)
- [Tài liệu tham khảo](#tài-liệu-tham-khảo)

# Giới thiệu
MAX7219 là một vi mạch điều khiển LED tích hợp do Maxim Integrated (nay thuộc Analog Devices) sản xuất, được thiết kế để điều khiển các loại hiển thị LED như ma trận LED 8x8, LED 7 đoạn (tối đa 8 chữ số) hoặc thanh LED đơn lẻ dưới dạng common-cathode. IC này sử dụng giao tiếp nối tiếp (SPI-like) để kết nối với vi điều khiển, giúp đơn giản hóa việc điều khiển nhiều LED đồng thời mà chỉ cần sử dụng 3 chân tín hiệu: DIN (Data In), CLK (Clock) và LOAD/CS (Chip Select).
Nó thì được ứng dụng rộng rãi, với một số ứng dụng như sau:  
- Hiển thị LED 7 đoạn trong đồng hồ, bộ đếm, hoặc bảng thông tin.
- Hiển thị ký tự hoặc hiệu ứng ánh sáng trên ma trận LED 8x8.
- Hệ thống hiển thị trong các thiết bị nhúng, Arduino, STM32, v.v.
- Bảng điều khiển công nghiệp, thiết bị đo lường và các ứng dụng hiển thị khác.
  
# Thông số kỹ thuật
- Điện áp hoạt động: 4.0V đến 5.5V  
- Dòng điện tiêu thụ ở chế độ chờ: 150 µA (chế độ tiết kiệm năng lượng)
- Dòng điện tối đa qua mỗi chân điều khiển chữ số (DIG 0–7): 500 mA
- Dòng điện tối đa qua mỗi chân điều khiển đoạn (SEG A–G, DP): 100 mA  
- Giao diện nối tiếp: Tương thích với SPI, QSPI và MICROWIRE  
- Tốc độ giao tiếp tối đa: 10 MHz  
- Chế độ giải mã: Hỗ trợ giải mã BCD code-B hoặc không giải mã cho từng chữ số  

# Sơ đồ kết nối
Phần cứng của MAX7219:  
- Pin 1: VCC (Nguồn cấp)
- Pin 2: GND (Nối đất, tất cả điện áp đều được tham chiếu đến GND)  
- Pin 3: DIN (MOSI, dữ liệu vào, Master Out Slave In)  
- Pin 4: CS  (chân chọn thiết bị)  
- Pin 5: CLK (Nhận xung clock từ vi điều khiển)  

Sơ đồ kết nối MAX7219 với rapsberry:    
- VCC -> 2(5V)  
- GND -> 25  
- DIN -> 19(MOSI)  
- CS  -> 24(CEO)  
- CLK -> 23(SCK)
  
# Hướng dẫn sử dụng
### Bật chế độ SPI 
```c
sudo raspi-config
```
Sau đó chọn: Interface option -> SPI -> Enable
  
### Cài đặt và sử dụng  
Bước 1:  
Tạo ra 2 file Max7219.c và file Max7219.h hoặc tải hoặc tham khảo 2 file code trên github. 

Bước 2:  
Chuyển file .c thành file .o:
```c
gcc -c -fPIC Max7219.c -o Max7219.o
```
Bước 3:  
Chuyển file .o thành file .so:
```c
gcc -shared -o libMax7219.so Max7219.o
```
Bước 4:  
Thêm thư viện .so vào thư viện động trên raspberry:
```c
sudo cp libMax7219.so /usr/lib/
```
Bước 5:  
Để dùng các chức năng người dùng cần thêm vào file code thư viện sau:    
```c
#include "Max7219.h"
```
Bước 6:  
Biên dịch và chạy file (file_code.c là file bạn đang viết):
```c
gcc file_code.c -o run -L. -lMax7219 -lwiringPi -lm 
sudo ./run
```

# Tính năng của thư viện
Ở đây mình sẽ liệt kê chức năng tổng quan của một số hàm trong thư viện, bạn có thể xem chi tiết chức năng và cách khai báo giá trị của từng biến của hàm trong file Max7219.h).
- Hàm gửi dữ liệu xuống thanh ghi trên Max7219
```c
void Max7219_send(uint8_t address, uint8_t data)
```  

- Hàm cấu hình Max7219
 ```c
void Max7219_init(uint8_t decode_mode, uint8_t intensity, uint8_t scan_limit)
```  

- Hàm xóa số trên 7 led
```c
void Max7219_clear(void)
```  

- Hàm chỉnh độ sáng
```c
void Max7219_set_intensity(uint8_t intensity)
```  

- Hàm hiển thị từng thanh led từ thanh a đến thanh h
```c
void Max7219_display_atoh(void)
```  

- Hàm hiển thị số (số nguyên, số thực)
```c
void Max7219_display_number(double number)
```

- Hàm hiển thị số có thể thêm 2 ký tự ở đâu và 2 ký tự ở cuối, có thể chọn vị trí hiện thị số cuối (số nguyên, số thực)
```c
void Max7219_display_number_pos(uint8_t kytudau1, uint8_t kytudau2, uint8_t kytucuoi1, uint8_t kytucuoi2, uint8_t led_end, double number, int decimal_places)
```

- Hàm hiển thị đồng hồ số, period là chế độ 12H hoặc 24H, kết hợp với thư viện DS3231 hoặc tự viết hàm lấy tham số đầu vào
```c
void Max7219_display_clock(int hr, int min, int sec, char *period)
```

- Hàm hiển thị led nhấp nháy
```c
void Max7219_display_flashing(int solan, int time)
```

- Hàm hiển thị led matrix
```c
void Max7219_display_matrix(char chucai, uint8_t *hamkytu) 
```

- Hàm hiển thị mũi tên theo độ nghiêng của Mpu6050, kết hợp với thư viện MPU6050, hoặc tự viết hàm lấy tham số đầu vào
```c
void Max7219_display_matrix_inclined_direction(float angle_x, float angle_y)
```

- Hàm chạy chữ trên led matrix
```c
void Max7219_scroll_text(const char *text, int speed)
```
## 📌📌Lưu ý: Một số hàm có thể có chức năng chưa hoàn toàn đúng, người dùng có thể viết lại hoặc tối ưu code để phù hợp với mục đích sử dụng 📌📌

# Tài liệu tham khảo
- [Datasheet MAX7219](https://www.analog.com/media/en/technical-documentation/data-sheets/max7219-max7221.pdf)
