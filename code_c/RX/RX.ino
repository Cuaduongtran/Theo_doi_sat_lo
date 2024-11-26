#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
// WiFi thông tin
const char* ssid = "XuongStem";
const char* password = "123456789";
int numbers[3];
int numbers2[3];
#define den 16
#define coi 13
// Server URL
const char* serverName = "http://192.168.0.103:5000/receive_data";
// Cấu hình chân RX và TX cho SoftwareSerial
SoftwareSerial mySerial(14, 12); // RX=14, TX=12 (có thể thay đổi theo chân của bạn)
long thoi_gian=2000,dem_thoi_gian=0;
int dem=0;
bool ph1=0, ph3=0;
int ktra_gui=0;
int do_am,thoi_gian_mua,do_nghieng;
int do_am1,thoi_gian_mua1,do_nghieng1;
int do_am2,thoi_gian_mua2,do_nghieng2;
int ktraa=0,ktrab=0;
void setup() {
  
  // Khởi động SoftwareSerial
  mySerial.begin(9600);

  // Serial chính để debug
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  lcd.print("Setup...");
  Serial.println("ESP8266-AS32 Communication Started");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  pinMode(den,OUTPUT);
  pinMode(coi,OUTPUT);
  Serial.println("\nWiFi connected");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("    Ket noi A");
   lcd.setCursor(0, 1);
  lcd.print("    Ket noi B");
}
void updata(int a, int b,  int c)
{
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;  // Tạo một đối tượng WiFiClient
    HTTPClient http;

    // Dữ liệu giả lập
    float temperature = a;
    float humidity = b;
    // Tạo URL với tham số dữ liệu
    String url = String(serverName) + "?temperature=" + String(temperature) + "&humidity=" + String(humidity);

    // Sử dụng WiFiClient khi gọi begin
    http.begin(client, url);

    // Gửi GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully!");
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error sending data");
    }

    http.end();  // Kết thúc kết nối
  }

//  delay(10000);  // Gửi dữ liệu mỗi 60 giây
  }
