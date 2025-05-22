#include <Servo.h>

// ===== IR SENSORS ===== //
#define IR_LEFT A0
#define IR_CL A1
#define IR_CR A2
#define IR_RIGHT A3
const int BLACK_THRESHOLD = 800;

// ===== ULTRASONIC ===== //
#define TRIG A4
#define ECHO A5
const int DEPTH_THRESHOLD = 5; // Minimum pothole depth (cm)

// ===== SERVO ===== //
Servo sandGate;
const int SERVO_PIN = 3;  // SERVO_1 on L293D shield

// ===== MOTOR PINS (L293D) ===== //
// Left motors: M1 (Rear Left), M2 (Front Left)
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
#define ENA 9

// Right motors: M3 (Front Right), M4 (Rear Right)
#define IN5 8
#define IN6 11
#define IN7 12
#define IN8 13
#define ENB 10

void setup() {
  Serial.begin(9600);
  
  // Servo
  sandGate.attach(SERVO_PIN);
  sandGate.write(0);
  
  // Ultrasonic
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT); pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT); pinMode(IN8, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  
  stopMoving();
  Serial.println("System Ready (Servo + Motors Active)");
}

void loop() {
  bool irL = analogRead(IR_LEFT) > BLACK_THRESHOLD;
  bool irCL = analogRead(IR_CL) > BLACK_THRESHOLD;
  bool irCR = analogRead(IR_CR) > BLACK_THRESHOLD;
  bool irR = analogRead(IR_RIGHT) > BLACK_THRESHOLD;

  Serial.print("Sensors: L=");
  Serial.print(irL ? "BLACK " : "WHITE ");
  Serial.print("CL=");
  Serial.print(irCL ? "BLACK " : "WHITE ");
  Serial.print("CR=");
  Serial.print(irCR ? "BLACK " : "WHITE ");
  Serial.print("R=");
  Serial.print(irR ? "BLACK " : "WHITE ");

  if (irCL && irCR && !irL && !irR) {
    int depth = getDepth();
    Serial.print(" | Depth: ");
    Serial.print(depth);
    Serial.print("cm | Action: ");
    if (depth < DEPTH_THRESHOLD) {
      Serial.println("CENTRED - DUMPING SAND");
      stopMoving();
      dumpSand();
    } else {
      Serial.println("Too shallow - ignoring");
      stopMoving();
    }
  } 
  else if (irCL && irCR && irL && irR) {
    Serial.println(" | Action: BACKWARD slightly - Overstepped center (SIMULATED)");
    moveBackward();
  } 
  else if (irCL && irCR && (irL || irR)) {
    Serial.println(" | Action: Slight BACKWARD to re-center (SIMULATED)");
    moveBackward();
  } 
  else if ((irL && irCL) || (irL && !irCL)) {
    Serial.println(" | Action: HARD LEFT to align (SIMULATED)");
    turnLeft();
  } 
  else if ((irR && irCR) || (irR && !irCR)) {
    Serial.println(" | Action: HARD RIGHT to align (SIMULATED)");
    turnRight();
  } 
  else if ((irCL && !irCR) || (!irCL && irCR)) {
    Serial.println(" | Action: ROTATE to center (SIMULATED)");
    turnLeft();  // Picked left by default
  } 
  else {
    Serial.println(" | Action: Move FORWARD - Searching pothole (SIMULATED)");
    moveForward();
  }

  delay(200);
}

int getDepth() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) * 0.034 / 2;
}

void dumpSand() {
  for (int pos = 0; pos <= 180; pos++) {
    sandGate.write(pos);
    delay(15);
  }
  delay(1000);
  for (int pos = 180; pos >= 0; pos--) {
    sandGate.write(pos);
    delay(15);
  }
}

// ===== MOTOR CONTROL FUNCTIONS ===== //
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); // M1
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); // M2
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW); // M3
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW); // M4
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // M1
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // M2
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH); // M3
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH); // M4
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void stopMoving() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
