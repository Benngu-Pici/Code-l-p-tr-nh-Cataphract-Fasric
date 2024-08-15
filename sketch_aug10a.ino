#include <PS2X_lib.h>
/******************************************************************
 * Cài đặt chân cho thư viện :
 * - Trên mạch Motorshield của VIA Makerbot BANHMI, có header 6 chân
 *   được thiết kế để cắm tay cầm PS2.
 * Sơ đồ chân header và sơ đồ GPIO tương ứng:
 *   MOSI | MISO | GND | 3.3V | CS | CLK
 *    12     13    GND   3.3V   15   14
 ******************************************************************/

#define PS2_DAT 12 // MISO
#define PS2_CMD 13 // MOSI
#define PS2_SEL 15 // SS
#define PS2_CLK 14 // SLK

#define M1_A 8
#define M1_B 9 
#define M2_A 12
#define M2_B 13
#define M3-A 14
#define M3-B 15
/******************************************************************
 * Lựa chọn chế độ cho tay cầm PS2 :
 *   - pressures = đọc giá trị analog từ các nút bấm
 *   - rumble    = bật/tắt chế độ rung
 ******************************************************************/
#define pressures false
#define rumble false

PS2X ps2x; // khởi tạo class PS2x
// Khai báo các servo
Servo servo6;

Servo servo7

void setup()
{
  Serial.begin(115200);
  Serial.print("Ket noi voi tay cam PS2:");
  // Thiết lập chân của servo
  servo6.attach(6);
  servo7.attach(7);
  int error = -1;
  for (int i = 0; i < 10; i++) // thử kết nối với tay cầm ps2 trong 10 lần
  {
    delay(1000); // đợi 1 giây
    // cài đặt chân và các chế độ: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    Serial.print(".");
  }

  switch (error) // kiểm tra lỗi nếu sau 10 lần không kết nối được
  {
  case 0:
    Serial.println(" Ket noi tay cam PS2 thanh cong");
    break;
  case 1:
    Serial.println(" LOI: Khong tim thay tay cam, hay kiem tra day ket noi vơi tay cam ");
    break;
  case 2:
    Serial.println(" LOI: khong gui duoc lenh");
    break;
  case 3:
    Serial.println(" LOI: Khong vao duoc Pressures mode ");
    break;
  }
}
// Bánh xe xoay về phía trước được quy định là 1
int MotorPin[] = {M1_A, M1_B, M2_A, M2_B};
void motor(int motor, int chanel, int speed) {
    analogWrite(MotorPin[motor*2-2+(chanel+1)%2],0);
    analogWrite(MotorPin[motor*2-2+chanel],speed);
}
void stopmotor(int motor) {
    analogWrite(MotorPin[motor*2-2],0);
    analogWrite(MotorPin[motor*2-2+1],0);
}
void loop()
{
  int ly = map(ps2x.Analog(PSS_LY), 0, 255, -1000, 1000);
  int lx = map(ps2x.Analog(PSS_LX), 0, 255, -1000, 1000);

  // Tính tốc độ dựa trên độ lệch của joystick từ trung tâm
  int speed = sqrt(lx * lx + ly * ly); // Tốc độ dựa trên độ dài vector
  if (speed > 1000) speed = 1000; // Giới hạn tốc độ tối đa
  speed = map(speed,0,1000,0,255)
  // Điều khiển robot theo 8 hướng dựa trên góc nghiêng của joystick

  delay(50);
}
  if (ly > 200) {
    if (lx > 200) {
      // Di chuyển về phía trên phải

      motor(1,1,50);
      motor(2,1,220);
    } else if (lx < -200) {
      // Di chuyển về phía trên trái
      motor(1,1,220);
      motor(2,1,50);
    } else {
      // Di chuyển về phía trên
      motor(1,1,speed);
      motor(2,1,speed);
    }
  } else if (ly < -200) {
    if (lx > 200) {
      // Di chuyển về phía dưới phải
      
      motor(1,0,220);
      motor(2,0,50);
    } else if (lx < -200) {
      // Di chuyển về phía dưới trái
      motor(1,0,50);
      motor(2,0,220);
    } else {
      // Di chuyển về phía dưới
      motor(1,0,speed);
      motor(2,0,speed);
    }
  } else {
    if (lx > 200) {
      // Xoay phải
      motor(1,0,speed);
      motor(2,1,speed);
    } else if (lx < -200) {
      // Xoay trái
      motor(1,1,speed);
      motor(2,0,speed);
    } else {
      // Dừng lại
      stopmotor(1);
      stopmotor(2);
      stopmotor(3);
      stopmotor(4);
    }
  }
  delay(50);

    // Điều khiển Servo để đổ bóng đen vào ô
  if (ps2x.Button(PSB_PAD_DOWN)) {
    servo6.write(90); // Quay 90 độ
  } else if (ps2x.Button(PSB_PAD_UP)) {
    servo6.write(-90); // Quay -90 độ
  
  }
  // Đóng mở tay kẹp
   if (ps2x.Button(PSB_R1)) {
    servo7.write(1);    //Đóng tay kẹp lại
  } else if (ps2x.Button(PSB_R2)) {
    servo7.write(-1);  // Mở tay kẹp
}
  // Điều khiển in/Outtake
  if (ps2x.Button(PSB_L1) {
    motor(1,1,speed); // Intake
  } else if (ps2x.Button(PSB_L2)) {
    smotor(1,0,speed); // Outtake
  
  }
}
}