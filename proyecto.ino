//////////
///LCD///////
#include <LiquidCrystal.h>

#define RS 6
#define EN 7
#define D4 9
#define D5 10
#define D6 11
#define D7 12
/////////

#define BOTON_PULSO 8//boton velocidad
#define BUTTONPin 5//boton heartbeat

#define Pulsador 4//encendido y apagado

////variables de interruptor
int strt = 0;
int estadoAC = 0;
int estadoAN = 0;
/////////
int Pot = 5; // Potenciometro = Pin A5 del Arduino
int valor; // Variable donde almacenamos el valor obtenido del Pot
int bpm;
bool buttonState, previousState, buttonStateh, previousStateh;
unsigned long tiempo, lastTiempo,tiempoh, lastTiempoh;
float sec, inter, vel, c,interh;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup()
{

  lcd.begin(16, 2);
  Serial.begin(9600);

  pinMode(Pulsador, INPUT);
  pinMode(13, OUTPUT); //voltaje lcd
  digitalWrite(13, LOW); //lcd encendido/apagado
  pinMode(BOTON_PULSO, INPUT);//boton velocidad
  pinMode(BUTTONPin, INPUT);//boton heart

  //LEDS de resistencia
  pinMode(A4, OUTPUT); //LED -> SALIDA
  pinMode(A3, OUTPUT); //LED -> SALIDA
  pinMode(A2, OUTPUT); //LED -> SALIDA
  digitalWrite(A4, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A2, LOW);

  lcd.clear();
  lcd.print("km/h");
  lcd.setCursor(7, 0);
  lcd.print("BPM");
  lcd.setCursor(12, 0);
  lcd.print("mi/h");

  previousState = false;

  lastTiempo = 0;

}

void loop()
{
  //Interruptor
  estadoAC = digitalRead(Pulsador);
  if (estadoAC && estadoAN == 0)
  {
    strt = 1 - strt;
    delay(100);
  }
  estadoAN = estadoAC;

  //Cuerpo de instrucciones
  if (strt == 0) {

    //Potenciometros resistencia
    valor = analogRead(Pot) / 4; // Pot -> valor (Analogica)
    // 0 - 1023 /4 -> 0 - 255
    //Serial.println(valor);
    if (valor <= 85)
    {
      digitalWrite(A4, HIGH);
      digitalWrite(A3, LOW);
      digitalWrite(A2, LOW);
      c = 0.5;
    }

    if (valor >= 86 && valor < 170)
    {
      digitalWrite(A4, HIGH);
      digitalWrite(A3, HIGH);
      digitalWrite(A2, LOW);
      c = 1;
    }

    if (valor >= 170)
    {
      digitalWrite(A4, HIGH);
      digitalWrite(A3, HIGH);
      digitalWrite(A2, HIGH);
      c = 1.5;
    }

    //velocidad
    buttonState = !digitalRead(BOTON_PULSO);
    if (previousState == false && buttonState == true)
    {
      tiempo = millis();
      lcd.display();
      lcd.setCursor(0, 1);
      inter = tiempo - lastTiempo;
      sec = inter / 1000;
      vel = c * 3600 * (0.0015 / sec);
      lcd.print(vel);
      lastTiempo = tiempo;
      
      //millas
      lcd.setCursor(12, 1);
      lcd.print(vel / 1.609);
    }
    previousState = buttonState;

    //fotoresis
    int ldr = analogRead(A0);
    //Serial.println(ldr);
    delay(100);
    if (ldr > 800)
    {
      digitalWrite(13, HIGH);
    }
    else
    {
      digitalWrite(13, LOW);
    }

    //Heartbeat
    buttonStateh = !digitalRead(BUTTONPin);
    if (previousStateh == false && buttonStateh == true)
    {
      tiempoh = millis();
      lcd.setCursor(5, 1);
      interh = tiempoh - lastTiempoh;
      lastTiempoh = tiempoh;
      //bpm=60000/interh;
      lcd.print(60000/interh);
    }
    previousStateh = buttonStateh;
    delay(50);
   
  }
  /////else interruptor apagado
  else
  {
    digitalWrite(13, LOW);
    digitalWrite(A4, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(A2, LOW);
    lcd.noDisplay();
  }

  Serial.println(strt);//imprimir el estado del interruptor

}
