#include <Arduino.h>
#include <SoftwareSerial.h>

// HC-SR04
#define TRIGGERPIN A0
#define ECHOPIN A1

// L298N
#define R_PWM_PIN 3
#define DIR_3A_PIN 4
#define DIR_4A_PIN 2
#define L_PWM_PIN 6
#define DIR_1A_PIN 7
#define DIR_2A_PIN 5
byte pins[] = { L_PWM_PIN, R_PWM_PIN, DIR_1A_PIN, DIR_2A_PIN, DIR_3A_PIN, DIR_4A_PIN };

// IR
#define RIGHT_IR_PIN 8
#define LEFT_IR_PIN 9

// HM-10 State
#define HM10_STATE_PIN 11

// HC-SR04 variables
const long DISTANCE_TIME_INTERVAL = 100;
unsigned long currentMillis = 0, distancePreviousTime = 0;
const long TRIGGER_LOW_TIME_INTERVAL = 5, TRIGGER_HIGH_TIME_INTERVAL = 10;
unsigned long currentMicros = 0, previousTriggerLowMicros = 0, previousTriggerHighMicros = 0;

// Obstacle avoidance
int DISTANCE_LIMIT = 15;
float distance = 0;
const long OBSTACLE_TIME_INTERVAL = 450;
unsigned long obstaclePreviousTime = 0;

// MH-10 Bluetooth
const long HM10_TIME_INTERVAL = 1000;
unsigned long hm10PreviousTime = 0;
byte hm10State = 0, hm10PrevState = 0, hm10StateIsConnected = -1;
int hm10ReceivedValue = 0;

// Bluetooth Data
char message[9] = { 0 };
int messageIndex = 0, splitIndex = 0;
int xAxisValue = 0, yAxisValue = 0;

// Software Serial for MH-10
SoftwareSerial softSerial(12, 13);  // RX, TX

// Found an obstacle state-machine
typedef enum { 
  STATE_NONE,
  STATE_FOUND, 
  STATE_STOP, 
  STATE_BACKWARDS, 
  STATE_TURN_RIGHT
 } ObstacleStates;

ObstacleStates currentObstacleState = STATE_NONE;

byte pwmValueLeft = 0;
byte enOneValue = LOW;
byte enTwoValue = LOW;
byte pwmValueRight = 0;
byte enThreeValue = LOW;
byte enFourValue = LOW;

void motorAction(
  byte enOnePin, 
  byte enOneValue, 
  byte enTwoPin, 
  byte enTwoValue, 
  byte pwmPin, 
  byte pwmValue
);
void stopMotors();
void hm10ConnectionStatus();
void detectDistance();
void handleForward();
void handleFound();
void handleStop();
void handleBackwards();
void handleTurnRight();
void remoteControlledCar();
void sensorControlledCar();

void setup() {
  // HM-10
  pinMode(HM10_STATE_PIN, INPUT);
  // HC-SR04
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  // L298N
  for (byte pin : pins) {
    pinMode(pin, OUTPUT);
  }
  // IR
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);

  // Serial
  softSerial.begin(9600);
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  hm10ConnectionStatus();
  if (hm10StateIsConnected) {
    if (currentObstacleState != STATE_NONE) {
      currentObstacleState = STATE_NONE;
      stopMotors();
    }

    remoteControlledCar();
  } else {
    detectDistance();
    sensorControlledCar();
  }
}

void motorAction(byte enOnePin, byte enOneValue, byte enTwoPin, byte enTwoValue, byte pwmPin, byte pwmValue) {
  digitalWrite(enOnePin, enOneValue);
  digitalWrite(enTwoPin, enTwoValue);
  analogWrite(pwmPin, pwmValue);
}

void stopMotors() {
  // Serial.println("stopMotors");
  motorAction(DIR_1A_PIN, LOW, DIR_2A_PIN, LOW, L_PWM_PIN, 0);
  motorAction(DIR_3A_PIN, LOW, DIR_4A_PIN, LOW, R_PWM_PIN, 0);
}

