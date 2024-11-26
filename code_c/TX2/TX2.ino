#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
// Chọn chân cho SoftwareSerial
SoftwareSerial mySerial(2, 3); // RX, TX
#define do_am A0
#define mua A1
int goc=0;
const int rainSensorPin = 4;

// Biến lưu trạng thái của cảm biến mưa
int rainState = 0;

// Biến lưu thời gian bắt đầu mưa
unsigned long startTime = 0;

// Cờ kiểm tra trạng thái mưa
bool isRaining = false;
unsigned long rainDuration=0;
unsigned long timerain = 0;
void setup() {
  // Khởi tạo cổng Serial
  Serial.begin(9600);     // Serial chính để giao tiếp với máy tính
  mySerial.begin(9600);   // Serial giao tiếp với AS32
  pinMode(do_am, INPUT);      // AUX để kiểm tra trạng thái (nếu cần)
  pinMode(rainSensorPin, INPUT);     // AUX để kiểm tra trạng thái (nếu cần)
  Serial.println("AS32 Module Initialized");
//  Serial.begin(115200);
  Wire.begin();

  // Khởi tạo MPU6050
  Serial.println("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    while (1);
  }

  // Cấu hình bộ lọc và các thông số khác
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 ready!");
}
void loop() { 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  rainState = digitalRead(rainSensorPin);

  if (rainState == LOW && !isRaining) { // Phát hi?n b?t d?u mua
    isRaining = true;
    startTime = millis(); // Luu th?i di?m b?t d?u mua
    Serial.println("Bat dau mua...");
  }

  if (rainState == LOW && isRaining) { // Ðang mua
    unsigned long currentTime = millis(); // L?y th?i gian hi?n t?i
    rainDuration = currentTime - startTime; // Tính th?i gian mua
  }

  if (rainState == HIGH && isRaining) { // Phát hi?n mua k?t thúc
    isRaining = false;
    unsigned long currentTime = millis(); // L?y th?i gian hi?n t?i
    rainDuration = currentTime - startTime; // Tính t?ng th?i gian mua
  }

  // Tính toán góc nghiêng (pitch và roll) dựa trên gia tốc kế
  int roll = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  int pitch = atan(-a.acceleration.x / sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
  if(abs(roll)>abs(pitch)) goc =roll;
  if(abs(roll)<abs(pitch)) goc =pitch;
  timerain = rainDuration / 60000.0;
  // Gửi dữ liệu từ máy tính qua AS32
  if (Serial.available()) {
    char data = Serial.read();
    mySerial.write(data);
    Serial.println("Sent: " + String(data));
  }
  String data="";
  // Nhận dữ liệu từ AS32 và hiển thị
  if (mySerial.available()) {
    char receivedData = mySerial.read();
    Serial.println("Received: " + String(receivedData));
    if(receivedData=='>')
    {
      int do_am = analogRead(do_am);
      int nghieng = 10;
      data=String(do_am)+","+String(timerain)+","+String(goc);
      mySerial.println(data);
      Serial.println(data);
      }
  }
}
