#define DEBOUNCE_THRESH 500
#define CASE_1 // this is ref tooth same as small tooth and aligned
//#define CASE_2 // this is ref tooth same as small tooth but not aligned
//#define CASE_3 // this is ref tooth double size, start aligned
//#define CASE_4 // this is ref tooth double size, start misaligned

long rpm = 6500;
long toothPerCycle = 116;
long toothPerCycleTimes2 = toothPerCycle * 2 - 1;
double fixCoef = 0.94;
long delayMicroSecondsBetweenSmallTooth = (1000000l / (rpm * toothPerCycle * 2l / 60l)) * fixCoef;
int toothCounter = 0;
long lastButtonTime = 0;
boolean changeDetected = false;

void recalculate() {
  toothPerCycleTimes2 = toothPerCycle * 2 - 1;
  delayMicroSecondsBetweenSmallTooth = (1000000l / (rpm * toothPerCycle * 2l / 60l)) * fixCoef;
}

void button() {
  long currentTime = millis();
  if (lastButtonTime < currentTime - DEBOUNCE_THRESH) {
    lastButtonTime = currentTime;
    if (rpm >= 6500) {
      rpm = 800;
    } else {
      rpm += 500;
    }
    recalculate();
  }
  changeDetected = true;
}

void setup() {
  // put your setup code here, to run once:
  DDRB = B11000000;
  PORTB = B11000000;
  toothCounter = 0;
  Serial.begin(9600);
  Serial.println(delayMicroSecondsBetweenSmallTooth);

  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), button, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  delayMicroseconds(delayMicroSecondsBetweenSmallTooth);
  toothCounter++;
#ifdef CASE_1
  if (toothCounter >= toothPerCycleTimes2) {
    PORTB = B11000000;
    toothCounter = -1;
  } else {
    PORTB = ~(PORTB) & B10000000;
  }
#endif
#ifdef CASE_2
  if (toothCounter >= toothPerCycleTimes2 - 1) {
    PORTB = B01000000;
  } else if (toothCounter >= toothPerCycleTime2) {
    PORTB = B10000000;
    toothCounter = -1;
  } else {
    PORTB = ~(PORTB) & B10000000;
  }
#endif
#ifdef CASE_3
  if (toothCounter >= toothPerCycleTimes2) {
    PORTB = B11000000;
    toothCounter = -1;
  } else if (toothPerCycleTime2 == 0) {
    PORTB = B01000000;
  } else if (toothPerCycleTime2 == 1) {
    PORTB = B10000000;
  } else {
    PORTB = ~(PORTB) & B10000000;
  }
#endif
#ifdef CASE_4
  if (toothCounter >= toothPerCycleTimes2) {
    PORTB = B10000000;
    toothCounter = -1;
  } else if (toothPerCycleTime2 == 0) {
    PORTB = B01000000;
  } else if (toothPerCycleTime2 == 1) {
    PORTB = B11000000;
  } else if (toothPerCycleTime2 == 2) {
    PORTB = B00000000;
  } else {
    PORTB = ~(PORTB) & B10000000;
  }
#endif
  if (changeDetected) {
    Serial.println("Change detected");
    Serial.println(rpm);
    changeDetected = false;
  }
}
