const unsigned long RES = 15;
const unsigned int  PIN = 14;
const unsigned int  PUSH = 5;
const unsigned int  CNT_PIN = 11;

long cnt = 0;
int flag = 0;

void setup()
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, 0);
  pinMode(PUSH, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, 0);
  pinMode(CNT_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Aqui estoy!\n");
  attachInterrupt(CNT_PIN, sendCnt, RISING);
  
}

unsigned long value = 2200;

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
  if(flag){
    Serial.println(++cnt);
    flag = 0;
  }   
}

void sendCnt(void){
  flag = 1;
}