void loop() {
  // Gửi dữ liệu từ Serial (debug) tới AS32
  if (dem==0) {
    mySerial.print('<');
    Serial.println("Sent to AS32:< ");
    dem=1;
    ph1=1;
    dem_thoi_gian=millis();
//    ktra_gui=ktra_gui+1;
  }
  if(dem==1)
  {
  // Nhận dữ liệu từ AS32 và hiển thị lên Serial (debug)
  if (mySerial.available()) {
    ph1=0;
    ktra_gui=ktra_gui+1;
    String input = mySerial.readString();
    Serial.println("Received from AS32: " + input);
  int index = 0;  // Chỉ số cho mảng

  while (input.length() > 0) {
    int commaIndex = input.indexOf(','); // Tìm dấu phẩy
    if (commaIndex == -1) {
      // Nếu không còn dấu phẩy, đây là phần tử cuối cùng
      numbers[index++] = input.toInt();
      break;
    }

    // Lấy phần tử đầu tiên
    String part = input.substring(0, commaIndex);
    numbers[index++] = part.toInt(); // Chuyển sang số

    // Loại bỏ phần tử vừa xử lý khỏi chuỗi
    input = input.substring(commaIndex + 1);
  }
  do_am1=(1023-numbers[0])/10;
  do_am1=map(do_am1,0,80,0,100);
  thoi_gian_mua1=numbers[1];
  do_nghieng1=abs(numbers[2]);
  Serial.println(do_am1);
  Serial.println(thoi_gian_mua1);
  Serial.println(do_nghieng1);
  // Hiển thị các số đã tách
  }
  if(millis()-dem_thoi_gian>thoi_gian)
  {
    if(ph1==1)
    {
      Serial.println("mat_ket_loi_1");
                lcd.setCursor(0, 0);
          lcd.print("Mat ket noi A");
          digitalWrite(den,1);
//    digitalWrite(coi,0);
      }
      if(ph1==0)
      {
           if(do_nghieng1>45)
        {
          Serial.println(">80");
                lcd.setCursor(0, 0);
          lcd.print(" phat hien do A ");
          digitalWrite(den,1);
          ktraa=2;
          }
          else
          {
        if(do_am1>80){
         Serial.println(">80");
                lcd.setCursor(0, 0);
          lcd.print("  Do am cao A  ");
          digitalWrite(den,1);
          ktraa=1;
        }
        else{
           lcd.setCursor(0, 0);
          lcd.print("    Ket noi A  ");
          ktraa==0;
        }
        }
          }
      
//      updata(numbers[0],numbers[1],numbers[2]);
      dem=2;
    }
  }
  if (dem==2) {
    mySerial.print('>');
    Serial.println("Sent to AS32: >");
    dem=3;
    ph3=1;
    dem_thoi_gian=millis();
    
  }
  if(dem==3)
  {
//    ph3=0;
  // Nhận dữ liệu từ AS32 và hiển thị lên Serial (debug)
  if (mySerial.available()) {
    ph3=0;
    ktra_gui=ktra_gui+1;
    String input = mySerial.readString();
    Serial.println("Received from AS32: " + input);
     // Mảng chứa các số sau khi chuyển đổi
  int index = 0;  // Chỉ số cho mảng

  while (input.length() > 0) {
    int commaIndex = input.indexOf(','); // Tìm dấu phẩy
    if (commaIndex == -1) {
      // Nếu không còn dấu phẩy, đây là phần tử cuối cùng
      numbers2[index++] = input.toInt();
      break;
    }

    // Lấy phần tử đầu tiên
    String part = input.substring(0, commaIndex);
    numbers2[index++] = part.toInt(); // Chuyển sang số

    // Loại bỏ phần tử vừa xử lý khỏi chuỗi
    input = input.substring(commaIndex + 1);
  }

  do_am2=(1023-numbers2[0])/10;
  do_am2=map(do_am2,0,80,0,100);
  thoi_gian_mua2=numbers2[1];
  do_nghieng2=abs(numbers2[2]);
  Serial.println(do_am2);
  Serial.println(thoi_gian_mua2);
  Serial.println(do_nghieng2);
  }
  if(millis()-dem_thoi_gian>thoi_gian)
  {
    if(ph3==1)
    {
      
     Serial.println("mat_ket_loi_2");
                lcd.setCursor(0, 1);
          lcd.print("Mat ket noi B ");
          digitalWrite(den,1);
          
        if(ph1==1)
        {
          digitalWrite(den,1);
          digitalWrite(coi,1);
          }
      }
      if(ph3==0)
      {
        if(do_nghieng2>45)
        {
          Serial.println(">80");
                lcd.setCursor(0, 1);
          lcd.print(" phat hien do B ");
          digitalWrite(den,1);
           ktrab=2;
          }
          else
          {
        if(do_am2>80){
         Serial.println(">80");
                lcd.setCursor(0, 1);
          lcd.print("  Do am cao B  ");
          digitalWrite(den,1);
           ktrab=1;
        }
        else{
           lcd.setCursor(0, 1);
          lcd.print("    Ket noi B  ");
           ktrab=0;
        }
        }
      }
//              Serial.print("ph2: ");
//      Serial.println(ph3);
//        updata(numbers[0],numbers[1],numbers[2]);
      dem=0;
    }
  }
  if(ktra_gui%2==0)
  {
  if((ph1==0)&&(ph3==0))
  {if((ktraa==0)&&(ktrab==0)){
    digitalWrite(den,0);
    digitalWrite(coi,0);
  }
  if((ktraa==2)&&(ktrab==2))
  {
       digitalWrite(den,1);
    digitalWrite(coi,1);
  }
    }
//  if((ph1==1)||(ph3==1))
//  {
//    digitalWrite(den,1);
//    digitalWrite(coi,0);
//    }

  }
    if(ktra_gui>=5)
  {
    if((ph1==0)&&(ph3==0))
    {

    do_am=(do_am1+do_am2)/2;
    thoi_gian_mua=(thoi_gian_mua1+thoi_gian_mua2)/2;
    do_nghieng=(do_nghieng1+do_nghieng2)/2;
    }
    if((ph1==1)&&(ph3==0))
    {
     do_am=do_am2;
    thoi_gian_mua=thoi_gian_mua2;
    do_nghieng=do_nghieng2;
     }
        if((ph1==0)&&(ph3==1))
    {
     do_am=do_am1;
    thoi_gian_mua=thoi_gian_mua1;
    do_nghieng=do_nghieng1;
     }
//     Serial.print("ph1: ");
//      Serial.println(ph1);
//      Serial.print("ph2: ");
//      Serial.println(ph3);
//     Serial.print("gui: ");
//    Serial.println(do_am);
//    Serial.println(thoi_gian_mua);
//    Serial.println(do_nghieng);
    updata(do_am,thoi_gian_mua,do_nghieng);
    ktra_gui=0;
  }
  
}
