#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
#define buzz 3
#define ledRed 5

String strID = "";
bool acess = false;


MFRC522 rfid(SS_PIN, RST_PIN);



  void acessoAutorizado(){
    tone(buzz, 500);
    delay(500);
    noTone(buzz);
    delay(100);
  }

  void acessoNegado(){
    tone(buzz, 200);
    digitalWrite(ledRed, HIGH);
    delay(100);
    delay(500);
    noTone(buzz);
    digitalWrite(ledRed, LOW);
    delay(100);
    delay(100);
  }

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Aproxime o cartão...");
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);
  pinMode(buzz, OUTPUT);
  pinMode(ledRed, OUTPUT);
}

void loop() {


    if(!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()){
          delay(100);
          return;
      }


  Serial.println("UID da tag: " + strID);
  
  strID = "";
  byte letra;
      for(byte i = 0; i<rfid.uid.size; i++){
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
        strID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
        strID.concat (String(rfid.uid.uidByte[i], HEX));
        
    }
    strID.toUpperCase();



  if(acess == true){
      acessoAutorizado();
    }
  else{
      acessoNegado();

     delay(1000);
    }


  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
