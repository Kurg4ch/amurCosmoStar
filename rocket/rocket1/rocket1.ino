#include <VB_MPU9250.h>
#include <VB_BMP280.h>
#include <Servo.h>
#include <SPI.h>
#include <SD.h>


#define tune_pin A1       // пин пьезлэлемента
#define ten_but_pin A3    // пин кнопки 10 минутного отсчета
#define photo_pin A0      // пин фоторезистора
#define servo_space_air_pin 9 // пин сервопривода на выпуск парашюта
#define set_channel_pin 7     // пин смены радиоканала
#define channel_led_pin 2     // пин светодиода для отображения канала связи

VB_MPU9250 imu;       // объявление акселерометра и гироскопа
VB_BMP280 barometer;
Servo Servo1;         // объявление серво

bool imu_connection;                // бул для проверки работоспособности акселерометра и гироскопа
bool barometer_connection;          // бул для проверки работыспособности барометра
bool parachute_opened = false;      // бул для хранения значение открытия парашюта
bool exit_save_height = false;      // вышли ли мы из безопасной зоны
bool on_air = false;                // бул для хранение значения о том в полете мы или нет
bool a = false;

unsigned long save_height;          // время с момента запуска когда мы продолели безопасную зону
unsigned long click_millis;         // время запуска ракеты

float ax;                           //ускорение x
float ay;                           //ускорение y
float az;                           //ускорение z

int photo_value;                    // значение фоторезистора в текущий момент
int max_value = 700;                // максимальное значение для фоторезистора
int alt;                            // значение высоты

void setup() {
  Servo1.attach(servo_space_air_pin);
  Serial1.begin(9600);
  Serial.begin(9600);
  Servo1.attach(servo_space_air_pin);     //активируем серво для возврата в начальное положение
  delay(45);
  Servo1.write(0);
  delay(300);
  Servo1.detach();
  setupSensors();                         // провряем работу датчиков


  if (imu_connection and SD.begin(4)) {
    Serial.print("ALL WORKING");
    tone(tune_pin, 100);
    digitalWrite(10, HIGH);
    delay(1000);
    digitalWrite(10, LOW);
    noTone(A1);
  }
}

void setupSensors() {
  imu_connection   = (imu.begin() == MPU_OK);
  barometer.start_altitude = 100;
  barometer_connection = barometer.begin();
}

void check_sensor() {
  imu.read();
  ax = imu.ax;
  ay = imu.ay;
  az = imu.az;

  barometer.read();
  alt = barometer.alti;
}


void loop() {
  if (on_air == true) {
    write_on_sd();
    send_radio_message();
    check_sensor();
    //Serial.print(channel);
    digitalWrite(set_channel_pin, HIGH);
  }

  if (digitalRead(ten_but_pin) == true) {
    click_millis = millis();
    on_air = true;
  }

  if (millis() - click_millis == 600000) {
    on_air = false;
  }

  photo_value = analogRead(A0);
  Serial.println(photo_value);

  if (photo_value > max_value and on_air == true and parachute_opened == false) {
    
    if (a == false){
      a = true;
      save_height = millis();
      exit_save_height = 1;
    }
      

      if (millis() - save_height >= 1000 and a == true) {
        parachute_opened = true;
        Servo1.attach(9);
        delay(15);
        Servo1.write(180);
        delay(300);
        Servo1.detach();
        delay(15);
      }

    }

  }

  void write_on_sd() {

    File myFile = SD.open("airvalue.txt", FILE_WRITE);

    if (myFile) {
      myFile.print("T6;");       //вывод ID команды
      myFile.print(millis() - click_millis);    //вывод времени полета
      myFile.print(";");
      //  barometer.read();
      myFile.print(alt);         //вывод высоты полета
      myFile.print(";");
      check_sensor();
      myFile.print(ax);          //вывод ускорения по иску
      myFile.print(";");
      myFile.print(ay);          // вывод ускорения по игрику
      myFile.print(";");
      myFile.print(az);          // вывод ускорения по зед
      myFile.print(";");
      myFile.print(parachute_opened);
      myFile.print(";");
      myFile.print(exit_save_height);
      myFile.println("|");
      myFile.close();
    }

  }

  void send_radio_message() {
    Serial1.print("T6;");       //вывод ID команды
    Serial1.print(millis() - click_millis);    //вывод времени полета
    Serial1.print(";");
    barometer.read();
    Serial1.print(alt);         //вывод высоты полета
    Serial1.print(";");
    check_sensor();
    Serial1.print(ax);          //вывод ускорения по иску
    Serial1.print(";");
    Serial1.print(ay);          // вывод ускорения по игрику
    Serial1.print(";");
    Serial1.print(az);          // вывод ускорения по зед
    Serial1.print(";");
    Serial1.print(parachute_opened);
    Serial1.print(";");
    Serial1.print(exit_save_height);
    Serial1.println("|");
    delay(100);
  }
