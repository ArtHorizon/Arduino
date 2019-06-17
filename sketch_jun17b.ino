#include "DHT.h"  //DHT 라이브러리
#define DHTPIN 2        // SDA 핀의 설정
#define DHTTYPE DHT11   // DHT22 센서종류 설정
#include <Servo.h> 
#include <SoftwareSerial.h>

int Raindrops_pin = A0;     // 빗방울센서 핀을 A0으로 설정
int pen = 7;                // 빨간색 LED 핀을 7번핀으로 설정
int sm = 8;
int angle = 0;
int led = 5;


SoftwareSerial BTSerial(11, 12); 
Servo servo; 
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(9600); 
  Serial.println("DHT11 RELAY FAN !!!");
  dht.begin();
  pinMode (pen, OUTPUT);     
  pinMode (led, OUTPUT); 
  servo.attach(sm); 
  BTSerial.begin(9600);
}

void sendData(int value) { //블루투스 모듈을 통해 데이터 전송
String message = String(value) = '\n';
BTSerial.print(message);
}

 
void loop() {
  // 센서의 온도와 습도를 읽어온다.
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  // 불쾌지수
  float di = discomfortIndex(temp,humi); 



  
  
  Serial.println(analogRead(A0));  // 센서 출력값을 시리얼모니터로 전송
  delay(100);                       

  if(analogRead(A0) > 20){    
  
    servo.write(90); 
  }
 
 else {                             // 센서 출력값이 20이상이면 (빗방울이 감지되지 않으면)

 } 
 
  if (isnan(temp) || isnan(humi)) {
    //값 읽기 실패시 시리얼 모니터 출력
    Serial.println("Failed to read from DHT");
  } else {
    //습도, 온도, 불쾌지수 시리얼 모니터 출력
    Serial.print("Humi: "); 
    Serial.print(humi);
    Serial.print(" %\t");
    Serial.print("Temp: "); 
    Serial.print(temp);
    Serial.print(" *C");
    Serial.print(" \t");
    Serial.print("DI: ");
    Serial.println(di);



if(BTSerial.available()){ // 블루투스 데이터 수신
byte data = BTSerial.read();
if(data == '1'){ // 범위 내의 값인 경우
servo.write(0); 
}else if(data =='2'){
  servo.write(90); 
} else if(data =='3') {
  digitalWrite (led, HIGH);
}  else if(data =='4') {
  digitalWrite (led, LOW);
}
}
 
    if(di>=80)
    {
      digitalWrite (pen, HIGH);
    } else {
      digitalWrite (pen, LOW);
    }
  }
  
    BTSerial.print(di);
    

  sendData(1);

}
 
//불쾌지수 구하기 (공식출처: 기상청)
float discomfortIndex(float temp, float humi) {
    return (1.8f*temp)-(0.55*(1-humi/100.0f)*(1.8f*temp-26))+32;
}
