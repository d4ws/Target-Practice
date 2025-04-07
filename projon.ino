#include <Servo.h>

Servo servo3;
Servo servo5;
Servo servo6;
Servo servo9;
Servo servo10;


const int ldrThreshold = 600; // Lazer vurulma eşiği.
const unsigned long maxGameTime = 20000; // Oyun süresi 100 saniye.

// Motorların en son kaçıncı saniyede açıldıklarını kayıt eden değişkenler.
// Bunu kullanarak hedefin zaman aşımına uğrayıp kapanmasını sağlıyoruz.
unsigned long lastopentime3;
unsigned long lastopentime5;
unsigned long lastopentime6;
unsigned long lastopentime9;
unsigned long lastopentime10;


int skor = 0;
int multiplier = 1;
bool gameOver = false;

// stateler : 0 -> 0 derece. 1 -> 90 derece. 2 -> 180 derece.
// servo3 : 0 derecede kapalı. servo5 : 90 derecede kapalı. ...
int servostate3 = 0; 
int servostate5 = 1; 
int servostate6 = 1; 
int servostate9 = 1; 
int servostate10 = 2; 

int opencount3 = 0;
int opencount5 = 0;
int opencount6 = 0;
int opencount9 = 0;
int opencount10 = 0;


void setup() {
    Serial.begin(9600);
    clearSerialMonitor();

    // Motorları attachla
    servo3.attach(3);
    servo5.attach(5);
    servo6.attach(6);
    servo9.attach(9);
    servo10.attach(10);

    // Motorların ilk konumlarını ayarla (tüm hedefler en başta kapalılar).
    servo3.write(0); 
    servo5.write(90); 
    servo6.write(90); 
    servo9.write(90); 
    servo10.write(180);

    Serial.println("-- GAME START --");
}

void loop() {
  // LDR değerlerini ata.
    int ldrvalue3 = analogRead(A0);
    int ldrvalue5 = analogRead(A1);
    int ldrvalue6 = analogRead(A2);
    int ldrvalue9 = analogRead(A3);
    int ldrvalue10 = analogRead(A4);


    if (gameOver) return; // Oyun bittiyse kodu çalıştırma

    // ASIL OYUN (hangi hedefin kaçıncı saniyede hangi konuma geleceği ve kaç saniye o konumda kalacağı, vurulup vurulmadığı, her şey burada kontrol ediliyor.)
    switch(opencount3) {
      case 0: Trigger3(5000, 2); break;
      case 1: CheckHit3(ldrvalue3, 2500); Trigger3(10000, 1); break;
      case 2: CheckHit3(ldrvalue3, 1500); Trigger3(14000, 2); break;
      case 3: CheckHit3(ldrvalue3, 3500); break;
    }

    switch(opencount5) {
      case 0: Trigger5(5500, 2); break;
      case 1: CheckHit5(ldrvalue5, 2500); Trigger5(10500, 0); break;
      case 2: CheckHit5(ldrvalue5, 1500); Trigger5(14500, 2); break;
      case 3: CheckHit5(ldrvalue5, 3500); break;
    }

    // will be continued...
  
    
    // Oyun süresi dolarsa oyunu bitir.
    if (millis() >= maxGameTime) {
        Serial.println("-- GAME OVER --");
        Serial.print("Your score: ");
        Serial.println(skor);
        gameOver = true;
    }
}

// Kapalı hedefleri açmak için Trigger metodunu kullanıyoruz. Her motorun Trigger metodu kendine özel.
void Trigger3(float triggertime , int wishedstate){
// Servo kapalıysa ve istenilen zaman gelmişse buraya gir
  if(servostate3 == 0 && millis() == triggertime){
    // 90 dereceye getirmek istiyorsan
    if(wishedstate == 1){
        servo3.write(90);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 3 : 90 derecede!");
        servostate3 = 1;
    }
    // 180 dereceye getirmek istiyorsan
    else if(wishedstate == 2){
        servo3.write(180);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 3 : 180 derecede!");
        servostate3 = 2;
    }
    // Hedefin açılma sayısını 1 artır ve açıldığı zamanı kaydet.
    opencount3 += 1;
    lastopentime3 = millis();
  }
}

void Trigger5(float triggertime ,int wishedstate){
  if(servostate5 == 1 && millis() == triggertime){
    // 0 dereceye getirmek istiyorsan
    if(wishedstate == 0){
        servo5.write(0);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 5 : 0 derecede!");
        servostate5 = 0;
    }
    // 180 dereceye getirmek istiyorsan
    else if(wishedstate == 2){
        servo5.write(180);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 5 : 180 derecede!");
        servostate5 = 2;
    }
    opencount5 += 1;
    lastopentime5 = millis();
  }
}

