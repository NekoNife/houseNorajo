#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include <DFRobotHighTemperatureSensor.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

int BuzzerPin = 2;
int LedPin = 3;
int FirePin = 4;
int Humi1Pin = 5;
int Humi2Pin = 6;
int Humi3Pin = 7;
int Humi4Pin = 8;
int MotorAPin = 9;
int MotorBPin = 10;

#define CdsPin A0
#define SoilPin A1
#define TempPin A3



LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED_COUNT 40
Adafruit_NeoPixel LED(LED_COUNT, LedPin, NEO_GRB + NEO_KHZ800);

const float voltageRef = 3.300;
DFRobotHighTemperature PT100 = DFRobotHighTemperature(voltageRef);

int bright = 255;
int humiLevelState = 0;
int humiState = 0;
int LedState = 0;

long OperatingTime = millis();

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  LED.begin();
  LED.show();
  LED.setBrightness(20);

  pinMode(BuzzerPin, OUTPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(FirePin, INPUT);
  pinMode(Humi1Pin, OUTPUT);
  pinMode(Humi2Pin, OUTPUT);
  pinMode(Humi3Pin, OUTPUT);
  pinMode(Humi4Pin, OUTPUT);
  pinMode(MotorAPin, OUTPUT);
  pinMode(MotorBPin, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
 
  int Temp = PT100.readTemperature(TempPin);
  int Soil = analogRead(SoilPin);
  int Cds = analogRead(CdsPin);
  int fireState = digitalRead(FirePin);
  int humiauto = 0;
  Soil = map(Soil, 0, 1023, 100, 0);



  //-----------------------------------------------
//블루투스 받는값
  switch (Serial1.read()) {
    case 'A':
      if (humiState == 0) {
        analogWrite(BuzzerPin , 200);
        delay(100);
        analogWrite(BuzzerPin , 0);
        delay(100);
        humiState++;
      } else if (humiState == 1) {
        analogWrite(BuzzerPin , 200);
        delay(100);
        analogWrite(BuzzerPin , 0);
        delay(100);
        analogWrite(BuzzerPin , 200);
        delay(100);
        analogWrite(BuzzerPin , 0);
        delay(100);
        humiState = 0;
      } else {
        //nothing
      }
      break;
    case 'B':
      if (LedState == 0) {
        analogWrite(BuzzerPin , 200);
        delay(100);
        analogWrite(BuzzerPin , 0);
        delay(100);
        LedState++;
      } else if (LedState == 1) {
        analogWrite(BuzzerPin , 200);
        delay(100);
        analogWrite(BuzzerPin , 0);
        delay(100);
        analogWrite(BuzzerPin , 200);
        delay(100);
        analogWrite(BuzzerPin , 0);
        delay(100);
        LedState = 0;
      } else {
        //nothing
      }
      break;
    case 'C':
      bright = 0;
      break;
    case 'D':
      bright = 64;
      break;
    case 'E':
      bright = 128;
      break;
    case 'F':
      bright = 192;
      break;
    case 'G':
      bright = 255;
      break;
       case 'H':
      humiauto = 0;
      break;
       case 'I':
      humiauto = 1;
      break;
       case 'J':
      humiauto = 2;
      break;
         case 'K':
      humiauto = 3;
      break;
    default:
      break;
  }
  //-----------------------------------------------

//습도 조절 (메인코드)
  if (humiState == 1) {
  if (humiauto == 1) {
  if (Soil <= 75) {
      humiLevelState = 5;
    }else if(Soil >= 85) {
      humiLevelState = 1;
    }else if (Soil >= 99){
      humiLevelState = 0;
      }
  
  }else if (humiauto == 2) {
 if (Soil <= 95) {
      humiLevelState = 5;
    }else if(Soil >= 97) {
      humiLevelState = 1;
    }else if (Soil >= 99){
      humiLevelState = 0;
      }
    
  }else if (humiauto == 3) {
 if (Soil <= 80) {
      humiLevelState = 5;
    }else if(Soil >= 83) {
      humiLevelState = 1;
    }else if (Soil >= 99){
      humiLevelState = 0;
      }

    
  }else{
    //humiState == 0
    if (Temp <= 100) {
      if (Soil <= 70) {
        if (Temp <= 5) {
          humiLevelState = 1;
        } else if (Temp > 5 && Temp <= 15) {
          humiLevelState = 2;
        } else if (Temp > 15 && Temp <= 25) {
          humiLevelState = 3;
        } else if (Temp > 25 && Temp <= 35) {
          humiLevelState = 4;
        } else if (Temp > 35) {
          humiLevelState = 5;
        } else {
          //nothing
        }
      } else {
        humiLevelState = 0;
      }
      Serial1.print("0");
      Serial1.print("/");
      analogWrite(BuzzerPin , 0);
      digitalWrite(MotorAPin , LOW);
      digitalWrite(MotorBPin , LOW);
    } 
    else if (Temp > 100 || fireState == LOW)
    {
      for (int i = 0; i < 8; i++) 
      {
        LED.setPixelColor(i, 255, 0, 0);
        LED.show();
      }
      Serial1.print("1");
      Serial1.print("/");
      analogWrite(BuzzerPin , 200);
      digitalWrite(MotorAPin , HIGH);
      digitalWrite(MotorBPin , LOW);
      digitalWrite(Humi1Pin , HIGH);
      digitalWrite(Humi2Pin , HIGH);
      digitalWrite(Humi3Pin , HIGH);
      digitalWrite(Humi4Pin , HIGH);
      delay(100);
    } 
    else
    {
      for (int i = 0; i < 40; i++)
      {
        LED.setPixelColor(i, 0, 0, 0);
        LED.show();
      }
      Serial1.print("0");
      Serial1.print("/");
      analogWrite(BuzzerPin , 0);
      digitalWrite(MotorAPin , LOW);
      digitalWrite(MotorBPin , LOW);
      digitalWrite(Humi1Pin , LOW);
      digitalWrite(Humi2Pin , LOW);
      digitalWrite(Humi3Pin , LOW);
      digitalWrite(Humi4Pin , LOW);
      delay(100);
    }

  }

 
  } else {
    if (Temp > 100 || fireState == LOW)
    {
      for (int i = 0; i < 40; i++) 
      {
        LED.setPixelColor(i, 255, 0, 0);
        LED.show();
      }
      Serial1.print("1");
      Serial1.print("/");
      analogWrite(BuzzerPin , 200);
      digitalWrite(MotorAPin , HIGH);
      digitalWrite(MotorBPin , LOW);
      digitalWrite(Humi1Pin , HIGH);
      digitalWrite(Humi2Pin , HIGH);
      digitalWrite(Humi3Pin , HIGH);
      digitalWrite(Humi4Pin , HIGH);
      digitalWrite(13, HIGH);
      delay(100);
    }
    else
    {
      for (int i = 0; i < 40; i++) 
      {
        LED.setPixelColor(i, 0, 0, 0);
        LED.show();
      }
      Serial1.print("0");
      Serial1.print("/");
      analogWrite(BuzzerPin , 0);
      digitalWrite(MotorAPin , LOW);
      digitalWrite(MotorBPin , LOW);
      digitalWrite(Humi1Pin , LOW);
      digitalWrite(Humi2Pin , LOW);
      digitalWrite(Humi3Pin , LOW);
      digitalWrite(Humi4Pin , LOW);
      digitalWrite(13, LOW);
      delay(100);
    }
    humiLevelState = 0;
  }

  //-----------------------------------------------
//LED 제어
  if (LedState == 1) {
    if (Cds < 512) {
      for (int i = 0; i < 40; i++)        //red
      {
        LED.setPixelColor(i, bright, bright, bright);
        LED.show();
      }
    } 
    else
    {
      for (int i = 0; i < 40; i++) {
        LED.setPixelColor(i, 0, 0, 0);
        LED.show();
      }
    }
  }
  else
  {
    for (int i = 0; i < 56; i++) {
      LED.setPixelColor(i, 0, 0, 0);
      LED.show();
    }
  }


  //-----------------------------------------------
//온도 습도 단계 표시
  Serial1.print(Temp);
  Serial1.print("/");
  Serial1.print(Soil);
  Serial1.print("/");

  if (humiLevelState == 1) {
    Serial1.print("1");
    Level1();
  } else if (humiLevelState == 2) {
    Serial1.print("2");
    Level2();
  } else if (humiLevelState == 3) {
    Serial1.print("3");
    Level3();
  } else if (humiLevelState == 4) {
    Serial1.print("4");
    Level4();
  } else if (humiLevelState == 5) {
    Serial1.print("5");
    Level5();
  } else {
    Serial1.print("0");
    Level0();
  }
  Serial.print(humiLevelState);
  Serial.print(", ");
  Serial.println(humiState);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("temp: ");
  lcd.print(Temp);
  lcd.print(" ^C");
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(Soil);
  lcd.print(" %");
  delay(1000);
}
//-----------------------------------------------
//습도 1~5단계
void Level1() {
  digitalWrite(Humi1Pin , HIGH);
  digitalWrite(Humi2Pin , HIGH);
  digitalWrite(Humi3Pin , HIGH);
  digitalWrite(Humi4Pin , HIGH);
  delay(1000);
  digitalWrite(Humi1Pin , LOW);
  digitalWrite(Humi2Pin , LOW);
  digitalWrite(Humi3Pin , LOW);
  digitalWrite(Humi4Pin , LOW);
  delay(2500);
}

void Level2() {
  digitalWrite(Humi1Pin , HIGH);
  digitalWrite(Humi2Pin , HIGH);
  digitalWrite(Humi3Pin , HIGH);
  digitalWrite(Humi4Pin , HIGH);
  delay(1000);
  digitalWrite(Humi1Pin , LOW);
  digitalWrite(Humi2Pin , LOW);
  digitalWrite(Humi3Pin , LOW);
  digitalWrite(Humi4Pin , LOW);
  delay(2000);
}

void Level3() {
  digitalWrite(Humi1Pin , HIGH);
  digitalWrite(Humi2Pin , HIGH);
  digitalWrite(Humi3Pin , HIGH);
  digitalWrite(Humi4Pin , HIGH);
  delay(1000);
  digitalWrite(Humi1Pin , LOW);
  digitalWrite(Humi2Pin , LOW);
  digitalWrite(Humi3Pin , LOW);
  digitalWrite(Humi4Pin , LOW);
  delay(1500);
}
void Level4() {
  digitalWrite(Humi1Pin , HIGH);
  digitalWrite(Humi2Pin , HIGH);
  digitalWrite(Humi3Pin , HIGH);
  digitalWrite(Humi4Pin , HIGH);
  delay(1000);
  digitalWrite(Humi1Pin , LOW);
  digitalWrite(Humi2Pin , LOW);
  digitalWrite(Humi3Pin , LOW);
  digitalWrite(Humi4Pin , LOW);
  delay(1000);
}

void Level5() {
  digitalWrite(Humi1Pin , HIGH);
  digitalWrite(Humi2Pin , HIGH);
  digitalWrite(Humi3Pin , HIGH);
  digitalWrite(Humi4Pin , HIGH);
  delay(1000);
  digitalWrite(Humi1Pin , LOW);
  digitalWrite(Humi2Pin , LOW);
  digitalWrite(Humi3Pin , LOW);
  digitalWrite(Humi4Pin , LOW);
  delay(500);
}
void Level0() {
  digitalWrite(Humi1Pin , LOW);
  digitalWrite(Humi2Pin , LOW);
  digitalWrite(Humi3Pin , LOW);
  digitalWrite(Humi4Pin , LOW);
}
