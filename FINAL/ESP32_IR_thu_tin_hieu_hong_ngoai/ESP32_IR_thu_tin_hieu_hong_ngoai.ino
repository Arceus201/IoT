//Các thư viện cần sử dụng
#include <Arduino.h>
//Thư viện này cho phép bạn gửi và nhận tín hiệu hồng ngoại 
//trên ESP8266 hoặc ESP32
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

const uint16_t kRecvPin = 4; // chân Data kết nối với ESP32
const uint32_t kBaudRate = 115200; // tốc độ đọc trên serial Monitor
const uint16_t kCaptureBufferSize = 1024;// kích thước bộ đệm

#if DECODE_AC
//độ trễ tính bằng mili giây khi nhận nhiều gói tin
const uint8_t kTimeout = 50;
#else   
// Các giao thức điều hòa IR thường sử dụng giá trị 20-40ms
const uint8_t kTimeout = 15;
#endif  

// Kích thước tối thiểu của dữ liệu ngẫu nhiên không xác định
//cho phép giảm nhiễu hồng ngoại
const uint16_t kMinUnknownSize = 12;

#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results; // nơi lưu kết  quả

// Đoạn mã này chỉ chạy một lần khi khởi động.
void setup() {
#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else  
  Serial.begin(kBaudRate, SERIAL_8N1);
#endif  
  while (!Serial)  // Đợi kết nối nối tiếp được thiết lập.
    delay(50);
  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
  // Bỏ qua các thông báo có xung bật hoặc tắt nhỏ hơn mức tối thiểu.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  
  irrecv.enableIRIn();  // bắt đầu nhận
}

// Phần lặp lại của mã
void loop() {
  // Kiểm tra xem đã nhận được mã IR chưa.
  if (irrecv.decode(&results)) {
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Kiểm tra xem chúng tôi có nhận được thông báo IR lớn
    // đối với bộ đệm chụp không.
    if (results.overflow)
    // cảnh báo mã IR dài vượt quá bộ đệmc
    // Hãy tăng kích thước bộ đệm
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Hiển thị phiên bản thư viện mà tin nhắn đã được chụp.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
    // Hiển thị đầu ra cơ bản của những gì tìm thấy.
    Serial.print(resultToHumanReadableBasic(&results));
   // Hiển thị bất kỳ thông tin A/C bổ sung nào có.
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield(); 
#if LEGACY_TIMING_INFO
    // Xuất thông tin thời gian RAW kế thừa của kết quả.
    Serial.println(resultToTimingInfo(&results));
    yield();  // Nạp WDT (một lần nữa)
#endif  // LEGACY_TIMING_INFO
  // Xuất kết quả dưới dạng mã nguồn
    Serial.println(resultToSourceCode(&results));
    Serial.println();    // Dòng trống giữa các mục
    yield();             // Nạp WDT (một lần nữa)
  }
}
