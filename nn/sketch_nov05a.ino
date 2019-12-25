#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define DEBUG 0
#define pin1 16                         
#define pin2 14  
#define pin3 13 
#define pin4 12 

const char* ssid     = "XXXXXX";              // XXXXXX -- 使用时请修改为当前你的 wifi ssid
const char* password = "XXXXXX";              // XXXXXX -- 使用时请修改为当前你的 wifi 密码
const char* host = "api.heclouds.com";
const char* APIKEY = "JwW7wq1EF0y7=w5DsonfqLQwq=E=";      // API KEY
int32_t deviceId = 577386286;                             // Device ID
int A=0;                                                  //状态标记码，决定了下一轮读哪个数据流

const size_t MAX_CONTENT_SIZE = 1024;
const unsigned long HTTP_TIMEOUT = 2100;               // max respone time from server

WiFiClient client;
const int tcpPort = 80;

struct UserData {
  int errno_val;                // 错误返回值
  char error[32];               // 错误返回信息
  int test_led_Val;             // TEST LED 状态值
  char udate_at[32];            // 最后更新时间及日期
};

// Skip HTTP headers so that we are at the beginning of the response's body
//  -- 跳过 HTTP 头，使我们在响应正文的开头
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }

  return ok;
}

// Read the body of the response from the HTTP server -- 从HTTP服务器响应中读取正文
void readReponseContent(char* content, size_t maxSize) {
  //  size_t length = client.peekBytes(content, maxSize);
  size_t length = client.readBytes(content, maxSize);
  delay(20);
  Serial.println(length);
  Serial.println("Get the data from Internet!");
  content[length] = 0;
  Serial.println(content);
  Serial.println("Read Over!");
}

bool parseUserData_test(char* content, struct UserData* userData) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  //  -- 根据我们需要解析的数据来计算JSON缓冲区最佳大小
  // This is only required if you use StaticJs
   const size_t BUFFER_SIZE = 1024;

  // Allocate a temporary memory pool on the stack -- 在堆栈上分配一个临时内存池
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  //  -- 如果堆栈的内存池太大，使用 DynamicJsonBuffer jsonBuffer 代替
  // If the memory pool is too big for the stack, use this instead:
  //  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(content);

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }
  
  // Here were copy the strings we're interested in
  userData->errno_val = root["errno"];
  strcpy(userData->error, root["error"]);
  if ( userData->errno_val == 0 ) {
    userData->test_led_Val = root["data"]["current_value"];
    strcpy(userData->udate_at, root["data"]["update_at"]);
    Serial.print("YF-Test_LED Value : ");
    Serial.print(userData->test_led_Val);
    Serial.print("\t The last update time : ");
    Serial.println(userData->udate_at);
    
///////////////////////////////////////////////
//以下一段是硬件功能实现代码，loop函数每轮根据接收到的值触发其中一种情况

if(userData->test_led_Val==1){
digitalWrite(pin1,1);  
digitalWrite(pin2,0);
digitalWrite(pin3,0);                      
A=1;
}
if(userData->test_led_Val==2){
digitalWrite(pin1,0);  
digitalWrite(pin2,1);
digitalWrite(pin3,0);
A=1;
}        
if(userData->test_led_Val==3){
digitalWrite(pin1,0);  
digitalWrite(pin2,0);
digitalWrite(pin3,1);
A=1;
}    
if(userData->test_led_Val==100){
digitalWrite(pin4,1);
A=0;
}
if(userData->test_led_Val==99){
A=0;
}      
if(userData->test_led_Val==0){
digitalWrite(pin1,0);
digitalWrite(pin2,0);
digitalWrite(pin3,0);
digitalWrite(pin4,0);
A=0;
}

///////////////////////////////////////////////

  Serial.print("errno : ");
  Serial.print(userData->errno_val);
  Serial.print("\t error : ");
  Serial.println(userData->error);
   return true;
}
}

void setup() {
  WiFi.mode(WIFI_AP_STA);                 //set work mode:  WIFI_AP /WIFI_STA /WIFI_AP_STA
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(500);

  if (!client.connect(host, tcpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/devices/";
  url += String(deviceId);
  url += "/datastreams/";
  
  if(A==0){  url += "status";}                                //数据流名称
  else{  url += "status2";}

  // This will send the request to the server
  String send_data = String("GET ") + url + " HTTP/1.1\r\n" +
                     "api-key:" + APIKEY + "\r\n" +
                     "Host:" + host + "\r\n" +
                     "Connection: close\r\n\r\n";
  client.print(send_data);
  if (DEBUG)
    Serial.println(send_data);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 2000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  if (skipResponseHeaders())  { //  发送请求
    char response[MAX_CONTENT_SIZE];
    readReponseContent(response, sizeof(response));
    UserData userData_testLED;
    if (parseUserData_test(response, &userData_testLED)) {
      Serial.println("daily data parse OK!");
    }
  }
  Serial.println("closing connection");
}



