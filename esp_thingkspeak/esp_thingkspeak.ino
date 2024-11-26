#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi thông tin
const char* ssid = "XuongStem";
const char* password = "123456789";

// Server URL
const char* serverName = "http://192.168.100.178:5000/receive_data";

void setup() {
  Serial.begin(115200);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;  // Tạo một đối tượng WiFiClient
    HTTPClient http;

    // Dữ liệu giả lập
    float temperature = random(0,100);
    float humidity = random(0,100);
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

  delay(10000);  // Gửi dữ liệu mỗi 60 giây
}
