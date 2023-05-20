#include <Wire.h>

// 자신의 슬레이브 주소를 설정해 줍니다.(슬레이브 주소에 맞게 수정해야 합니다.)
#define SLAVE 3  

// 카운터
byte count = 0; 
char temp;

void setup() {
  // Wire 라이브러리 초기화
  // 슬레이브로 참여하기 위해서는 주소를 지정해야 한다.
  Wire.begin(SLAVE);
  Wire.onReceive(receiveFromMaster);
  // 마스터의 데이터 전송 요구가 있을 때 처리할 함수 등록
  Wire.onRequest(sendToMaster);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop () {
  // 요청이 들어오면 13번 LED를 0.5초간 켜줍니다.
  if (temp == 'o') { 
    play();
  }
}

void receiveFromMaster(int bytes) {
  char ch[2];
  for (int i = 0 ; i < bytes ; i++) {
    // 수신 버퍼 읽기
    ch[i] = Wire.read(); 
  }
  temp = ch[0];
}

void play() { 
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  temp = 0;
}

void sendToMaster() {
  // 자신의 슬레이브 주소를 담은 메세지를 마스터에게 보냅니다. 슬레이브 주소에 맞게 수정해야 합니다.
  Wire.write("3rd Arduino ");
}
