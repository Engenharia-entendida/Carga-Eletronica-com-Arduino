//Projeto|Carga eletrônica 
//By Dhanuzio Alves
//Canal Engenharia entendida
//Março 2021
//============================================================================================================================
#include <LiquidCrystal.h>
#define Button 6
#define ledON 5
#define ledOFF 4
#define Control 2
#define Read0 A7
#define Read1 A1
bool Buttonstate = false;
float Current = 00.00;
float Volts = 0.00;
float Watts = 00.00;
float Voltsvalue = 00.00;
float Currentvalue = 0.00;
String State = "";
LiquidCrystal lcd(7,8,9,10,11,12);
void setup() {
lcd.begin(16,2);
pinMode(Control,OUTPUT);
pinMode(Button,INPUT_PULLUP);
pinMode(ledON,OUTPUT);
pinMode(ledOFF,OUTPUT);
digitalWrite(ledON,HIGH);
digitalWrite(ledOFF,LOW);
digitalWrite(Control,LOW);
    lcd.setCursor(2,0);
    lcd.print("Carga Ativa");
    lcd.setCursor(6,1);
    lcd.print("36 W");
    delay(1500);
printlcd();
}
void loop() {
     while(Volts>74){
      Serial.println(Volts);
            lcd.clear();
            lcd.print("  Entrada Acima");
            lcd.setCursor(5,1);
            lcd.print("de 14V");
            digitalWrite(Control,LOW);
            digitalWrite(ledON,HIGH);
            digitalWrite(ledOFF,LOW);
            delay(1500);
            Volts=analogRead(Read0);
            Volts=map(Volts,0,1023.0,0,80.0);
            State="OFF";
            }
   if(digitalRead(Button)==LOW && Buttonstate==false){
      delay(100);
      digitalWrite(Control,HIGH);
      digitalWrite(ledON,LOW);
      digitalWrite(ledOFF,HIGH);
      Buttonstate=true;
      State="ON";
   }
   else if(digitalRead(Button)==LOW && Buttonstate==true){
      delay(100);            
      digitalWrite(Control,LOW);
      digitalWrite(ledON,HIGH);
      digitalWrite(ledOFF,LOW);
      Buttonstate=false;
      State="OFF";
   }
   for(int x=0;x<200;x++){
   Current+=analogRead(Read1);
   }
   Current=Current/200.0;
   for(int i=0;i<200;i++){
   Volts+=analogRead(Read0);
   }
   Volts=Volts/200.0;  
   Volts=map(Volts,0,1023.0,0,80.0);
   Voltsvalue=Volts*0.197;
   Current=map(Current,0,1023.0,0,100.0);
   Currentvalue=Current*0.0338;
   Watts=Currentvalue*Voltsvalue; 
   printlcd();
  }   
void printlcd(){
  lcd.clear();
  lcd.print(" Current:");
    if(Currentvalue<1){
      Currentvalue=Currentvalue*1000;
      lcd.print(Currentvalue,0);
      lcd.print(" mA");
    }
    else if(Currentvalue>=1){
      lcd.print(Currentvalue,2);
      lcd.print("A");
    }
  lcd.setCursor(1,1);
  lcd.print(Voltsvalue,1);
  lcd.print("V");
  lcd.setCursor(7,1);
  lcd.print(Watts,1);
  lcd.print("W");
  lcd.setCursor(13,1);
  lcd.print(State);
}