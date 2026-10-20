// @author Dženardas Jevič
// Themed Task: car turn signal

#include <EEPROM.h>



// buttons, pins, leds
int leftButton = 2;
int rightButton = 3;
int setButton = 4;
int leftLED = 6;
int rightLED = 5;
int buzzerPIN = 8;

int state = 0; // 0-off, 1-left, 2-right

// blinking
bool ledOn = false;

// changed in interrupts
volatile bool leftPressed = false;
volatile bool rightPressed = false;
volatile bool blinkFlag = false;
volatile long ticks = 0;

int VERSION = 1;  // signature in EEPROM (address 0)

volatile int speed = 30; // 30=300ms, 60=600ms
int buzzerTone = 2000; // frequency Hz

// button debounce
long debounceDelay = 50;
volatile long lastLeftTime = 0;
volatile long lastRightTime = 0;
long lastSetTime = 0;
int setState = HIGH;
int lastSetReading = HIGH;




// left button interrupt
void leftISR()
{
  leftPressed = true;
  lastLeftTime = millis();
}

// right button interrupt
void rightISR()
{
  rightPressed = true;
  lastRightTime = millis();
}

// timer1 interrupt, every 10 ms
ISR(TIMER1_COMPA_vect)
{
  ticks++;
  if(ticks % speed == 0){
    blinkFlag = true;
  }
}




void setup()
{
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(setButton, INPUT_PULLUP);
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(buzzerPIN, OUTPUT);
  
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(leftButton), leftISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(rightButton), rightISR, FALLING);

  // EEPROM.write(0, VERSION); // test
  // EEPROM.write(1, 45); // test

  // check EEPROM
  if(EEPROM.read(0) == VERSION){
    speed = EEPROM.read(1);
    buzzerTone = EEPROM.read(2) * 10;
    if((speed != 30 && speed != 60) || (buzzerTone != 1000 && buzzerTone != 2000)){
      speed = 30;
      buzzerTone = 2000;
      EEPROM.update(1, speed);
      EEPROM.update(2, buzzerTone / 10);
    }
  } else{
    EEPROM.update(0, VERSION);
    EEPROM.update(1, speed);
    EEPROM.update(2, buzzerTone / 10);
  }
  
  Serial.print("speed: ");
  Serial.println(speed);
  Serial.print("tone: ");
  Serial.println(buzzerTone);
  
  // timer1, interrupt every 10 ms
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
  OCR1A = 2499;
  TIMSK1 = (1 << OCIE1A);
  interrupts();
}




void loop()
{
  // left button
  if(leftPressed == true && (millis() - lastLeftTime) > debounceDelay){
    Serial.println("left pressed");
    leftPressed = false;
    if(digitalRead(leftButton) == LOW){
      if(state == 1){
        state = 0;
      }  else{
        state = 1;
      }
      ledOn = false;
    }
  }

  // right button
  if(rightPressed == true && (millis() - lastRightTime) > debounceDelay){
    Serial.println("right pressed");
    rightPressed = false;
    if(digitalRead(rightButton) == LOW){
      if(state == 2){
        state = 0;
      }  else{
        state = 2;
      }
      ledOn = false;
    }
  }
  
  // set button
  int setReading = digitalRead(setButton);
  if(setReading != lastSetReading){
    lastSetTime = millis();
  }
  if((millis() - lastSetTime) > debounceDelay){
    if(setReading != setState){
      setState = setReading;
      if(setState == LOW){
        if(speed == 30){
          speed = 60;
          buzzerTone = 1000;
        } else{
          speed = 30;
          buzzerTone = 2000;
        }
        EEPROM.update(1, speed);
        EEPROM.update(2, buzzerTone / 10);
        
        Serial.print("saved speed: ");
        Serial.println(speed);
        Serial.print("saved tone: ");
        Serial.println(buzzerTone);
      }
    }
  }
  lastSetReading = setReading;

  // change LED on/off
  if(blinkFlag == true){
    blinkFlag = false;
    ledOn = !ledOn;
    // Serial.println(millis()); // test
  }

  // everything off
  if(state == 0){
    digitalWrite(leftLED, LOW);
    digitalWrite(rightLED, LOW);
    noTone(buzzerPIN);
  }

  // left signal
  if(state == 1){
    digitalWrite(rightLED, LOW);
    if(ledOn){
      digitalWrite(leftLED, HIGH);
      tone(buzzerPIN, buzzerTone);
    } else{
      digitalWrite(leftLED, LOW);
      noTone(buzzerPIN);
    }
  }

  // right signal
  if(state == 2){
    digitalWrite(leftLED, LOW);
    if(ledOn){
      digitalWrite(rightLED, HIGH);
      tone(buzzerPIN, buzzerTone);
    } else{
      digitalWrite(rightLED, LOW);
      noTone(buzzerPIN);
    }
  }
}