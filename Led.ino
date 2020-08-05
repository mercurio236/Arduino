int ledPin = 9;    
int analogPin = 3;  
int x = 0;   
float tenSao = 0;    


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(analogPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  

  for(tenSao=0; tenSao<5; tenSao += 0.1){
    x = map(tenSao, 0,5, 0,255);
    analogWrite(9, x);
    delay(20);
  }

  for(tenSao=5; tenSao>0; tenSao -= 0.1){
    x = map(tenSao, 0,5, 0,255);
    analogWrite(9, x);
    delay(10);
  }
  
}
