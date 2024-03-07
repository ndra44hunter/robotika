#include <Servo.h>

// variabel untuk ultrasonik diluar
const int trigPin1 = 3;
const int echoPin1 = 4;

long timer1;
int jarak1;

// variabel untuk ultrasonik di dalam
const int trigPin2 = 6;
const int echoPin2 = 7;
long timer;
int jarak;

// variabel untuk servo
Servo servo1; // servo untuk diluar (membuka pintu luar sampah)
Servo servo2; // servo untuk didalam (menggerakan tempah sampah ke logam dan non logam)

// variabel untuk sensor proximity (cek sensor logam/non logam)
const int sensorPin = 11;

const int resetPin = 2;

const int maxCount=4;

int logamCount=0;
int nonLogamCount=0;

// kondisi tutup luar
bool isTutup=true;

void setup() {
  Serial.begin(9600);
  
  // servo diatur ke posisi default
  servo1.attach(10);
  servo1.write(180);
  delay(100);
  servo2.attach(9);
  servo2.write(90);
  delay(100);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT); 

  pinMode(sensorPin, INPUT);
  pinMode(resetPin, INPUT_PULLUP);

}

void loop() {
  int resetButtonState = digitalRead(resetPin);
  if(resetButtonState==LOW){
    logamCount=0;
    nonLogamCount=0;
  }

  // code ultrasonic luar
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  timer1 = pulseIn(echoPin1, HIGH);
  jarak1 = timer1/58;
  if(jarak1 <= 30){
    Serial.println("Pintu Terbuka");
    // keadaan tutup luar diatur ke "buka"
    isTutup=false;
    // servo1.write(0);
    servo1.write(180);    
    delay(1000);
  } else {
    // keadaan tutup luar diatur ke "tutup"
    Serial.println("Pintu tertutup");
    isTutup=true;
    Serial.println(jarak1);
    servo1.write(45);
    delay(1000);
  }

  // code ultrasonic dalam
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  timer = pulseIn(echoPin2, HIGH);
  jarak = timer/58;

  // lanjut dengan mengecek logam/non logam dengan sensor proximity
  // mendefinisikan sensor proximity
  int cekSampah = digitalRead(sensorPin);
  
  Serial.print("logam ");
  Serial.println(logamCount);
  Serial.print("non logam ");
  Serial.println(nonLogamCount);

  // cek apakah ada benda di "trash checker", cek apakah
  // jaraknya bukan 0, cek apakah jumlah sampah belum penuh,
  // cek apakah tutup luar dalam posisi "buka"
  if(jarak<=10  && !(jarak==0) && !((logamCount>=maxCount) && (nonLogamCount>=maxCount)) && (!isTutup)){
    if(cekSampah == LOW){
      if(!(logamCount>=maxCount)){
        logamCount++;
        Serial.println("Ini sampah logam");
        delay(1000);
        servo2.write(22);
        delay(1000);
        servo2.write(90);
      }
    } else{
      if(!(nonLogamCount>=maxCount)){
        nonLogamCount++;
        Serial.println("Ini sampah non-logam");
        delay(1000);
        servo2.write(158);
        delay(1000);
        servo2.write(90);
      }
    }
  } 
  delay(1000);
}
