//Template ID Blynk 

#define BLYNK_TEMPLATE_ID "TMPLZX_Ozrjl"
#define BLYNK_DEVICE_NAME "TieuLuanIoT"
#define BLYNK_AUTH_TOKEN "EqFvRPo4TG2uq6Ql4ywXc5aKu1P4cDMy"

// Các thư viện wifi, blynk,dht
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
//Thư viện này cho phép bạn gửi và nhận tín hiệu hồng ngoại 
//trên ESP8266 hoặc ESP32
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <DHT.h>
// cac biến liên quan đến chân gpio, switch
int pinValue = 0;// tùy chỉnh
int pinValue1 = 0;// điều hòa
int pinValue2 = 0;// máy tạo ẩm
int pinValue3;// slider điều chỉnh nhiệt độ điều hòa
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "MI 8 SE";  // name wifi
char pass[] = "04062001";  // pass word wifi

BlynkTimer timer;

#define DHTPIN 4 //Connect to DHT22
//#define HumiPin 22 // connect to máy tạo ẩm 
const uint16_t kIrLed = 26;// connect IR phát tín hiệu hồng ngoại
#define DHTTYPE DHT22

IRsend irsend(kIrLed);
DHT dht(DHTPIN, DHTTYPE);

bool Button_on;// kiểm tra điều hòa bật tắt
// biến đánh dấu để  bật tắt điều hòa 1 lần khi nhiệt độ thay đổi
// nếu không có biến đánh dấu này, điều hòa sẽ bị bật tắt ko kiểm soát
bool ok1;
bool ok2;
int td1;
int td2;




// void td_may_tao_am(float h){
//   // tự động bật máy tạo ẩm nếu độ ẩm dưới 40%
//   if(h<40){
//    Blynk.virtualWrite(V4, 1);              
//     digitalWrite(HumiPin, HIGH);
//   }
//   else{// tự động tắt máy tạo ẩm nếu độ ẩm >40%
//    Blynk.virtualWrite(V4, 0);              
//     digitalWrite(HumiPin, LOW);
//   }
// }

// //che de dieu khien
// void bat_may_tao_am(int pinValue2){
//   if(pinValue2 == 1  ){
//    Blynk.virtualWrite(V4, 1);              
//     digitalWrite(HumiPin, HIGH);
//   }
//   else{
//    Blynk.virtualWrite(V4, 0);              
//     digitalWrite(HumiPin, LOW);
//   }
// }

	// hàm setuo
void setup(){   

  Serial.begin(115200);// đọc Serial monitor ở tốc độ 115200
  pinMode(DHTPIN, INPUT);
  //set PinMode cho các đầu ra
   //pinMode(HumiPin, OUTPUT);
  //// Đặt các đầu ra ở chế độ LOW - OFF
  //digitalWrite(HumiPin, LOW);
  // Kết nối tới Blynk

  td1=0;
  td2=0;
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  irsend.begin();
  

  }

//ktra bật tắt tuy chinh
BLYNK_WRITE(V2){
  pinValue = param.asInt();
}

// kiểm tra bật tắt Air
BLYNK_WRITE(V3){
  pinValue2 = param.asInt();
  if(pinValue2 == 1){
      Button_on = true;
      ok1 = true;
    }
    else{
      Button_on = false;
      ok2=true;
    }
}
BLYNK_WRITE(V5){ // lấy giá trị nhiệt độ điều hòa
   pinValue3 = param.asInt();
}
//ktra  bật tắt máy tạo ẩm
// BLYNK_WRITE(V4){
//   pinValue2 = param.asInt();
// }

// điều khiển bật tắt điều hòa
void controlAir(uint16_t rawData[211],uint16_t rawData_Off[211],uint8_t state_on[13],uint8_t state_off[13]){
  if(Button_on== true && ok1 == true){
      irsend.sendRaw(rawData, 211, 38);
      delay(1000);
      irsend.sendElectraAC(state_on,13,2);
      ok1=false;
    }
    if(Button_on == false && ok2 == true){
      irsend.sendRaw(rawData_Off, 211, 38);
      delay(1000);
      irsend.sendElectraAC(state_off,13,2);
      ok2 = false;
    }
}

