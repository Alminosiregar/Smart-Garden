#include <DS3231.h>
#define echo 2 
#define trig 3 
DS3231  rtc(SDA, SCL);
Time waktu;
int sensorPin = A2;
int sensorValue = 0;
int humidity = 0;

long durasi;
int jarak; 

const int RELAY_PIN1 = 6;
const int RELAY_PIN2 = 7;
const int RELAY_PIN3 = 8;
void setup() {
  Serial.begin(9600);  
  rtc.begin();
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(trig, OUTPUT);    
  pinMode(echo, INPUT);     
}

void loop() {
  //Declare
  sensorValue = analogRead(sensorPin);
  humidity = convertToPercent(sensorValue);
  waktu = rtc.getTime();

  int dataJam = waktu.hour;     
  int dataMenit = waktu.min;
  int dataDetik = waktu.sec;

  //Ultrasonic
  digitalWrite(trig, LOW);
  delayMicroseconds(8);
  digitalWrite(trig, HIGH);
  delayMicroseconds(8);
  digitalWrite(trig, LOW);
  delayMicroseconds(8);

  durasi = pulseIn(echo, HIGH); 
  jarak = (durasi / 2) / 29.1;  
  Serial.println(jarak);        

  //RTC
  Serial.print(dataJam);
  Serial.print(':');
  Serial.print(dataMenit);
  Serial.print(':');
  Serial.println(dataDetik);

  //Soil Moisture
  Serial.print("Nilai Sensor : ");
  Serial.println(sensorValue);
  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.println("%");

  //Conditions
  if (humidity < 40 && dataJam == 7){
    digitalWrite(RELAY_PIN1, LOW);
  delay(30000);
    }
    else if (humidity < 40 && dataJam == 17){
      digitalWrite(RELAY_PIN1, LOW);
  delay(30000);
      }
     else if (dataJam == 23 && dataMenit >= 00){
      digitalWrite(RELAY_PIN2, LOW);
      }
     else if (dataJam == 0 && dataJam <= 6){
      digitalWrite(RELAY_PIN2, LOW);
      }
      else if (jarak <= 5 ){
      digitalWrite(RELAY_PIN3, LOW);
      delay (20000);
      digitalWrite(RELAY_PIN3, HIGH);
      }
     else {
            digitalWrite(RELAY_PIN1, HIGH);
            digitalWrite(RELAY_PIN2, HIGH);
            digitalWrite(RELAY_PIN3, HIGH);
      }
       delay(1000);
}

//Soil Moisture Convert Percentage
int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 465, 0, 100);
  return percentValue;
}
