#include <SoftwareSerial.h> // libreria que permite establecer pines digitales

//----- RF24
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10

//Variable con la dirección del canal que se va a leer
byte direccion[5] = {'c', 'a', 'n', 'a', 'l'};

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector para los datos recibidos
float datos[3];
//----- FIN RF24


// ----------- definir led
int PinLED1 = 22; //Sierna
int PinLED2 = 23;
int PinLED3 = 24;
int PinLED4 = 25; //Sierna
int PinLED5 = 26; // adelante izquierda
int PinLED6 = 27; //adelante derecha


// Monster
#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8

//MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6

#define CURRENT_SEN_1 A2
#define CURRENT_SEN_2 A3

#define EN_PIN_1 A0
#define EN_PIN_2 A1

#define MOTOR_1 0
#define MOTOR_2 1
unsigned short usMotor_Status = BRAKE;

// FIN Monster

// Velocidad
short usSpeed = 255;  //default motor speed
short speedMax = 255;
short speed_Med = 160;
short speed_Min = 70;


// ---------- Sensor infrarrojo de obstaculos.
int IR = 33;
int valorSensor;

// lazer
int LAZER = 48;


void setup()
{

  //deficiciones NRF24L01
  radio.begin(); //inicializamos el NRF24L01
  Serial.begin(9600);  //inicializamos el puerto serie
  radio.openReadingPipe(1, direccion); //Abrimos el canal de Lectura
  radio.startListening(); //empezamos a escuchar por el canal
  // FIN NRF24L01

  // LED
  pinMode(PinLED1, OUTPUT);
  pinMode(PinLED2, OUTPUT);
  pinMode(PinLED3, OUTPUT);
  pinMode(PinLED4, OUTPUT);
  pinMode(PinLED5, OUTPUT);
  pinMode(PinLED6, OUTPUT);


  // LAZER
  pinMode(LAZER, OUTPUT);

  // Sensor IR obstaculos
  pinMode(IR, INPUT);

  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(CURRENT_SEN_2, OUTPUT);

  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

  digitalWrite(EN_PIN_1, HIGH);
  digitalWrite(EN_PIN_2, HIGH);


}

void loop() {
  
  uint8_t numero_canal;
  //if ( radio.available(&numero_canal) )
  if ( radio.available() )
  {
    //Leemos los datos y los guardamos en la variable datos[]
    radio.read(datos, sizeof(datos));

    //leemos los datos de cada posición del array
    Serial.print("Dato0= " );
    Serial.print(datos[0]);
    Serial.print("velocidad= " );
    Serial.print(datos[1]);
    Serial.print("milisegundos= " );
    Serial.println(datos[2]);

    if (datos[2] == 0) {
      Stop();  goLucesOff();
    }
    else if (datos[2] == 1) {
      Forward();
      goLucesAdelante();
    }
    else if (datos[2] == 2) {
      Reverse();
      goLucesReversa();
    }
    else if (datos[2] == 3) {
      Left();
      goLucesIzquierda();
    }
    else if (datos[2] == 4) {
      Right();
      goLucesDerecha();
    }
    else if (datos[2] == 5) {
      derForward();
    }
    else if (datos[2] == 6) {
      derReverse();
    }
    else if (datos[2] == 7) {
      izqForward();
    }
    else if (datos[2] == 8) {
      izqReverse();
    }
    else if (datos[2] == 9) {
      danceDance();
    }
    else if (datos[2] == 15) {
      goLuces();
    }
    else if (datos[2] == 14) {
      goLucesOff();
    }
    else if (datos[2] == 15) {
      led_blink();
    }
    else if (datos[2] == 16) {
      goLucesColor();
    }
    else if (datos[2] == 17) {
      goLucesOffColor();
    }
    else if (datos[2] == 10) {
      digitalWrite(LAZER, HIGH);
    }
    else if (datos[0] < 70) {
      setSpeed(datos[0]); // seteo la velocidad 
    }
    else {
      Stop();
      goLucesOff();
    }
    delay(10); // Espera un segundo antes de enviar el siguiente mensaje

  }
  else
  {
    Serial.println("No hay datos de radio disponibles");
  }
  delay(100);
  }

// funciones
int setSpeed(int speed) {
  usSpeed = usSpeed;
  Serial.println(usSpeed);
}

