#include <Servo.h>         
#include <MQ7.h>
#include <DHT11.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Servo servoLeft;       
Servo servoRight;

int motor_control = 7;  // 서보모터
Servo servo;

int inputPin = 11;  // 인체 감지 센서

int pirState = LOW; // 센서 초기상태에는 움직임이 없음을 가정

int val = 0;  // 센서 신호의 판별을 위한 변수
int trig = 2;
int echo = 3;

int pin=4;
DHT11 dht11(pin);
LiquidCrystal_I2C lcd(0x27,16,2);   

byte tem[8] = {B00100,B00100,B00100,B00100,B01110,B10001,B10001,B01110};
byte d[8] = {B00100,B01010,B00100,B00000,B00000,B00000,B00000,B00000};
byte du[8] = {B00000,B00000,B10101,B01110,B11111,B01110,B10101,B00000};
byte smile[8] = {B00000,B01010,B01010,B01010,B00000,B11111,B01110,B00100};
byte soso[8] = {B00000,B01010,B01010,B01010,B00000,B11111,B00000,B00000};
byte bad[8] = {B00000,B01010,B01010,B01010,B00000,B01110,B10001,B00000};

MQ7 mq7(A0, 5.0);

void setup()
{ 
  pinMode(10, INPUT); pinMode(9, OUTPUT); //Left IR LED & Receiver
  pinMode(8, INPUT); pinMode(7, OUTPUT); //Right IR LED & Receiver
  delay(1000);               // Delay to finish tone
  
  servoLeft.attach(13);      
  servoRight.attach(12); 
  
  pinMode(inputPin, INPUT);  // 인체감지 센서
  
  Serial.begin(9600);  
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.home();

  lcd.createChar(0, tem);
  lcd.createChar(1, du);
  lcd.createChar(2, smile);
  lcd.createChar(3, soso);
  lcd.createChar(4, bad);
  lcd.createChar(5, d);
  
  servo.attach(motor_control);
  }   
 


void loop()
{
  int err;
  float temp, humi;
  int irLeft = irDetect(9, 10, 41100); 
  int irRight = irDetect(7, 8, 41000);
  int cntRight=7;
  int cntLeft=7;
  int i;
  
  servo.write(0);
  
  val = digitalRead(inputPin);
  
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig,LOW);

  float duration = pulseIn(echo,HIGH);
  float distance = duration*340/2/10000;

  Serial.print(irLeft);
  Serial.println(irRight);
  Serial.print("\nDistance : ");
  Serial.print(distance);
  Serial.println("cm\n");

  if(val == HIGH){
    Serial.print("인체 감지됨");
    for(i=0; i<90;i+=10){
      servo.write(i);
    }
  }
  
  if((irLeft==1)&&(irRight == 1))
  { 
    Serial.print(irLeft);
    Serial.println(irRight);
    Serial.print("\nDistance : ");
    Serial.print(distance);
    Serial.println("cm\n");
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);  // 전진
    delay(380);
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1700); // 우회전
    delay(540);
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);  // 전진
    delay(750);
  }
  
  if((irLeft == 0)&&(irRight=1))
  {
    Serial.print(irLeft);
    Serial.println(irRight);
    Serial.print("\nDistance : ");
    Serial.print(distance);
    Serial.println("cm\n");
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);  // 전진
    delay(75);
    servoLeft.writeMicroseconds(1500);
    servoRight.writeMicroseconds(1300);  // 좌회전
    delay(1150);
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);  // 전진
    delay(50);
  }
  
  if((irLeft==1)&&(irRight==0))
  {
    Serial.print(irLeft);
    Serial.println(irRight);
    Serial.print("\nDistance : ");
    Serial.print(distance);
    Serial.println("cm\n");
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);  // 전진
    if(distance >= 5.5){ 
      servoLeft.writeMicroseconds(1700);
      servoRight.writeMicroseconds(1450);   //우
      delay(3);
    }
    
    else { 
      servoLeft.writeMicroseconds(1550);
      servoRight.writeMicroseconds(1330);   //좌
      delay(3);
    }
    
  }
  
  if((irLeft==0)&&(irRight==0))
  { 

    Serial.print(irLeft);
    Serial.println(irRight);
    Serial.print("\nDistance : ");
    Serial.print(distance);
    Serial.println("cm\n");
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);  // 전진
    delay(75); 
    servoLeft.writeMicroseconds(1500);
    servoRight.writeMicroseconds(1300);  // 좌회전
    delay(1150);
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);  // 전진
    delay(50);
  }
  
  if((err=dht11.read(humi, temp))==0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();
  }
  
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Escape!!");
  
  lcd.setCursor(0,1);
  lcd.write(0);
  lcd.print((int)temp);
  lcd.write(5);
  lcd.print("C");
  lcd.print("  ");
  lcd.write(1);
  lcd.print((int)mq7.getPPM());
  lcd.print("ppm");
  lcd.print(" ");
  
  if((int)mq7.getPPM() > 9){
    lcd.write(4);
  }
  
  if(((int)mq7.getPPM() > 2) && ((int)mq7.getPPM() <= 9)){
    lcd.write(3);
  }
  
  if((int)mq7.getPPM() < 2){
    lcd.write(2);
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
