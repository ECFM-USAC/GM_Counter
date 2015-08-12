const unsigned long RES = 15;
const unsigned int  PIN = 14;
const unsigned int  PUSH = 5;

void setup()
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, 0);
  pinMode(PUSH, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, 0);
}

unsigned long value = 100;

void loop()
{
  if(!digitalRead(PUSH)){
    value += 10;
    value %= 5000;
    delay(10);
    analogFrequency(value);
    analogResolution(RES);
    analogWrite(PIN, RES/2);
    if(value > 2500 && value < 3100){
      digitalWrite(RED_LED, 1);
    }else{
      digitalWrite(RED_LED, 0);
    }
  }
}