void derForward() {
  Serial.println("derForward");
  motorGo(MOTOR_2, CW, usSpeed);
  motorGo(MOTOR_1, CW, usSpeed / 2);
}
void derReverse() {
  Serial.println("derReverse");
  motorGo(MOTOR_2, CCW, usSpeed);
  motorGo(MOTOR_1, CCW, usSpeed / 2);
}
void izqForward() {
  Serial.println("izqForward");
  motorGo(MOTOR_1, CW, usSpeed);
  motorGo(MOTOR_2, CW, usSpeed / 2);
}
void izqReverse() {
  Serial.println("izqReverse");
  motorGo(MOTOR_1, CCW, usSpeed);
  motorGo(MOTOR_2, CCW, usSpeed / 2);
}
void Forward() {
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}
void Reverse() {
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}
void Left() {
  Serial.println("Left");
  motorGo(MOTOR_1, CW, usSpeed);
  motorGo(MOTOR_2, CCW, usSpeed);
}
void Right() {
  Serial.println("Right");
  motorGo(MOTOR_1, CCW, usSpeed);
  motorGo(MOTOR_2, CW, usSpeed);
}
void Stop() {
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) {        //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);

  if (motor == MOTOR_1) {
    if (direct == CW) {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if (direct == CCW) {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }
    else {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }
    analogWrite(PWM_MOTOR_1, pwm);
  }
  else if (motor == MOTOR_2) {
    if (direct == CW) {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if (direct == CCW) {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);
    }
    else {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);
    }
    analogWrite(PWM_MOTOR_2, pwm);
  }
}
// ----------------- LUCES  -----------------
void led_blink() {
  int var = 0;
  digitalWrite(PinLED1, HIGH);  delay(40);
  digitalWrite(PinLED2, HIGH);  delay(40);
  digitalWrite(PinLED3, HIGH);  delay(40);
  digitalWrite(PinLED4, HIGH);  delay(40);
  digitalWrite(PinLED5, HIGH);  delay(40);
  digitalWrite(PinLED6, HIGH);  delay(40);

  digitalWrite(PinLED1, LOW);  delay(40);
  digitalWrite(PinLED2, LOW);  delay(40);
  digitalWrite(PinLED3, LOW);  delay(40);
  digitalWrite(PinLED4, LOW);  delay(40);
  digitalWrite(PinLED5, LOW);  delay(40);
  digitalWrite(PinLED6, LOW);  delay(40);
  var ++;
}

void goLuces() {
  digitalWrite(PinLED1, HIGH); // adelante derecha
  digitalWrite(PinLED2, HIGH);
  digitalWrite(PinLED3, HIGH); //  atras
  digitalWrite(PinLED4, HIGH); // sirena
  digitalWrite(PinLED5, HIGH); // naranja
  digitalWrite(PinLED6, HIGH);  //adelante izq
}

void goLucesAdelante() {
  digitalWrite(PinLED1, HIGH);  // adelante derecha
  digitalWrite(PinLED2, HIGH); // adelante izquierda
}

void goLucesReversa() {
  digitalWrite(PinLED4, HIGH);  // adelante derecha
  digitalWrite(PinLED6, HIGH); // adelante izquierda
}

void goLucesIzquierda() {
  digitalWrite(PinLED2, HIGH);  // adelante derecha
}

void goLucesDerecha() {
  digitalWrite(PinLED1, HIGH);  // adelante derecha
}

void goLucesOff() {
  digitalWrite(LAZER, LOW);
  digitalWrite(PinLED1, LOW);
  digitalWrite(PinLED2, LOW);
  digitalWrite(PinLED3, LOW);
  digitalWrite(PinLED4, LOW);
  digitalWrite(PinLED5, LOW);
  digitalWrite(PinLED6, LOW);
}

void goLucesColor() {
  digitalWrite(PinLED2, HIGH);
  digitalWrite(PinLED4, HIGH);
}

void goLucesColorSirena() {
  digitalWrite(PinLED3, HIGH);
}

void goLucesOffColor() {
  digitalWrite(PinLED2, LOW);
  digitalWrite(PinLED4, LOW);
}

void danceDance() {
  led_blink();
  Right();
  delay(70);

  led_blink();
  Left();
  delay(70);

  led_blink();
  Stop();
}
