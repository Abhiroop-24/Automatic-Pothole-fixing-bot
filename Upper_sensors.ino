#include <Servo.h>


#define IR_LEFT A0      
#define IR_CL A1        
#define IR_CR A2        
#define IR_RIGHT A3     
const int BLACK_THRESHOLD = 800;  


#define TRIG A4         
#define ECHO A5         
const int DEPTH_THRESHOLD = 5; 


Servo sandGate;
const int SERVO_PIN = 10;  

void setup() {
  Serial.begin(9600);
  
  
  sandGate.attach(SERVO_PIN);
  sandGate.write(0);  
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.println("System Ready (Servo Active, Motors Disabled)");
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

  
  if (irCL && irCR) {  
    int depth = getDepth();
    Serial.print(" | Depth: ");
    Serial.print(depth);
    Serial.print("cm | Action: ");
    
    if (depth < DEPTH_THRESHOLD) {
      Serial.println("CENTRED - DUMPING SAND");
      dumpSand();  
    } else {
      Serial.println("Too shallow - ignoring");
    }
  } 
  else if ((irL && irCL) || (irR && irCR)) {  
    if (irL && irCL) {
      Serial.println(" | Action: HARD LEFT to align (Strong Left detected) (SIMULATED)");
    } else {
      Serial.println(" | Action: HARD RIGHT to align (Strong Right detected) (SIMULATED)");
    }
  }
  else if (irL || irCL) {  
    if (irL && !irCL) {
      Serial.println(" | Action: HARD LEFT to align (Far Left detected) (SIMULATED)");
    } else {
      Serial.println(" | Action: Gentle LEFT to align (SIMULATED)");
    }
  } 
  else if (irR || irCR) {  
    if (irR && !irCR) {
      Serial.println(" | Action: HARD RIGHT to align (Far Right detected) (SIMULATED)");
    } else {
      Serial.println(" | Action: Gentle RIGHT to align (SIMULATED)");
    }
  } 
  else {  
    Serial.println(" | Action: Move FORWARD (SIMULATED)");
  }

  delay(200);
}

int getDepth() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) * 0.034 / 2; // cm
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