#include <Servo.h>
#define shag 
#define nasos1
#define nasos2
#define nasos3
#define butt1 29
#define butt2 28
#define potm A15
#define trig 7
#define echo 8
Servo servo1;
Servo servo2;
unsigned long time = 0;
int Leds[] = { 22, 23, 24, 25, 26, 27 };
void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(Leds[i], OUTPUT);
  }
  // atach.servo1();
  // attach.servo2();
  pinMode(butt1, INPUT_PULLUP);
  pinMode(butt2, INPUT_PULLUP);
  Serial.begin(9600);
}

float Filtr(float newVal) {//Универсальная фильтрация показаний датчиков
  static float filVal = 0;
  filVal += (newVal - filVal) * 0.6; 
  return int(filVal);
}

void Naliv() {
  int sek = analogRead(potm);
  int sek2;
  if (millis() - time <= 10000) {
    
    if (sek >= analogRead(potm) - 5 && sek <= analogRead(potm) + 5){
        for (int i = 0; i < 6; i++) {
          digitalWrite(Leds[i], 0);
        }
      }
    time = millis();
  }
}

bool flag = 0;
bool butt_mode = 0;
int Button1() {
  bool butt_flag = digitalRead(29);
  if (butt_flag && !flag) {  // обработчик нажатия
    flag = true;
    butt_mode = !butt_mode;  // если надо одиночный то просто удалить эту строку
  }
  if (!butt_flag && flag) {  // обработчик отпускания
    flag = false;
  }
  return butt_mode;
}

bool flag2 = 0;
bool butt_mode2 = 0;
int Button2() {
  bool butt_flag2 = digitalRead(28);
  if (butt_flag2 && !flag2) {  // обработчик нажатия
    flag2 = true;
    butt_mode2 = !butt_mode2;  // если надо одиночный то просто удалить эту строку
  }
  if (!butt_flag2 && flag2) {  // обработчик отпускания
    flag2 = false;
  }
  return butt_mode2;
}
int sem = 0;
bool ZAKAZ[] = { 0, 0, 0, 0, 0, 0 };
//////////////////////////////////////////__________-НАЧАЛО-__________////////////////////////////////////////////////
void loop() {
  int start = analogRead(potm);
  while (start >= analogRead(potm) - 5 && start <= analogRead(potm) + 5) { // Спящий режим
    for (int i = 0; i < 6; i++) {
      digitalWrite(Leds[i], 0);
    }
  }
  int lednum = round(Filtr(analogRead(potm)) / 204); // преобразование значения с потанциометра в номер светодиода
  int led;
  while (Button1() == 1 && Button2() == 1) { 
    int lednum = round(Filtr(analogRead(potm)) / 204);
    for (int i = 0; i < 6; i++) {
      digitalWrite(Leds[i], 0);
    }
    digitalWrite(Leds[lednum], 1);
    led = lednum;
    Serial.println(analogRead(potm));
  }
  delay(100);
  if (Button1() == 0) {
    ZAKAZ[led]++; //добавление напитка в заказ
    sem++;
  } else if (Button2() == 0) {
    ZAKAZ[led] = 0; //удаление напитка из заказа
    sem--;
  }
  butt_mode = 1;
  butt_mode2 = 1;
  Serial.println(constrain(sem, 0, 4));  //Вывод на семисегментник
  Naliv();
  //digitalWrite(23, 1);
   
}