//library load
#include <LiquidCrystal.h>
#include <dht.h>


// const and globals

#define DHT11_PIN 7
#define led_red 10
#define led_yel 9
#define led_wh 8
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
dht DHT;




//setup
void setup() {
  lcd.begin(16, 2);
  
  pinMode(led_red, OUTPUT); 
  pinMode(led_yel, OUTPUT); 
  pinMode(led_wh, OUTPUT); 
  pinMode(A0, INPUT);

  Serial.begin(9600);
}


void display_led(float t){
  if(t>30){
    digitalWrite(led_red, HIGH);
    digitalWrite(led_yel, LOW);
    digitalWrite(led_wh, LOW);
     
  }
  else if(t<30 && t>10){
    digitalWrite(led_red, LOW);
    digitalWrite(led_yel, HIGH);
    digitalWrite(led_wh, LOW);
  }
 else{
    digitalWrite(led_red, LOW);
    digitalWrite(led_yel, LOW);
    digitalWrite(led_wh, HIGH);
 }
}



//main func
void loop() {
 
  int readData = DHT.read11(DHT11_PIN);
  float t = DHT.temperature;
  float h = DHT.humidity;
  
  display_led(t);

  
  lcd.setCursor(0,0);
  lcd.print("Temp.: ");
  lcd.print(t);
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("Humi.: ");
  lcd.print(h);
  lcd.print(" %");

  delay(5000);
  lcd.clear();
}