void Trigger6(float triggertime ,int wishedstate){
  if(servostate6 == 1 && millis() == triggertime){
    if(wishedstate == 0){
        servo6.write(0);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 6 : 0 derecede!");
        servostate6 = 0;
    }
    else if(wishedstate == 2){
        servo6.write(180);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 6 : 180 derecede!");
        servostate6 = 2;
    }
    opencount6 += 1;
    lastopentime6 = millis();
  }
}

void Trigger9(float triggertime ,int wishedstate){
  if(servostate9 == 1 && millis() == triggertime){
    if(wishedstate == 0){
        servo9.write(0);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 9 : 0 derecede!");
        servostate9 = 0;
    }
    else if(wishedstate == 2){
        servo9.write(180);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 9 : 180 derecede!");
        servostate9 = 2;
    }
    opencount9 += 1;
    lastopentime9 = millis();
  }
}

void Trigger10(float triggertime ,int wishedstate){
  if(servostate10 == 2 && millis() == triggertime){
    if(wishedstate == 1){
        servo10.write(90);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 10 : 90 derecede!");
        servostate10 = 1;
    }
    else if(wishedstate == 0){
        servo10.write(0);
        Serial.print(millis() / 1000.0);
        Serial.println("-> Hedef 10 : 0 derecede!");
        servostate10 = 0;
    }
    opencount10 += 1;
    lastopentime10 = millis();
  }
}

// Açık olan hedeflerin kapanması için CheckHit metotlarını kullanıyoruz.
// Her motorun metodu kendine özel.
void CheckHit3(int ldr, float duration){
  // Hedef zaten kapalıysa bir şey yapma.
  if(servostate3 == 0) return;

  // Hedef vurulursa
  if (ldr >= ldrThreshold) {
      servo3.write(0); // Hedefi kapat.
      servostate3 = 0; // Son durumunu kaydet.
      skor += 250 * multiplier; 
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 3 vuruldu! " + String(250 * multiplier) + " +puan (Skor : " + String(skor) +")");
      multiplier *= 2;
  }
  // Hedef vurulamadan kapanırsa
  else if (millis() >= lastopentime3 + duration) {
      servo3.write(0); // Hedefi kapat
      servostate3 = 0; // Son durumunu kaydet.
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 3 zaman aşımına uğradı!");
      multiplier = 1; // Hedef kaçarsa katlayıcıyı 1'e çek.
  }
}

void CheckHit5(int ldr, float duration){
  if(servostate5 == 1) return;

  if (ldr >= ldrThreshold) {
      servo5.write(90);
      servostate5 = 1; // Servo5 , 6 ve 9 ; 90 derecedeyken kapalı
      skor += 250 * multiplier;
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 5 vuruldu! " + String(250 * multiplier) + " +puan (Skor : " + String(skor) +")");
      multiplier *= 2;
  }
  else if (millis() >= lastopentime5 + duration) {
      servo5.write(90); 
      servostate5 = 1; // Servo5, 90 derecedeyken kapalı
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 5 zaman aşımına uğradı!");
      multiplier = 1;
  }
}

void CheckHit6(int ldr, float duration){
  if(servostate6 == 1) return;

  // Rehine vurulursa
  if (ldr >= ldrThreshold) {
      servo6.write(90);
      servostate6 = 1; 
      skor -= 2000; // Servo6 sivil.
      multiplier = 1;
      Serial.print(millis() / 1000.0);
      Serial.println("-> Sivil vuruldu! -2000 puan");
  }
  else if (millis() >= lastopentime6 + duration) {
      servo6.write(90);
      servostate6 = 1;
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 6 zaman aşımına uğradı!");
  }
}

void CheckHit9(int ldr, float duration){
  if(servostate9 == 1) return;

  if (ldr >= ldrThreshold) {
      servo9.write(90);
      servostate9 = 1; 
      skor += 250 * multiplier;
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 9 vuruldu! " + String(250 * multiplier) + " +puan (Skor : " + String(skor) +")");
      multiplier *= 2;
  }
  else if (millis() >= lastopentime9 + duration) {
      servo9.write(90);
      servostate9 = 1;
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 9 zaman aşımına uğradı!");
      multiplier = 1;
  }
}

void CheckHit10(int ldr, float duration){
  if(servostate10 == 2) return; // Servo 10 , 180 derecede kapalı.

  if (ldr >= ldrThreshold) {
      servo10.write(180);
      servostate10 = 2;
      skor += 250 * multiplier;
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 10 vuruldu! " + String(250 * multiplier) + " +puan (Skor : " + String(skor) +")");
      multiplier *= 2;
  }
  else if (millis() >= lastopentime10 + duration) {
      servo10.write(180);
      servostate10 = 2;
      Serial.print(millis() / 1000.0);
      Serial.println("-> Hedef 10 zaman aşımına uğradı!");
      multiplier = 1;
  }
}

void clearSerialMonitor() {
    for (int i = 0; i < 20; i++) {  
        Serial.println();  // boş satır yazdır
    }
}
