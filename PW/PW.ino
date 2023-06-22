int R_IS = 8; //ungu
int L_IS = 9; //coklat
int R_EN = 6; //abu
int L_EN = 7; //merah
int R_PWM = 4; //putih
int L_PWM = 5; //oren

bool motorOn = false;
unsigned long startTime = 0;
unsigned long duration = 3000;  // Durasi putaran motor (dalam milidetik)

volatile int pulseCount = 0;
unsigned long lastTime;
float currentRPS;

const int ENCODER_PIN_A = 3; //oren
const int ENCODER_PIN_B = 2; //kuning
const float POSITION_TO_MM = 1.0 / 3.0; // Faktor konversi posisi ke milimeter

float currentDistance = 0.0; // Jarak dalam satuan milimeter
int currentPosition = 0; // Posisi saat ini dalam satuan putaran
int count = 0;

void setup() {
  pinMode(R_IS, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_IS, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(L_PWM, OUTPUT);

  Serial.begin(9600);

  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoder, FALLING);
  lastTime = micros();
}
bool balik = false;
void loop() {
  static float lsRPS, deceleration;
  if (!digitalRead(14) && !balik) {
    rotateCW();
    
        Serial.println('1');
        motorOn = true;
  }
  else if (!digitalRead(15) && digitalRead(21)) {
    rotateCCW();
    motorOn = true;
        Serial.println('2');
  }
  else if(!balik) {
        Serial.println('3');
    stopMotor();
        motorOn = false;
  }

  if(motorOn && digitalRead(21)){
//    if (currentRPS <= 48) {
//      rotateCCW();
//      balik = true;
//    }
  }

  if (!digitalRead(21) && balik) {
    balik = false;
    stopMotor();
    motorOn = false;
  }
  if (Serial.available()) {
    char input = Serial.read();

    if (input == 't') {
      rotateCCW();
      motorOn = true;
      startTime = millis();
    } else if (input == 'n') {
      rotateCW();
      motorOn = true;
      startTime = millis();
    }
  }

  //  if (motorOn) {
  //    unsigned long elapsedTime = millis() - startTime;
  //    if (elapsedTime >= duration) {
  //      stopMotor();
  //      motorOn = false;
  //    }
  //
  //  }

  unsigned long currentTime = micros();
  unsigned long elapsedTime = currentTime - lastTime;
  if (elapsedTime >= 50000) { // Delay minimal 50 milidetik / 0.05 detik setiap pembacaan
    lastTime = currentTime;
    int pls = pulseCount;
    currentRPS = (float(pls) * 60000) / elapsedTime;

    currentDistance = currentPosition * POSITION_TO_MM; // Konversi posisi menjadi jarak dalam satuan milimeter
    //    Serial.print("position: ");
    //    Serial.println(currentPosition);
    //    Serial.print(" (");
    //    Serial.print(currentDistance, 2);
    //    Serial.println(" mm)");
    //    Serial.print("RPS: ");
    
    deceleration = currentRPS - lsRPS;
    Serial.println(deceleration);
    lsRPS = currentRPS;
    pulseCount = 0;
  }
}

void rotateCCW() {
  digitalWrite(R_EN, LOW);
  digitalWrite(R_IS, LOW);
  analogWrite(R_PWM, 0);

  digitalWrite(L_EN, HIGH);
  digitalWrite(L_IS, HIGH);
  analogWrite(L_PWM, 255);

  currentPosition = -currentPosition;
}

void rotateCW() {
  digitalWrite(R_EN, HIGH);
  digitalWrite(R_IS, HIGH);
  analogWrite(R_PWM, 255);

  digitalWrite(L_EN, LOW);
  digitalWrite(L_IS, LOW);
  analogWrite(L_PWM, 0);
}

void stopMotor() {
  digitalWrite(R_EN, LOW);
  digitalWrite(R_IS, LOW);
  analogWrite(R_PWM, 0);

  digitalWrite(L_EN, LOW);
  digitalWrite(L_IS, LOW);
  analogWrite(L_PWM, 0);
}

void handleEncoder() {
  if (digitalRead(ENCODER_PIN_A) == digitalRead(ENCODER_PIN_B)) {
    currentPosition++;
  } else {
    currentPosition--;
  }
  pulseCount++;
}
