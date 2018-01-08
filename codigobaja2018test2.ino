//Temperatura
#include <Thermistor.h>

//Display___________________________________________________________
#include "U8glib.h"
U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7);

//Velocidade________________________________________________________
int pino_D0 = 2;
float rpm =0;
int vel=0;
int velk=0;
volatile byte pulsos = 0;
unsigned long timeold = 0;
//Altere o numero abaixo de acordo com o seu disco encoder
unsigned int pulsos_por_volta = 1;

//Temperatura_______________________________________________________
Thermistor temp(0);                       //Pino anal�gico que o resistor NTC est� conectado.
int temperature=0;

//Combustível_______________________________________________________

int sensor1 = 8;
int sensor2 = 9;
int sensor3 = 10;
int leitura1 = 0;
int leitura2 = 0;
int leitura3 = 0;
int leiturabut = 0;
int led1 = 11;
int led2 = 12;
int led3 = 13;

void setup()
{
  Serial.begin(9600);

  // Velocidade__________________________________________
  
  pinMode(pino_D0, INPUT);
  //Interrupcao 0 - pino digital 2
  //Aciona o contador a cada pulso
  attachInterrupt(0, contador, FALLING);

  // Combustível________________________________________
  
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (sensor3, INPUT);
}

void loop()
{
  ler_vel();
  ler_temp();
  ler_comb();
 vel = (2*3.14*0.27*(rpm/60)*3.6); 
 // picture loop
  u8g.firstPage();  
  do {
    drawVel();
    drawTemp();
    draw();
    drawComb();
  } while( u8g.nextPage() );
  ler_dados();
 
  // rebuild the picture after some delay
  delay(1500);
}

void contador()
{
  //Incrementa contador
  pulsos++;
}

void ler_vel(){
  //Atualiza contador a cada segundo
  if (millis() - timeold >= 1000)
  {
    //Desabilita interrupcao durante o calculo
    detachInterrupt(0);
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos;
    timeold = millis();
    pulsos = 0;
    attachInterrupt(0, contador, FALLING);
  }
    //vel = (2*PI*0.27*(rpm/60)*3.6);
    delay(100);
   
}
void ler_temp(){
temperature = temp.getTemp();
}

void ler_comb(){
  
  leitura1 = digitalRead(sensor1);
  leitura2 = digitalRead(sensor2);
  leitura3 = digitalRead(sensor3);
 /* Serial.print(leitura1);
    Serial.print(leitura2);
      Serial.print(leitura3); */

  
  if (leitura1 == 1){
    digitalWrite(led1, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led2, LOW);
    Serial.println("Tanque cheio");
  }
  else{
    digitalWrite(led1, LOW);
  }
    if (leitura2 == 1){
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led3, LOW);
    Serial.println("Tanque médio");
  }
  else{
    digitalWrite(led2, LOW);
  }
    if (leitura3 == 1){
    digitalWrite(led3, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led1, LOW);
    Serial.println("Tanque na reserva");
  }
  else{
    digitalWrite(led3, LOW);
  }
}

void drawVel(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_fub20);
  u8g.setPrintPos(49,40); 
  // call procedure from base class, http://arduino.cc/en/Serial/Print
  u8g.print(vel);
    u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(49, 55); 
  // call procedure from base class, http://arduino.cc/en/Serial/Print
  u8g.print("km/h");
}

void drawTemp(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_9x15);
  u8g.setPrintPos(90, 11); 
  // call procedure from base class, 
  u8g.print(temperature);
   u8g.setPrintPos(108, 11); 
  // call procedure from base class, 
  u8g.print("C");
}

void drawComb(){
  if (leitura1 == 1) { 
  u8g.setFont(u8g_font_9x15);
  u8g.setPrintPos(49, 90); 
  u8g.print("H");
  }
  if (leitura2 == 1) { 
  u8g.setFont(u8g_font_9x15);
  u8g.setPrintPos(49, 90); 
  u8g.print("M");
}
  if (leitura3 == 1) { 
  u8g.setFont(u8g_font_9x15);
  u8g.setPrintPos(49, 90); 
  u8g.print("L");
  }}
  
void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_4x6);
  u8g.setPrintPos(5, 11); 
  // call procedure from base class, 
  u8g.print("Baja UEA");
}

void ler_dados() {
  Serial.print("RPM = ");
  Serial.println(rpm, DEC);
  Serial.print("vel = ");
  Serial.println(vel, DEC);
  Serial.println("temp= ");
  Serial.println(temperature);
}
