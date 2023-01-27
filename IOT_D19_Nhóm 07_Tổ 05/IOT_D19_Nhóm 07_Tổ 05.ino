//Template ID Blynk 

#define BLYNK_TEMPLATE_ID "TMPLPzTE49aT"
#define BLYNK_DEVICE_NAME "nhietdodoamIoT"
#define BLYNK_AUTH_TOKEN "aCMqViDavV9uvlvrQTLxlmo3egQU9FNF"

// Các thư viện wifi, blynk,dht
#define BLYNK_PRINT Serial
#include <WiFi.h>
//#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp32.h>

#include <DHT.h>
// cac biển liên quan đến chân gpio, switch
int pinValue1 = 0;
int pinValue2 = 0;
int pinValue3 = 0;
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "TP-Link_1586";  // name wifi
char pass[] = "khongcomatkhau";  // pass word wifi

BlynkTimer timer;


#define DHTPIN 4 //Connect to DHT22
#define DHTTYPE DHT22
#define ledPin1 23  //Connect to Led1 - cảnh báo cháy
#define ledPin2 22 // Connect to Led2 - điều khiển bật tắt led
#define QuatPin 27 // connect to quat - điều khiển bật tắt quạt
DHT dht(DHTPIN, DHTTYPE);


void canh_bao_chay(float t){
  //set led1 on neu >=70
  if(t>=20.00  ){
    Blynk.virtualWrite(V2, 1);              
    digitalWrite(ledPin1, HIGH);
  }
  //set led1 on neu <=70
  else{
    Blynk.virtualWrite(V2, 0);              
    digitalWrite(ledPin1, LOW);
  }
}
// chế độ tự động
void td_bat_tat_den(float h){
  // tự động bật đèn led1 khi dộ ẩm >=80%
  if(h>=80.00){
    Blynk.virtualWrite(V3, 1);              
    digitalWrite(ledPin2, HIGH);
  }
  //tự động tắt led1 khi độ ẩm dưới 80%
  else{
    Blynk.virtualWrite(V3, 0);              
    digitalWrite(ledPin2, LOW);
  }
}

void td_bat_tat_quat(float t){
  // tự động bật quạt nếu 25<=t<=40
  if(t>=50.00 && t<=90.00){
   Blynk.virtualWrite(V4, 1);              
    digitalWrite(QuatPin, HIGH);
  }
   // tự động tắt đền nếu nằm ngoài khoảng nhiệt dộ trên
  else{
   Blynk.virtualWrite(V4, 0);              
    digitalWrite(QuatPin, LOW);
  }
}



//che de dieu khien
void bat_tat_den(int pinValue1){
  // Switch LED1  On
  if(pinValue1 == 1  ){
    Blynk.virtualWrite(V3, 1);              
    digitalWrite(ledPin2, HIGH);
  }
    // Switch LED1 OFF
  else{
    Blynk.virtualWrite(V3, 0);              
    digitalWrite(ledPin2, LOW);
  }
}

void bat_tat_quat(int pinValue2){
    // Switch  QUat On
  if(pinValue2 == 1  ){
   Blynk.virtualWrite(V4, 1);              
    digitalWrite(QuatPin, HIGH);
  }
   // Switch QUạt Off
  else{
   Blynk.virtualWrite(V4, 0);              
    digitalWrite(QuatPin, LOW);
  }
}

	// hàm setuo
void setup()
{   

  Serial.begin(115200);// đọc Serial monitor ở tốc độ 115200
  pinMode(DHTPIN, INPUT);
//set PinMode cho các đầu ra
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(QuatPin, OUTPUT);
// Đặt các đầu ra ở chế độ LOW - OFF
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(QuatPin, LOW);
  // Kết nối tới Blynk
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, sendSensor);

  }
// đọc dữ liệu dưới dạng int từ V3 - Switch Led2 control
BLYNK_WRITE(V3){
  pinValue1 = param.asInt();
}
// đọc dữ liệu dưới dạng int từ V4 - Switch  Fan control
BLYNK_WRITE(V4){
  pinValue2 = param.asInt();
}
// đọc dữ liệu dưới dạng int từ V5 - Switch MODE control
BLYNK_WRITE(V5){
  pinValue3 = param.asInt();
}

void sendSensor()
{
  float h = dht.readHumidity();// đọc giá trị độ ẩm từ cảm biến
  float t = dht.readTemperature(); // đọc giá trị nhiệt độ từ cảm biến
  //ktra xem co nhan nhiet do or đo am khong
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  // Ghi kết quả Nhiệt độ, Độ ẩm lên 2 biến V0,V1 lên Blynk
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    // Ghi kết quả Nhiệt độ, Độ ẩm lên 2 biến V0,V1 lên Serial monitor
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
     
    // che do tu dong
    canh_bao_chay(t);
    if(pinValue3 == 0){
        td_bat_tat_den(h);
        td_bat_tat_quat(t);
    }
    // che do dieu khien
    else{
      bat_tat_den(pinValue1);
      bat_tat_quat(pinValue2);
    }
}
void loop()
{
  Blynk.run();
  timer.run();
  delay(500);
 }


 