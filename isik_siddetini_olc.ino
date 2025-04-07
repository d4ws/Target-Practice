#include <Servo.h>

Servo servo3;

void setup() {
    Serial.begin(9600);
    clearSerialMonitor();
    //attach
    servo3.attach(3);
    
    // konumları ayarla
    servo3.write(90); 
}

void loop() {
    int ldrvalue3 = analogRead(A0);
    

    Serial.print("Işık şiddeti:");
    Serial.println(ldrvalue3);

}



void clearSerialMonitor() {
    for (int i = 0; i < 15; i++) {  
        Serial.println();  // 50 boş satır yazdır
    }
}