// Các mã IR điều khiển bật tắt điều hòa tương ứng 
// với các nhiệt độ từ 25-28
// thu được sau khi thực hiện thu tín hiệu hồng ngoại
//ở phần trước
void xulyAir(){
  {
    Serial.print(pinValue3);
    if(pinValue3==25){
      uint16_t rawData[211]  = {8966, 4562,  500, 1752,  500, 1752,  496, 638,  474, 614,  526, 584,  498, 612,  528, 1726,  552, 1702,  522, 1728,  498, 1780,  472, 1752,  524, 1726,  502, 612,  500, 610,  498, 616,  550, 1698,  500, 610,  500, 614,  576, 536,  524, 590,  496, 640,  472, 1754,  522, 1728,  498, 1750,  502, 612,  500, 610,  500, 614,  498, 614,  500, 638,  470, 616,  520, 588,  524, 588,  496, 614,  524, 586,  498, 612,  524, 588,  526, 586,  498, 1752,  522, 590,  522, 586,  526, 586,  526, 586,  500, 612,  498, 612,  524, 588,  522, 586,  526, 588,  496, 612,  500, 610,  502, 610,  500, 612,  524, 612,  474, 612,  498, 1754,  498, 612,  502, 638,  500, 586,  522, 592,  496, 612,  526, 612,  530, 556,  498, 638,  502, 586,  524, 584,  500, 612,  526, 588,  496, 612,  524, 588,  500, 616,  520, 588,  498, 614,  496, 614,  500, 612,  500, 614,  496, 614,  500, 638,  472, 612,  522, 1730,  498, 612,  524, 588,  552, 560,  500, 614,  522, 612,  474, 616,  522, 588,  496, 640,  494, 592,  496, 612,  500, 1754,  524, 612,  474, 1752,  496, 614,  522, 590,  524, 614,  470, 614,  524, 614,  498, 1726,  526, 1728,  498, 1754,  498, 638,  472, 1754,  498, 614,  524, 612,  496, 1730,  500};  // UNKNOWN F9924DF2

      uint16_t rawData_Off[211] = {8970, 4538,  522, 1752,  502, 1750,  500, 614,  498, 612,  500, 610,  526, 584,  502, 1752,  500, 1752,  500, 1752,  524, 1728,  522, 1726,  500, 1752,  500, 612,  498, 612,  526, 588,  498, 1752,  500, 612,  500, 612,  526, 586,  500, 610,  524, 590,  524, 1726,  524, 1726,  500, 1752,  498, 612,  526, 586,  500, 612,  524, 588,  498, 640,  472, 612,  500, 610,  500, 614,  498, 610,  502, 612,  522, 588,  500, 612,  524, 586,  524, 1728,  498, 612,  500, 614,  496, 614,  522, 588,  498, 610,  500, 612,  524, 588,  500, 614,  524, 586,  500, 612,  500, 612,  500, 610,  500, 614,  522, 586,  526, 586,  524, 1728,  522, 590,  498, 614,  496, 616,  522, 586,  522, 588,  498, 616,  522, 586,  500, 612,  500, 612,  522, 590,  496, 612,  498, 614,  496, 614,  500, 614,  498, 612,  524, 588,  524, 590,  496, 612,  524, 586,  500, 612,  524, 590,  496, 612,  498, 612,  498, 612,  500, 612,  522, 590,  496, 614,  498, 638,  500, 586,  578, 560,  496, 588,  522, 588,  524, 586,  500, 614,  520, 1732,  522, 588,  496, 1754,  552, 562,  522, 586,  500, 612,  524, 588,  498, 612,  550, 1702,  524, 1726,  524, 1728,  498, 612,  550, 1704,  522, 1728,  496, 1778,  474, 614,  498};  // UNKNOWN 2F6C1E4D

    //controlAir(rawData,rawData_Off);
  }

  else if(pinValue3==26){
      uint16_t rawData[211] = {9028, 4504,  558, 1694,  558, 1692,  560, 552,  556, 554,  556, 554,  560, 554,  532, 1720,  554, 1696,  556, 1696,  556, 1694,  558, 1692,  558, 554,  530, 1722,  556, 556,  582, 528,  560, 1692,  556, 556,  556, 554,  556, 554,  558, 554,  558, 554,  556, 1694,  556, 1696,  556, 1696,  556, 556,  556, 556,  554, 556,  556, 556,  556, 556,  556, 554,  558, 554,  558, 556,  556, 554,  558, 554,  530, 580,  558, 556,  554, 554,  556, 1696,  556, 556,  554, 558,  556, 556,  542, 568,  554, 558,  530, 582,  554, 558,  554, 556,  556, 556,  554, 556,  556, 556,  552, 558,  556, 556,  554, 556,  556, 556,  554, 1696,  554, 556,  556, 558,  552, 558,  554, 558,  556, 558,  552, 556,  556, 558,  552, 560,  552, 558,  554, 558,  552, 558,  554, 558,  554, 558,  554, 558,  552, 580,  530, 558,  554, 580,  530, 582,  530, 580,  530, 580,  530, 582,  530, 582,  530, 582,  530, 1722,  530, 582,  530, 582,  528, 584,  528, 584,  526, 584,  528, 584,  528, 584,  526, 586,  524, 586,  526, 586,  524, 1728,  524, 588,  524, 1728,  522, 588,  524, 588,  524, 590,  522, 588,  524, 588,  524, 1728,  524, 1728,  524, 1728,  522, 1728,  524, 1728,  522, 590,  522, 590,  522, 1730,  522};  // ELECTRA_AC
       uint8_t state_on[13] = {0xC3, 0x97, 0xE0, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x05, 0x9F};

      uint16_t rawData_Off[211] = {9028, 4504,  532, 1718,  560, 1692,  532, 580,  560, 550,  534, 580,  556, 554,  558, 1694,  556, 1694,  556, 1696,  554, 1696,  556, 1694,  558, 554,  560, 1692,  554, 556,  532, 580,  556, 1696,  556, 556,  556, 554,  556, 556,  556, 554,  534, 578,  558, 1694,  532, 1718,  558, 1694,  556, 556,  532, 580,  556, 556,  554, 558,  530, 582,  554, 556,  558, 554,  556, 556,  558, 556,  554, 554,  558, 554,  556, 556,  556, 556,  556, 1696,  554, 560,  556, 554,  554, 558,  556, 556,  554, 558,  528, 582,  554, 556,  556, 556,  554, 558,  554, 556,  556, 556,  554, 558,  554, 556,  554, 556,  554, 556,  556, 1696,  554, 556,  530, 582,  556, 558,  552, 558,  554, 556,  554, 558,  552, 562,  550, 560,  552, 560,  550, 582,  530, 558,  528, 584,  552, 580,  532, 580,  532, 586,  526, 582,  504, 606,  530, 582,  532, 580,  528, 584,  528, 582,  530, 582,  528, 582,  528, 584,  528, 584,  526, 584,  528, 584,  526, 584,  526, 586,  526, 586,  524, 588,  524, 586,  524, 588,  524, 588,  524, 1728,  522, 588,  524, 1728,  524, 588,  522, 590,  522, 588,  522, 590,  522, 590,  522, 1728,  524, 1728,  522, 1728,  522, 1730,  524, 1728,  522, 1730,  522, 1730,  522, 590,  522};  // ELECTRA_AC
       uint8_t state_off[13] = {0xC3, 0x97, 0xE0, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x05, 0x7F};

      controlAir(rawData,rawData_Off,state_on,state_off);
  }
  else if(pinValue3==27){
      uint16_t rawData[211] = {9030, 4500,  560, 1692,  558, 1694,  558, 554,  556, 556,  556, 554,  558, 554,  558, 1694,  558, 1694,  558, 1692,  558, 1694,  558, 1694,  556, 1694,  556, 1696,  556, 556,  556, 556,  556, 1696,  556, 556,  556, 558,  554, 556,  554, 560,  552, 560,  552, 1698,  554, 1700,  550, 1700,  552, 560,  552, 582,  530, 582,  530, 582,  530, 584,  528, 584,  528, 582,  528, 584,  528, 584,  526, 586,  526, 586,  524, 588,  524, 586,  522, 1730,  522, 588,  522, 590,  522, 590,  522, 588,  522, 590,  522, 588,  522, 590,  522, 588,  520, 590,  522, 590,  522, 590,  522, 588,  522, 590,  522, 590,  522, 592,  520, 1730,  520, 590,  522, 590,  522, 590,  522, 590,  520, 590,  522, 590,  522, 590,  522, 592,  520, 590,  520, 592,  520, 592,  520, 592,  520, 590,  522, 614,  496, 590,  522, 590,  522, 614,  496, 616,  496, 614,  496, 614,  496, 616,  496, 616,  498, 614,  494, 1756,  472, 640,  496, 616,  496, 616,  494, 616,  494, 618,  496, 616,  470, 642,  470, 642,  474, 638,  494, 616,  478, 1774,  470, 642,  470, 1782,  470, 640,  472, 666,  446, 640,  470, 666,  450, 662,  446, 1806,  446, 1806,  446, 1804,  446, 666,  446, 666,  446, 1782,  470, 666,  446, 1806,  446};  // UNKNOWN 3E5B0D5A

      uint16_t rawData_Off[211] = {9024, 4506,  556, 1700,  550, 1698,  554, 556,  556, 556,  530, 580,  556, 556,  554, 1698,  528, 1724,  554, 1696,  554, 1696,  556, 1696,  554, 1720,  530, 1700,  550, 560,  554, 556,  556, 1720,  532, 556,  554, 560,  528, 584,  554, 560,  552, 558,  552, 1698,  528, 1724,  554, 1720,  530, 560,  554, 580,  532, 558,  552, 582,  532, 558,  552, 558,  554, 560,  526, 606,  530, 582,  506, 584,  526, 606,  506, 584,  554, 560,  550, 1722,  506, 606,  530, 582,  530, 560,  552, 582,  528, 582,  532, 560,  550, 582,  506, 584,  526, 606,  530, 560,  552, 560,  554, 580,  530, 582,  530, 582,  526, 586,  506, 1746,  528, 582,  530, 582,  530, 582,  528, 582,  528, 584,  504, 608,  528, 582,  528, 584,  526, 584,  528, 584,  528, 582,  528, 582,  504, 608,  528, 584,  528, 584,  526, 584,  502, 610,  526, 586,  500, 610,  526, 586,  524, 588,  524, 586,  524, 588,  522, 588,  524, 590,  524, 586,  500, 612,  524, 588,  522, 590,  524, 588,  498, 614,  522, 588,  524, 588,  524, 586,  524, 1728,  524, 588,  522, 1728,  524, 588,  522, 588,  522, 588,  504, 608,  522, 588,  522, 1730,  522, 1730,  524, 1728,  522, 588,  524, 588,  522, 588,  522, 590,  522, 1728,  522};  // ELECTRA_AC

    //controlAir(rawData,rawData_Off);
  }
  else if(pinValue3==28){
      uint16_t rawData[211] = {9018, 4534,  528, 1724,  504, 1748,  500, 612,  502, 608,  500, 610,  502, 610,  526, 1726,  524, 1726,  500, 1750,  500, 1752,  524, 1726,  500, 614,  498, 612,  500, 1752,  524, 588,  524, 1726,  526, 588,  500, 612,  524, 588,  524, 590,  500, 612,  524, 1728,  500, 1752,  506, 1746,  498, 614,  524, 586,  524, 588,  524, 588,  498, 614,  522, 588,  500, 614,  522, 588,  524, 590,  522, 588,  500, 612,  498, 614,  522, 588,  522, 1730,  522, 588,  500, 612,  522, 590,  524, 588,  522, 590,  522, 590,  498, 612,  500, 612,  498, 614,  498, 612,  500, 612,  498, 612,  548, 562,  498, 612,  524, 588,  502, 1748,  500, 612,  522, 590,  520, 590,  524, 590,  498, 614,  524, 586,  524, 588,  522, 588,  498, 614,  522, 590,  498, 612,  498, 614,  522, 590,  524, 588,  498, 614,  522, 590,  498, 614,  522, 590,  498, 614,  522, 588,  498, 612,  524, 588,  496, 616,  522, 1730,  522, 590,  522, 590,  520, 590,  524, 590,  522, 612,  496, 590,  522, 590,  498, 614,  522, 590,  522, 590,  498, 1754,  516, 594,  522, 1756,  472, 614,  522, 616,  498, 588,  522, 614,  496, 590,  522, 1728,  498, 1754,  520, 1756,  498, 1730,  524, 588,  524, 1728,  520, 616,  472, 1756,  496};  // UNKNOWN 5A708B48

      uint16_t rawData_Off[211] = {8996, 4556,  506, 1726,  524, 1726,  550, 584,  504, 588,  548, 584,  530, 562,  524, 1748,  526, 1724,  528, 1724,  502, 1750,  502, 1750,  502, 610,  502, 610,  502, 1750,  524, 588,  526, 1726,  524, 588,  500, 612,  500, 612,  500, 612,  526, 586,  500, 1752,  524, 1728,  524, 1728,  500, 612,  522, 590,  524, 586,  500, 614,  522, 588,  522, 590,  522, 588,  498, 614,  524, 586,  524, 588,  522, 588,  522, 588,  524, 588,  524, 1728,  498, 612,  524, 590,  498, 612,  524, 588,  524, 588,  498, 614,  498, 612,  498, 612,  500, 612,  524, 588,  498, 614,  524, 588,  498, 614,  498, 612,  524, 590,  496, 1754,  520, 588,  524, 590,  522, 588,  524, 588,  524, 588,  498, 614,  522, 590,  498, 612,  524, 586,  500, 612,  522, 590,  522, 588,  522, 588,  498, 614,  510, 602,  522, 590,  498, 614,  498, 616,  520, 588,  498, 614,  522, 590,  498, 614,  522, 590,  520, 590,  522, 590,  522, 590,  524, 588,  522, 590,  520, 590,  498, 614,  522, 588,  522, 590,  522, 590,  522, 590,  496, 1756,  520, 590,  520, 1730,  496, 616,  522, 588,  522, 590,  522, 590,  498, 614,  522, 1730,  496, 1754,  522, 1728,  498, 1754,  522, 590,  520, 590,  498, 614,  522, 1730,  520};  // ELECTRA_AC

      //controlAir(rawData,rawData_Off);
  }
  
  }
}

// đọc dữ liệu từ cảm biển nhiệt độ độ ẩm DHT22
// hiển thị thông số lên blynk và serial monitor
//xử lý bật tắt các thiết bị đầu ra ở các chế độ điều khiển và tùy chỉnh
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
     
    // che độ điều khiển

    if(pinValue == 0){    
        xulyAir();
        td2=0;
        td1=0;
      //  bat_may_tao_am(pinValue2);        
    }
    // chế độ tự động
    else{   
        if((t<25 || t>28) &&(td1==0) ){
          Blynk.virtualWrite(V3, 1);  
          Button_on= true;
          ok1 = true;
           xulyAir();
          ok1=false;
          td1=1;
          td2=0;
        }
         else if((t>=25 && t<=28) && td2==0){
          Blynk.virtualWrite(V3, 0);
          Button_on= false;
          ok2 = true;
           xulyAir();
          ok2=false;
          td2=1;
          td1=0;
        }
      // td_may_tao_am(h);

    }
}
void loop()
{
  Blynk.run();
  sendSensor();
  delay(1000);
 }


 