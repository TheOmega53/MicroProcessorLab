/*Mert Arduino and Raspberry Pi - Line Following Robot*/

//Define Pins

int IN1 = 3; //Enable Pin of the Right Motor (must be PWM)
int IN2 = 4; //Control Pin

int IN3 = 5; //Enable Pin of the Left Motor (must be PWM)
int IN4 = 6; //Control Pin

const int modeButton = 13;

//Speed of the Motors
int ENASpeed = 255;
int ENBSpeed = 255;
int Sensor1Threshhold = 100;
int Sensor2Threshhold = 100;
int Sensor3Threshhold = 100;

int Sensor1 = 0;
int Sensor2 = 0;
int Sensor3 = 0;

enum st{
  None,
  Calibrating,
  Stopped,
  Forward,
  TurnRight,
  TurnLeft,
  Reversed 
  };
  
st status;

void calibrate() {
  Sensor1Threshhold = analogRead(A1) + 150;
  Sensor2Threshhold = analogRead(A2);
  Sensor3Threshhold = analogRead(A3) + 150;
  Serial.println("Calibrating");
}

void setup() {

  Serial.begin(9600); 
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);

  pinMode(modeButton, INPUT_PULLUP);

  calibrate();

}

void loop() {

  //delay(100);

  //Use analogWrite to run motor at adjusted speed
  //  analogWrite(ENA, ENASpeed);
  //  analogWrite(ENB, ENBSpeed);

  //Read the Sensor if HIGH (BLACK Line) or LOW (WHITE Line)
  Sensor1 = analogRead(A1);
  Sensor2 = analogRead(A2);
  Sensor3 = analogRead(A3);

  Serial.println((String)Sensor1+ " " + (String)Sensor2+ " " +(String)Sensor3);

  //Serial.println(analogRead(A3));
  //Serial.println(Threshhold);

  while(digitalRead(modeButton) == LOW){
    calibrate();
  }

//  if(Sensor2 < Threshhold){
    if (Sensor1 >= Sensor1Threshhold && Sensor3 < Sensor3Threshhold){
      //Turn Right
      //motor A Forward
      analogWrite(IN1, 0);
      digitalWrite(IN2, LOW);
  
      //motor B Backward
      analogWrite(IN3, 250);
      digitalWrite(IN4, LOW);
      Serial.println("Pure Right");
    }
    else if (Sensor1 < Sensor1Threshhold && Sensor3 >= Sensor3Threshhold){
      //Turn Left
      //motor A Backward
      analogWrite(IN1, 250);
      digitalWrite(IN2, LOW);
  
      //motor B Forward
      analogWrite(IN3, 0);
      digitalWrite(IN4, LOW);
      Serial.println("Pure Left");
    }
//  } else {
      else if(( Sensor1 < Sensor1Threshhold && Sensor3 < Sensor3Threshhold ) || Sensor2 >= Sensor2Threshhold){
        //Forward    
        analogWrite(IN1, 100);
        digitalWrite(IN2, LOW);
    
        analogWrite(IN3, 150);
        digitalWrite(IN4, LOW);
    
        Serial.println("Forward");
      }
//      else if (Sensor1 >= Sensor1Threshhold && Sensor3 < Sensor3Threshhold){
//        //Turn Right
//        //motor A Forward
//        analogWrite(IN1, 200);
//        digitalWrite(IN2, LOW);
//    
//        //motor B Backward
//        analogWrite(IN3, 100);
//        digitalWrite(IN4, LOW);
//        Serial.println("Steer Right");
//        }
//      else if (Sensor1 < Sensor1Threshhold && Sensor3 >= Sensor3Threshhold){
//        //Turn Left
//        //motor A Backward
//        analogWrite(IN1, 100);
//        digitalWrite(IN2, LOW);
//    
//        //motor B Forward
//        analogWrite(IN3, 200);
//        digitalWrite(IN4, LOW);
//        Serial.println("Steer Left");
//        }
      else if (Sensor1 >= Sensor1Threshhold && Sensor3 >= Sensor3Threshhold){
        //motor A Stop
        analogWrite(IN1, 0);
        digitalWrite(IN2, LOW);
    
        //motor B Stop
        analogWrite(IN3, 0);
        digitalWrite(IN4, LOW);
        Serial.println("Stopped");
      }        
}
