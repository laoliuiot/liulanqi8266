
//wifi 控制，有个最大的毛病就是 消耗电流大，这个用电池供电的话，需要70ma左右，如果是市电供电无所谓了
//如果想降低耗电量，可以在loop里面添加delay(1000),耗电量会下降一半多，反应变慢一些而已
//http://192.168.1.4/?TURN_ON_LED=1
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>  // 引入Web服务器库

// WiFi配置
const char* ssid = "ChinaNet-ggzf";
const char* password = "jlkjxlav";
 

// Web服务器端口（默认80）
ESP8266WebServer server(80);

const int ledPin = 2;  // LED连接的GPIO引脚（通常为内置LED）

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // 设置LED引脚为输出
  
  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // 打印ESP8266的IP地址
  Serial.print("ESP8266 IP Address: ");
  Serial.println(WiFi.localIP());

  // 处理浏览器发送的请求
  server.on("/", handleRoot);  // 访问根目录时调用的函数

  // 启动Web服务器
  server.begin();
  WiFi.mode(WIFI_STA);  // 设置为Station模式
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);  // 启用Modem Sleep模式
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();  // 处理客户端请求
}

// 处理根目录的请求
void handleRoot() {
  // 解析查询参数
  String ledState = server.arg("TURN_ON_LED");
  
  // 如果有TURN_ON_LED参数
  if (ledState.length() > 0) {
    if (ledState == "1") {
      digitalWrite(ledPin, LOW);  // 开启LED（通常内置LED低电平点亮）
      server.send(200, "text/plain", "LED is ON");
      Serial.println("LED turned ON");
    } else if (ledState == "0") {
      digitalWrite(ledPin, HIGH); // 关闭LED
      server.send(200, "text/plain", "LED is OFF");
      Serial.println("LED turned OFF");
    } else {
      server.send(400, "text/plain", "Invalid value for TURN_ON_LED"); // 错误处理
    }
  } else {
    server.send(400, "text/plain", "TURN_ON_LED parameter missing");
  }
}
