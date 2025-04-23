#include <Servo.h>

Servo servo3;
Servo servo5;
Servo servo6;
Servo servo9;
Servo servo10;

// Music :: Death by Glamour - Toby Fox
const int ldrThreshold = 600; // Lazer vurulma eşiği.
const unsigned long maxGameTime = 135000; // Oyun süresi (milisaniye)

// Motorların en son kaçıncı saniyede açıldıklarını kayıt eden değişkenler.
// Bunu kullanarak hedefin zaman aşımına uğrayıp kapanmasını sağlıyoruz.
unsigned long lastopentime3;
unsigned long lastopentime5;
unsigned long lastopentime6;
unsigned long lastopentime9;
unsigned long lastopentime10;

// Skor ve skor katlayıcısı.
unsigned long skor = 0;
int multiplier = 1;
bool gameOver = false;

// stateler : 0 -> 0 derece. 1 -> 90 derece. 2 -> 180 derece.
// servo3 : 0 derecede kapalı. servo5 ,6 ve 9 : 90 derecede kapalı. servo10 : 180 derecede kapalı.
int servostate3 = 0; 
int servostate5 = 1; 
int servostate6 = 1; 
int servostate9 = 1; 
int servostate10 = 2; 

// Her bir hedefin kaç kez açıldığını kaydeden değişkenler.
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
    int ldrvalue3 =  analogRead(A0);
    int ldrvalue5 =  analogRead(A1);
    int ldrvalue6 =  analogRead(A2);
    int ldrvalue9 =  analogRead(A3);
    int ldrvalue10 = analogRead(A4);

    if (gameOver) return; // Oyun bittiyse kodu çalıştırma


    // ASIL OYUN (hangi hedefin kaçıncı saniyede hangi konuma geleceği ve kaç saniye o konumda kalacağı, vurulup vurulmadığı, her şey burada kontrol ediliyor.)
    // Burası sadece hedefleri zamanı gelince açmak için.
    // Açılma sayısı tablosu :                          3 -- 5 -- 6 -- 9 -- 10
    switch(millis()) {
      case 6600:  Trigger10(0); break;              //  0 -- 0 -- 0 -- 0 -- 1
      case 9800:  Trigger3(2);  break;              //  1 -- 0 -- 0 -- 0 -- 1
      case 13000: Trigger9(0);  break;              //  1 -- 0 -- 0 -- 1 -- 1
      case 16250: Trigger5(2);  break;              //  1 -- 1 -- 0 -- 1 -- 1
      case 22800: Trigger6(0);  break;              //  1 -- 1 -- 1 -- 1 -- 1
      case 26240: Trigger10(1); Trigger9(0); break; //  1 -- 1 -- 1 -- 2 -- 2
      case 29350: Trigger5(2); Trigger3(1);  break; //  2 -- 2 -- 1 -- 2 -- 2
      case 32500: Trigger10(0); Trigger3(2); break; //  3 -- 2 -- 1 -- 2 -- 3
      case 35750: Trigger9(2); Trigger5(0);  break; //  3 -- 3 -- 1 -- 3 -- 3
      case 42300: Trigger6(2); break;               //  3 -- 3 -- 2 -- 3 -- 3
      case 45500: Trigger10(1); break;              //  3 -- 3 -- 2 -- 3 -- 4
      case 48800: Trigger9(2); break;               //  3 -- 3 -- 2 -- 4 -- 4
      case 52000: Trigger3(2); break;               //  4 -- 3 -- 2 -- 4 -- 4
      case 55200: Trigger5(0); break;               //  4 -- 4 -- 2 -- 4 -- 4
      case 61700: Trigger9(0); break;               //  4 -- 4 -- 2 -- 5 -- 4
      case 62500: Trigger3(2); break;               //  5 -- 4 -- 2 -- 5 -- 4
      case 63350: Trigger5(2); break;               //  5 -- 5 -- 2 -- 5 -- 4
      case 64150: Trigger10(0); break;              //  5 -- 5 -- 2 -- 5 -- 5

      case 68200: Trigger5(0); break;               //  5 -- 6 -- 2 -- 5 -- 5
      case 69000: Trigger3(1); break;               //  6 -- 6 -- 2 -- 5 -- 5
      case 69800: Trigger9(0); Trigger6(0); break;  //  6 -- 6 -- 3 -- 6 -- 5
      case 74700: Trigger5(2); break;               //  6 -- 7 -- 3 -- 6 -- 5 
      case 75500: Trigger9(2); break;               //  6 -- 7 -- 3 -- 7 -- 5 
      case 76300: Trigger3(2); Trigger10(0); break; //  7 -- 7 -- 3 -- 7 -- 6
      case 81200: Trigger10(1); break;              //  7 -- 7 -- 3 -- 7 -- 7
      case 82000: Trigger3(1); break;               //  8 -- 7 -- 3 -- 7 -- 7
      case 82800: Trigger6(2); Trigger5(2); break;  //  8 -- 8 -- 4 -- 7 -- 7
      case 87700: Trigger5(0); break;               //  8 -- 9 -- 4 -- 7 -- 7
      case 88500: Trigger9(0); break;               //  8 -- 9 -- 4 -- 8 -- 7
      case 89300: Trigger6(0); Trigger10(1); break; //  8 -- 9 -- 5 -- 8 -- 8

      case 94200: Trigger9(0); break;               //  8 -- 9 -- 5 -- 9 -- 8
      case 95800: Trigger3(2); break;               //  9 -- 9 -- 5 -- 9 -- 8
      case 97550: Trigger10(1); break;              //  9 -- 9 -- 5 -- 9 -- 9
      case 99000: Trigger3(2); break;               //  10 -- 9 -- 5 -- 9 -- 9
      case 100700: Trigger6(2); break;              //  10 -- 9 -- 6 -- 9 -- 9
      case 102300: Trigger5(0); break;              //  10 -- 10 -- 6 -- 9 -- 9
      case 103900: Trigger9(2); break;              //  10 -- 10 -- 6 -- 10 -- 9
      case 105500: Trigger10(0); break;             //  10 -- 10 -- 6 -- 10 -- 10
      case 107200: Trigger9(1); break;              //  10 -- 10 -- 6 -- 11 -- 10
      case 108800: Trigger6(0); break;              //  10 -- 10 -- 7 -- 11 -- 10
      case 110600: Trigger9(2); break;              //  10 -- 10 -- 7 -- 12 -- 10
      case 112000: Trigger3(2); break;              //  11 -- 10 -- 7 -- 12 -- 10
      case 113700: Trigger9(2); break;              //  11 -- 10 -- 7 -- 13 -- 10
      case 114000: Trigger5(0); break;              //  11 -- 11 -- 7 -- 13 -- 10
      case 114300: Trigger10(0); break;             //  11 -- 11 -- 7 -- 13 -- 11
      case 114500: Trigger3(2); break;              //  12 -- 11 -- 7 -- 13 -- 11
      



    }  


    // Burası hedeflerin kapanmasını sağlamak için.
    switch(opencount3) {
      case 1:  CheckHit3(ldrvalue3, 2500); break;
      case 2:  CheckHit3(ldrvalue3, 32500 - 29350 - 500); break;
      case 3:  CheckHit3(ldrvalue3, 35750 - 32500); break;
      case 4:  CheckHit3(ldrvalue3, 3000); break;
      case 5:  CheckHit3(ldrvalue3, 3850); break;
      case 6:  CheckHit3(ldrvalue3, 3000); break;
      case 7:  CheckHit3(ldrvalue3, 3000); break;
      case 8:  CheckHit3(ldrvalue3, 3000); break;
      case 9:  CheckHit3(ldrvalue3, 1600); break;
      case 10: CheckHit3(ldrvalue3, 1600); break;
      case 11: CheckHit3(ldrvalue3, 1600); break;
      case 12: CheckHit3(ldrvalue3, 1600); break;
    }

    switch(opencount5) {
      case 1:  CheckHit5(ldrvalue5, 2500); break;
      case 2:  CheckHit5(ldrvalue5, 32500 - 29350 - 500); break;
      case 3:  CheckHit5(ldrvalue5, 40000 - 35750); break;
      case 4:  CheckHit5(ldrvalue5, 3000); break;
      case 5:  CheckHit5(ldrvalue5, 3000); break;
      case 6:  CheckHit5(ldrvalue5, 3000); break;
      case 7:  CheckHit5(ldrvalue5, 3000); break;
      case 8:  CheckHit5(ldrvalue5, 3000); break;
      case 9:  CheckHit5(ldrvalue5, 3000); break;
      case 10: CheckHit5(ldrvalue5, 1600); break;
      case 11: CheckHit5(ldrvalue5, 1600); break;
    }

    switch(opencount6) {
      case 1: CheckHit6(ldrvalue6, 2500); break;
      case 2: CheckHit6(ldrvalue6, 2000); break;
      case 3: CheckHit6(ldrvalue6, 3000); break;
      case 4: CheckHit6(ldrvalue6, 3000); break;
      case 5: CheckHit6(ldrvalue6, 3000); break;
      case 6: CheckHit6(ldrvalue6, 1500); break;
      case 7: CheckHit6(ldrvalue6, 1700); break;
    }

    switch(opencount9) {
      case 1:  CheckHit9(ldrvalue9, 2500); break;
      case 2:  CheckHit9(ldrvalue9, 29350 - 26240); break;
      case 3:  CheckHit9(ldrvalue9, 40000 - 35750); break;
      case 4:  CheckHit9(ldrvalue9, 3000); break;
      case 5:  CheckHit9(ldrvalue9, 4650); break;
      case 6:  CheckHit9(ldrvalue9, 3000); break;
      case 7:  CheckHit9(ldrvalue9, 3000); break;
      case 8:  CheckHit9(ldrvalue9, 3000); break;
      case 9:  CheckHit9(ldrvalue9, 1600); break;
      case 10: CheckHit9(ldrvalue9, 1600); break;
      case 11: CheckHit9(ldrvalue9, 1600); break;
      case 12: CheckHit9(ldrvalue9, 1600); break;
      case 13: CheckHit9(ldrvalue9, 1600); break;
    }

    switch(opencount10) {
      case 1:  CheckHit10(ldrvalue10, 2500); break;
      case 2:  CheckHit10(ldrvalue10, 29350 - 26240); break;
      case 3:  CheckHit10(ldrvalue10, 35750 - 32500); break;
      case 4:  CheckHit10(ldrvalue10, 2500); break;
      case 5:  CheckHit10(ldrvalue10, 2200); break;
      case 6:  CheckHit10(ldrvalue10, 3000); break;
      case 7:  CheckHit10(ldrvalue10, 3000); break;
      case 8:  CheckHit10(ldrvalue10, 3000); break;
      case 9:  CheckHit10(ldrvalue10, 1600); break;
      case 10: CheckHit10(ldrvalue10, 1600); break;
      case 11: CheckHit10(ldrvalue10, 1600); break;
    }


  
    
    // Oyun süresi dolarsa oyunu bitir.
    if (millis() >= maxGameTime) {
        Serial.println("-- GAME OVER --");
        Serial.print("Your score: ");
        Serial.println(skor);
        gameOver = true;
    }
}

// Kapalı hedefleri açmak için Trigger metodunu kullanıyoruz. Her motorun Trigger metodu kendine özel.
// Bu metot 2 değişken alıyor. İlk değişken hedefin kaçıncı saniyede açılacağı, ikinci değer ise hangi duruma geleceği (hangi açıya geleceği).
void Trigger3(int wishedstate){
// Servo kapalıysa buraya gir
  if(servostate3 == 0){
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

void Trigger5(int wishedstate){
  if(servostate5 == 1){
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

void Trigger6(int wishedstate){
  if(servostate6 == 1){
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

void Trigger9(int wishedstate){
  if(servostate9 == 1){
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

void Trigger10(int wishedstate){
  if(servostate10 == 2){
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

// Açık olan hedeflerin kapanması için CheckHit metotlarını kullanıyoruz. Her motorun metodu kendine özel.
// Bu metot 2 değişken alıyor. İlki LDR'ye düşen değer, ikinicisi ise hedefin kaç milisaniye açık kaldıktan sonra kapanacağı.
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