void hm10ConnectionStatus() {
  hm10State = digitalRead(11);
  currentMillis = millis();
  if (currentMillis - hm10PreviousTime >= HM10_TIME_INTERVAL) {
    hm10PreviousTime = currentMillis;
    hm10StateIsConnected = hm10State == hm10PrevState;
    hm10PrevState = hm10State;
  }
}

void detectDistance() {
  currentMillis = millis();

  if (currentMillis - distancePreviousTime >= DISTANCE_TIME_INTERVAL) {
    distancePreviousTime = currentMillis;

    currentMicros = micros();
    if (currentMicros - previousTriggerLowMicros >= TRIGGER_LOW_TIME_INTERVAL) {
      previousTriggerLowMicros = currentMicros;
      digitalWrite(TRIGGERPIN, LOW);
    }

    if (currentMicros - previousTriggerHighMicros >= TRIGGER_HIGH_TIME_INTERVAL) {
      previousTriggerHighMicros = currentMicros;
      digitalWrite(TRIGGERPIN, HIGH);
    }

    if (currentMicros - previousTriggerLowMicros >= TRIGGER_LOW_TIME_INTERVAL) {
      previousTriggerLowMicros = currentMicros;
      digitalWrite(TRIGGERPIN, LOW);
    }
    distance = 1 / 29.154 * pulseIn(ECHOPIN, HIGH, 100000) / 2;
  }
}

void handleForward() {
  // Serial.println("goForward");
  motorAction(DIR_1A_PIN, HIGH, DIR_2A_PIN, LOW, L_PWM_PIN, 140);
  motorAction(DIR_3A_PIN, HIGH, DIR_4A_PIN, LOW, R_PWM_PIN, 110);
  
  // Stop left motor while ´LEFT_IR_PIN´ signal is low
  if (digitalRead(RIGHT_IR_PIN) == LOW) {
    motorAction(DIR_1A_PIN, LOW, DIR_2A_PIN, LOW, L_PWM_PIN, 0);
  }

  // Stop right motor while ´RIGHT_IR_PIN´ signal is low
  if (digitalRead(LEFT_IR_PIN) == LOW) {
    motorAction(DIR_3A_PIN, LOW, DIR_4A_PIN, LOW, R_PWM_PIN, 0);
  }
  
  // Add fix to avoid first distance read at 0 ´distance > 0´
  if (distance > 0 && distance <= DISTANCE_LIMIT) {
    // Serial.print(String(distance) + " <= " + String(DISTANCE_LIMIT));
    currentObstacleState = STATE_FOUND;
  }
}

void handleFound() {
  // Serial.print("STATE_FOUND ");
  // Serial.println(distance);
  currentObstacleState = STATE_STOP;
}

void handleStop() {
  stopMotors();

  if (currentMillis - obstaclePreviousTime >= OBSTACLE_TIME_INTERVAL) {
    obstaclePreviousTime = currentMillis;
    currentObstacleState = STATE_BACKWARDS;
  }
  // Serial.print("STATE_STOP ");
  // Serial.println(distance);
}

void handleBackwards() {
  // Serial.println("goBackwards");
  motorAction(DIR_1A_PIN, LOW, DIR_2A_PIN, HIGH, L_PWM_PIN, 150);
  motorAction(DIR_3A_PIN, LOW, DIR_4A_PIN, HIGH, R_PWM_PIN, 150);

  if (currentMillis - obstaclePreviousTime >= OBSTACLE_TIME_INTERVAL) {
    obstaclePreviousTime = currentMillis;
    currentObstacleState = STATE_TURN_RIGHT;
  }
  // Serial.print("STATE_BACKWARDS ");
  // Serial.println(distance);
}

void handleTurnRight() {
  // Serial.println("goRight");
  motorAction(DIR_1A_PIN, LOW, DIR_2A_PIN, HIGH, L_PWM_PIN, 120);
  motorAction(DIR_3A_PIN, HIGH, DIR_4A_PIN, LOW, R_PWM_PIN, 120);
  
  if (currentMillis - obstaclePreviousTime >= OBSTACLE_TIME_INTERVAL) {
    obstaclePreviousTime = currentMillis;
    currentObstacleState = STATE_NONE;
  }
  // Serial.print("STATE_TURN_RIGHT ");
  // Serial.println(distance);
}

