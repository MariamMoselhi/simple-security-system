#include <LiquidCrystal.h>
#include <Arduino.h>
#define A A0
#define B A1
#define C A2
#define D A3
#define flameSensor A4
#define fan A5
#define buzzer A3
const int ledPin = 13;
const int trigPin = 8;
const int echoPin = 7;
const int rs = 12, e = 11, D4 = 5, D5 = 4, D6 = 6, D7 = 2;
LiquidCrystal lcd(rs, e, D4, D5, D6, D7);
bool isFlameDetected = false; // Flame detection status
int flameValue = 0;
int mode = 1;
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(flameSensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop()
{
  if (Serial.available())
  {
    char input = Serial.read();
    if (input == '1')
    {
      mode = 1;
    }
    else if (input == '2')
    {
      mode = 2;
    }
  }

  if (mode == 1)
  {
    // Read flame detector value
    flameValue = digitalRead(flameSensor);

    if (flameValue == HIGH)
    {
      // Flame detected
      isFlameDetected = true;
      tone(buzzer, 1000);
      digitalWrite(fan, HIGH);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Fire Detected");
    }
    else
    {
      // No flame detected
      isFlameDetected = false;
      digitalWrite(buzzer, LOW);
      digitalWrite(fan, LOW);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("No Fire      ");
    }
  }
  if (mode == 2)
  {

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);

    int distance = duration * 0.017;

    if (distance <= 50)
    {

      digitalWrite(ledPin, HIGH);

      tone(buzzer, 1000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Warning!");
      lcd.setCursor(0, 1);
      lcd.print("Object Detected");
    }
    else
    {

      digitalWrite(ledPin, LOW);
      noTone(buzzer);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System Ready");
      delay(500);
    }
  }
}