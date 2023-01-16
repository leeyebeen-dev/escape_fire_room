//대피안내로봇
#include <Servo.h>         
#include <SoftwareSerial.h>

Servo servoLeft;       
Servo servoRight;

//대피 안내 관련 핀 번호
int rec = 5;
int playe = 6;

//사이렌 관련 핀 번호
int piezo = 4;

//화재 감지
int flame = 11;
int state = 0;

//블루투스 모듈
int bluetoothTx = 8;
int bluetoothRx = 7;
SoftwareSerial bluetooth(bluetoothTx,bluetoothRx);

void setup()          
{ 
  Serial.begin(9600);

  //적외선
  pinMode(10, INPUT); pinMode(9, OUTPUT);
  pinMode(3, INPUT); pinMode(2, OUTPUT);

  servoLeft.attach(13);    
  servoRight.attach(12);

  //불꽃 감지 센서
  pinMode(flame, INPUT);

  //블루투스
  bluetooth.begin(9600);
}  
 
void loop()                
{ 
  state = digitalRead(flame);
  int irLeft = irDetect(9, 10, 42000);  
  int irRight = irDetect(2, 3, 42000);
  
  if(state == 0){ // 불꽃이 감지 되었을 때
     siren();
     siren();
     delay(100);
     noTone(piezo);
     delay(100);
     digitalWrite(playe, HIGH);
     digitalWrite(playe, LOW);
  }
  if((state > 0) || (state < 0)) { //불꽃이 감지 되지 않을 때
    noTone(piezo);
    delay(100);
  }
  Serial.println("HAha");
  Serial.println(irLeft);
  Serial.println(irRight);

  //블루투스로 조종       
  char cmd;
  if(bluetooth.available())
  {
    cmd = (char)bluetooth.read();
    Serial.print("Command = ");
    Serial.println(cmd);
    if(cmd == '1')
    { 
      Serial.println("OK");  
       //대피 방송 및 주행
       while(1)
       {
         while(1)
         {
          int irLeft = irDetect(9, 10, 42000);  
          int irRight = irDetect(2, 3, 42000);
          if((irLeft == 0) && (irRight == 0))  
          { 
            Serial.println("Sound on");
            digitalWrite(playe, HIGH);
            digitalWrite(playe, LOW);
            delay(100);
            Serial.println("sound OFF");
            backward(1000);            
            turnLeft(1080);
            servoLeft.writeMicroseconds(1500);  
            servoRight.writeMicroseconds(1500); 
            delay(100);
            break;
           }            
           else                
           { 
            forward(1);
            Serial.println("NO");
            Serial.println(irLeft);
            Serial.println(irRight);
            if(cmd == '2'){
              break;
            }         
           } 
         }
        }
      }
    }
    if(cmd == '2')
    {
      servoLeft.writeMicroseconds(1500);  
      servoRight.writeMicroseconds(1500);
      delay(5000);
    }
  }


int irDetect(int irLedPin, int irReceiverPin, long frequency)
{ 
  tone(irLedPin, frequency, 8);   
  delay(1);                   
  int ir = digitalRead(irReceiverPin); 
  delay(1);                
  return ir;    
}      

//사이렌 소리 출력 함수
void siren()
{
  for (int hz = 300; hz <= 750; hz++)
  {
    tone(piezo, hz);
    delay(5);
  }

  for (int hz = 750; hz >= 300; hz--)
  {
    tone(piezo, hz);
    delay(5);
  }
}

//주행 관련 함수
void forward(int time)         
{ 
  servoLeft.writeMicroseconds(1700);  
  servoRight.writeMicroseconds(1300); 
  delay(time);     
} 
               
void turnLeft(int time)              
{ 
  servoLeft.writeMicroseconds(1300); 
  servoRight.writeMicroseconds(1300);
  delay(time);        
}  
         
void backward(int time)      
{ 
  servoLeft.writeMicroseconds(1300); 
  servoRight.writeMicroseconds(1700); 
  delay(time);      
}
