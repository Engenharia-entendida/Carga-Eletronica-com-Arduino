//===========================================================Configurações iniciais====================================================//
#include <LiquidCrystal.h>                                                    //Biblioteca do display
#include <ResponsiveAnalogRead.h>                                             //Biblioteca para leituras analógicas
LiquidCrystal lcd(3,2,9,10,11,12);                                            //Configura os pinos do display
#define Read5V_Positivo A5                                                    //Leitura corrente +5V 
#define Read5V_Negativo A3                                                    //Leitura corrente -5V
#define Read12V_Positivo A6                                                   //Leitura corrente +12V
#define Read12V_Negativo A4                                                   //Leitura corrente -12V
#define ReadAdj  A2                                                           //Leitura corrente da saída ajustável
#define ReadAdjVolt A1                                                        //Leitura tensão da saída ajustável
#define Lm35 A7                                                               //Pino do Lm35
#define Control0 7                                                            //Relé para saídas ajustável 
#define Control1 8                                                            //Relé para saídas fixas 
#define Fan 4                                                                 //Pino que aciona a ventilação                           
#define AdjON 6                                                               //Botão para saída ajustável                       
#define FixedON 5                                                             //Botão para saídas fixas
String AdjONStatus="OFF";                                                     //Status do botão ADJ
String FixedONStatus="OFF";                                                   //Status do botão Fixed 
String FanStatus="OFF";                                                       //Status do fan    
float Current5V_Positivo=0;                                                   //Corrente em +5V
float Current5V_Negativo=0;                                                   //Corrente em -5V
float Current12V_Positivo=0;                                                  //Corrente em +12V
float Current12V_Negativo=0;                                                  //Corrente em -12V
float Current5V=0;                                                            //Corrente total nas saídas +5V e -5V
float Current12V=0;                                                           //Corrente total da saída de +12V e -12V
float CurrentAdj=0;                                                           //Corrente na saída ajustável
float VoltAdj=0;                                                              //Tensão na saída ajustáve
float Temp=0;                                                                 //Variável para a temperatura interna
int CaseStatus = 0;                                                           //Variável para status do Switch adj
int g=0;
int CaseStatus1 = 0;                                                          //Variável para status do Switch fixed
ResponsiveAnalogRead analog0(Read5V_Positivo, true);                          //Objeto para +5V
ResponsiveAnalogRead analog1(Read5V_Negativo, true);                          //Objeto para -5V
ResponsiveAnalogRead analog3(Read12V_Positivo, true);                         //Objeto para +12V
ResponsiveAnalogRead analog2(Read12V_Negativo, true);                         //Objeto para -12V
ResponsiveAnalogRead analog4(ReadAdj, true);                                  //Objeto para Adj current
ResponsiveAnalogRead analog5(ReadAdjVolt, true);                              //Objeto para Adj Volt
ResponsiveAnalogRead analog6(Lm35, true);                                     //Objeto para lm35
byte SimboloOut[8]={0b00100,0b00100,0b11111,0b00100,0b00100,0b00000,0b11111,0b00000};           //Cria o cunjunto de byte que formará o caracter ±
byte TempSimb0[8]={0b01100,0b10010,0b10010,0b01100,0b00000,0b00000,0b00000,0b00000};            //Cria o cunjunto de byte que formará o caracter °
//===============================================================Setup==================================================================//
void setup() {
  lcd.begin(20,4);                                                            //Inicia o display
  lcd.createChar(1,SimboloOut);                                               //Passa o parâmetro SimboloOut para 1.
  lcd.createChar(2,TempSimb0);                                                //Passa o parâmetro TempSimb0 para 2.
  pinMode(Control0,OUTPUT);                                                   //Controle do relé para saídas ajustáveis
  pinMode(Control1,OUTPUT);                                                   //Controle do relé para saídas fixas
  pinMode(Fan,OUTPUT);                                                        //Controle do fan
  pinMode(AdjON,INPUT_PULLUP);                                                //Botão que libera a saída ajustável
  pinMode(FixedON,INPUT_PULLUP);                                              //Botão que libera as saídas fixas
  digitalWrite(Control1,LOW);                                                 //Matém o relé da saída ajustável desativado
  digitalWrite(Control0,LOW);                                                 //Matém o relé das saídas fixas desativado
  digitalWrite(Fan,LOW);                                                      //Matém o fan desligado
  delay(500);                                                                 //Tempo de acomodação de 500mS
}
//===========================================================Loop principal=============================================================//
void loop() {
  analog0.update();                                                           //Inicia o cálculo de leitura analógica A0
  analog1.update();                                                           //Inicia o cálculo de leitura analógica A1
  analog2.update();                                                           //Inicia o cálculo de leitura analógica A2
  analog3.update();                                                           //Inicia o cálculo de leitura analógica A3
  analog4.update();                                                           //Inicia o cálculo de leitura analógica A4
  analog5.update();                                                           //Inicia o cálculo de leitura analógica A5
  analog6.update();                                                           //Inicia o cálculo de leitura analógica A6  
//===========================================================Saída fixas================================================================//
if(digitalRead(FixedON)==LOW && FixedONStatus=="OFF"){                        //Verifica se o botão foi pressionado e se as saídas fixas estão desligadas
  CaseStatus1 = 1;                                                            //Variável para controle do switch case
  delay(420);                                                                 //Espera 100mS para amenizar o efeito bouncing
  digitalWrite(Control0,HIGH);                                                //Liga as saídas fixas
  FixedONStatus="ON";                                                         //Altera o status para ON 
  }  
if(digitalRead(FixedON)==LOW && FixedONStatus=="ON"){                         //Verifica se o botão foi pressionado e se as saídas fixas estão ligadas
  CaseStatus1 = 0;                                                            //Variável para controle do switch case
  delay(200);                                                                 //Espera 100mS para amenizar o efeito bouncing
  digitalWrite(Control0,LOW);                                                 //Desliga as saídas fixas
  FixedONStatus="OFF";                                                        //Altera o status para OFF          
  }
//===========================================================Saída ajustável============================================================//
if(digitalRead(AdjON)==LOW && AdjONStatus=="OFF"){                            //Verifica se o botão foi pressionado e se a saída ajustável está desligada
  CaseStatus = 1;                                                             //Variável para controle do switch case
  delay(420);                                                                 //Espera 100mS para amenizar o efeito bouncing
  digitalWrite(Control1,HIGH);                                                //Liga a saída
  AdjONStatus="ON";                                                           //Altera o status para ON
  }  
if(digitalRead(AdjON)==LOW && AdjONStatus=="ON"){                             //Verifica se o botão foi pressionado e se a saída ajustável está ligada
  CaseStatus = 0;                                                             //Variável para controle do switch case
  delay(200);                                                                 //Espera 100mS para amenizar o efeito bouncing
  digitalWrite(Control1,LOW);                                                 //Desliga a saída
  AdjONStatus="OFF";                                                          //Altera o status  para OFF      
  }
//===========================================================Análise de status Adj======================================================//
switch (CaseStatus){                                                          //Variável a ser analisada
  case 1:                                                                     //Caso seja 1...
//===============================================================Adj tensão=============================================================//
  VoltAdj=analog5.getValue();                                                 //Faz a leitura analógica já estável tensão Adj 
  VoltAdj=map(VoltAdj,30.0,489,1.26,17.75);                                   //Mapeia o valor de tensão ADJ
//===============================================================Adj corrente===========================================================//
  CurrentAdj=analog4.getValue();                                              //Faz a leitura analógica já estável current Adj
  CurrentAdj=map(CurrentAdj,0.000,300.0,0.000,3);                             //Mapeia o valor do ADC para os valores de corrente
    if(CurrentAdj<0){                                                         //Se a corrente for <0...
      CurrentAdj=0;                                                           //Zera a corrent
    }
  VoltAdj=VoltAdj-(CurrentAdj*0.8);                                           //Subtrai o valor de tensão no Shunt
  break;                                                                      //Para
  case 0:                                                                     //Caso seja 0...
  VoltAdj=0;                                                                  //Tensão será 0
  CurrentAdj=0;                                                               //Corrente será 0
  break;                                                                      //Para
}
//===========================================================Análise de status fixas===================================================//
switch (CaseStatus1){                                                         //Variável a ser analisada
  case 1:                                                                     //Caso seja 1...
  Current5V_Positivo=analog0.getValue();                                      //Faz a leitura analógica já estável +5
  Current5V_Negativo=analog1.getValue();                                      //Faz a leitura analógica já estável -5
  Current12V_Positivo=analog3.getValue();                                     //Faz a leitura analógica já estável +12
  Current12V_Negativo=analog2.getValue();                                     //Faz a leitura analógica já estável -12
  Current5V_Positivo=map(Current5V_Positivo,510.0,413.0,0.0,2.5);             //Mapeia o valor do ADC para os valores de corrente +5V
  Current5V_Negativo=map(Current5V_Negativo,510.0,407.0,0.0,2.5);             //Mapeia o valor do ADC para os valores de corrente -5V
  Current12V_Positivo=map(Current12V_Positivo,513.0,420.0,0.0,2.5);           //Mapeia o valor do ADC para os valores de corrente +12V
  Current12V_Negativo=map(Current12V_Negativo,509.0,408.0,0.0,2.5);           //Mapeia o valor do ADC para os valores de corrente -12V
  
//===========================================================Evitar valores < 0========================================================// 
    if(Current5V_Positivo<0.05){                                              //Se tensão < 0, zera...
      Current5V_Positivo=0;                                                   //Zera a tensão
    }
    if(Current12V_Positivo<0.05){                                             //Se tensão < 0, zera...
      Current12V_Positivo=0;                                                  //Zera a tensão
    }
    if(Current5V_Negativo<0.05){                                              //Se tensão < 0, zera...
      Current5V_Negativo=0;                                                   //Zera a tensão  
    }
    if(Current12V_Negativo<0.05){                                             //Se tensão < 0, zera...
      Current12V_Negativo=0;                                                  //Zera a tensão  
    }
  Current5V=Current5V_Positivo+Current5V_Negativo;                            //Imprime a corrente total ±5V
  Current12V=Current12V_Positivo+Current12V_Negativo;                         //Imprime a corrente total ±12V
  break;                                                                      //Para
  case 0:                                                                     //Caso seja 0...
  Current5V=0;                                                                //Corrente será 0
  Current12V=0;                                                               //Corrente será 0
  break;                                                                      //Para 
} 
//=================================================================Proteção==========================================================//
if(Current5V>2.7||Current12V>2.7||CurrentAdj>3.2){                            //Se alguma corrente estiver acima de 3A...
  digitalWrite(Control0,LOW);                                                 //Desliga as saídas fixas
  digitalWrite(Control1,LOW);                                                 //Desliga a saída ajustável
  lcd.clear();                                                                //Limpa o display
  lcd.setCursor(5,0);                                                         //Seta o cursor
  lcd.print("OUTPUT OFF");                                                    //Printa o status
  AdjONStatus="OFF";                                                          //Altera o status  para OFF  
  FixedONStatus="OFF";                                                        //Status OFF
  lcd.setCursor(2,2);                                                         //Seta o cursor
  lcd.print("Corrente elevada!");                                             //Print para corrente acima de 3A
  VoltAdj=0;                                                                  //Tensão será 0
  CurrentAdj=0;                                                               //Corrente será 0
  Current5V=0;                                                                //Corrente será 0
  Current12V=0;                                                               //Corrente será 0
  CaseStatus=0;                                                               //Muda o status para OFF
  CaseStatus1=0;                                                              //Muda o status para OFF
  delay(2000);                                                                //Espera 3 segundos
}
  delay(100);                                                                 //Delay de 100mS
  print();                                                                    //Chama a função de print
}
//=======================================================Tela e temperatura============================================================//
void print(){                                                                 //Função de print
g++;                                                                          //Tempo para atualizar o display
if(g>8){                                                                      //Se g maior que 8...
  Temp=analog6.getValue();                                                    //Faz a leitura analógica já estável lm35
  Temp=Temp*0.5;                                                              //Converte para graus
  if(Temp>35){                                                                //Se for maior que 35 °C...
    digitalWrite(Fan,HIGH);                                                   //Liga o fan
    FanStatus="ON";                                                           //Status do fan
  }       
  else{                                                                       //Caso não...
    digitalWrite(Fan,LOW);                                                    //Desliga o fan
    FanStatus="OFF";                                                          //Status do fan
  }
g=0;                                                                          //Zera a variáel g
  }                                                     
  lcd.clear();                                                                //Limpa o display
//=======================================================Tratamento para A e mA ±5=====================================================//
  lcd.print("CH1|");                                                          //Print do canal
  lcd.write(1);                                                               //Escreve o caracter especial ±
  lcd.print("5V:");                                                           //Print da corrent+5V e -5V
  lcd.setCursor(11,0);                                                        //Seta o cursor  
  if(Current5V<1){                                                            //Se corrente for menos que 1A...
    Current5V=Current5V*1000;                                                 //Exibirá em mili.                                               
    lcd.print(Current5V,0);                                                   //Imprime a tensão da saída ajustável
    lcd.print("mA");                                                          //Em mA
  }
  else{                                                                       //Se não...
    lcd.print(Current5V,2);                                                   //Imprime a corrente da saída ajustável
    lcd.print("A");                                                           //Em A
  } 
 //=====================================================Tratamento para A e mA ±12=====================================================//
  lcd.setCursor(0,1);                                                         //Seta o cursor
  lcd.print("CH2|");                                                          //Print do canal
  lcd.write(1);                                                               //Escreve o caracter especial ±
  lcd.print("12V:");                                                          //Print da corrent+5V e -5V
  lcd.setCursor(11,1);                                                        //Seta o cursor
  if(Current12V<1){                                                           //Se corrente for menos que 1A...
    Current12V=Current12V*1000;                                               //Exibirá em mili.                                               
    lcd.print(Current12V,0);                                                  //Imprime a tensão da saída ajustável
    lcd.print("mA");                                                          //Em mA
  }
  else{                                                                       //Se não...
    lcd.print(Current12V,2);                                                  //Imprime a corrente da saída ajustável
    lcd.print("A");                                                           //Em A
  } 
 //===================================================Tratamento status e Tensão Adj===================================================//                   
 lcd.setCursor(17,0);                                                         //Seta o cursor
 lcd.print(FixedONStatus);                                                    //Exibe o status da saída fixa
 lcd.setCursor(17,1);                                                         //Seta o cursor
 lcd.print(FixedONStatus);                                                    //Exibe o status da saída fixa
 lcd.setCursor(0,2);                                                          //Seta o cursor
 lcd.print("Adj|");                                                           //Print do canal
 lcd.print(float(VoltAdj)+String ("V  "));                                    //Imprime a tensão da saída ajustável 
 //=======================================================Tratamento para A e mA=======================================================//
 lcd.setCursor(11,2);                                                         //Seta o cursor
  if(CurrentAdj<1){                                                           //Se corrente for menos que 1A...
    CurrentAdj=CurrentAdj*1000;                                               //Exibirá em mili.                                               
    lcd.print(CurrentAdj,0);                                                  //Imprime a tensão da saída ajustável
    lcd.print("mA");                                                          //Em mA
  }
  else{                                                                       //Se não...
    lcd.print(CurrentAdj,2);                                                  //Imprime a corrente da saída ajustável
    lcd.print("A");                                                           //Em A
  }
 lcd.setCursor(17,2);                                                         //Seta o cursor
 lcd.print(AdjONStatus);                                                      //Exibe o status da saída ajustável
 //=======================================================Tratamento para Temperatura==================================================//
 lcd.setCursor(0,3);                                                          //Seta o cursor
 lcd.print("T:");                                                             //Imprime C
 lcd.print(Temp,1);                                                           //Imprime temperatura
 lcd.write(2);                                                                //Imprime o caracter °
 lcd.print("C");                                                              //Imprime C
 lcd.setCursor(13,3);                                                         //Seta o cursor
 lcd.print("FAN:");                                                           //Imprime fan
 lcd.print(FanStatus);                                                        //Imprime C
 delay(200);                                                                  //Delay de 200mS
}
//=========================================================Configura a função 'mapf"===================================================//
float map(float x, float in_min, float in_max, float out_min, float out_max){ //Range de valores mínimos e máximos
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;      //Retorna os valores como float
}