void remoteControlledCar() {
  if (softSerial.available() > 0 && messageIndex < 9) {
    message[messageIndex++] = softSerial.read();
  } else if (messageIndex == 9) {
    char* splittedMessage = strtok(message, "-");
    while (splittedMessage != NULL) {
      splitIndex == 0 ? (xAxisValue = String(splittedMessage).toInt()) : (yAxisValue = String(splittedMessage).toInt());
      splittedMessage = strtok(NULL, "-");
      splitIndex += 1;
    }
    memset(message, '\0', 9);
    messageIndex = splitIndex = 0;
    // Serial.println("X: " + String(xAxisValue) + " Y: " + String(yAxisValue)); 
    
    if (yAxisValue < 410) {
        // Serial.println("Forward");
        pwmValueRight = pwmValueLeft = map(yAxisValue, 410, 0, 0, 255);
        // Serial.println(String(yAxisValue) + " " + String(pwmValueRight));
        enOneValue = HIGH;
        enTwoValue = LOW;
        enThreeValue = HIGH;
        enFourValue = LOW;

        if (xAxisValue < 410) {
          // Serial.println("Left");
          pwmValueRight = map(yAxisValue, 410, 0, 0, 255) / 3;
        } else if (xAxisValue > 614) {
          // Serial.println("Right");
          pwmValueLeft = map(yAxisValue, 410, 0, 0, 255) / 3;
        }
    } else if (yAxisValue > 614) {
        // Serial.println("Backwards");
        pwmValueRight = pwmValueLeft = map(yAxisValue, 614, 1023, 0, 255);
        // Serial.println(String(yAxisValue) + " " + String(pwmValueRight));
        enOneValue = LOW;
        enTwoValue = HIGH;
        enThreeValue = LOW;
        enFourValue = HIGH;

        if (xAxisValue < 410) {
          // Serial.println("Left");
          pwmValueRight = map(yAxisValue, 614, 1023, 0, 255) / 3;
        } else if (xAxisValue > 614) {
          // Serial.println("Right");
          pwmValueLeft = map(yAxisValue, 614, 1023, 0, 255) / 3;
        }
    } else if (yAxisValue >= 410 && yAxisValue <= 614 && xAxisValue >= 410 && xAxisValue <= 614) {
      // Serial.println("YX stop");
      pwmValueLeft = 0;
      pwmValueRight = 0;
      enOneValue = LOW;
      enTwoValue = LOW;
      enThreeValue = LOW;
      enFourValue = LOW;
    // } else if (xAxisValue < 255) {
    //   pwmValueLeft = map(yAxisValue, 255, 0, 0, 255);
    //   pwmValueRight = map(yAxisValue, 255, 0, 0, 255);
    //   Serial.println("Just Left");
    //   enOneValue = HIGH;
    //   enTwoValue = LOW;
    //   enThreeValue = LOW;
    //   enFourValue = LOW;
    // } else if (xAxisValue > 767) {
    //   pwmValueLeft = map(yAxisValue, 255, 0, 0, 255);
    //   pwmValueRight = map(yAxisValue, 255, 0, 0, 255);
    //   Serial.println("Just Right");
    //   enOneValue = LOW;
    //   enTwoValue = LOW;
    //   enThreeValue = HIGH;
    //   enFourValue = LOW;
    }

    motorAction(DIR_1A_PIN, enOneValue, DIR_2A_PIN, enTwoValue, L_PWM_PIN, pwmValueLeft);
    motorAction(DIR_3A_PIN, enThreeValue, DIR_4A_PIN, enFourValue, R_PWM_PIN, pwmValueRight);
  }
}

void sensorControlledCar() {
  currentMillis = millis();
  switch (currentObstacleState) {
    case STATE_NONE:
      handleForward();
      break;
    case STATE_FOUND:
      handleFound();
      break;
    case STATE_STOP:
      handleStop();
      break;
    case STATE_BACKWARDS:
      handleBackwards();
      break;
    case STATE_TURN_RIGHT:
      handleTurnRight();
      break;
    }
}