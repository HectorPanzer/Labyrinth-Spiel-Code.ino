/*  Name: Jens Gerbert (geje1015)  
 *  Datum: 05.12.2019
 *  Projekt: Labyrinth-Spiel mithilfe 2er Servos und einem 3-Achsen-Gyroskop(MPU6050) steuern
 */

// Einbinden der verschiedenen Bibliotheken
#include <SPI.h>   // Einbinden wichtig, um mit dem MPU6050 kommunizieren zu können
#include <Wire.h>  // Einbinden wichtig, um mit dem MPU6050 kommunizieren zu können
#include <Servo.h> // Einbinden, um die 2 Servos ansteuern zu können
#define MPU 0x68   // I2C Adresse des MPU-6050

// Definiere alle Variablen 
Servo ServoX, ServoY; 
double AcX,AcY,AcZ;
int Pitch, Roll;


void setup(){
  Serial.begin(9600);  // Muss gleich der Baudrate des verwendeten Arduino-Bords sein (max. 115200)
  ServoX.attach(5);    // Servo 1 ist an PIN 8 des Arduino Nano verbunden   
  ServoY.attach(6);    // Servo 2 ist an PIN 9 des Arduino Nano verbunden
  init_MPU();          // Initialisierung des MPU-6050
}
 
void loop()
{
  FunctionsMPU(); // Achsen festlegen AcX, AcY.
    
  Roll = FunctionsPitchRoll(AcX, AcY, AcZ);   // 
  Pitch = FunctionsPitchRoll(AcY, AcX, AcZ);  // 
 
  int ServoRoll = map(Roll, -65, 90, 0, 179);    // Startwinkel und max.Rotationswinkel des Servo-Motors in X-Richtung wird festgelegt
  int ServoPitch = map(Pitch, -115, 90, 179, 0); // Startwinkel und max.Rotationswinkel des Servo-Motors in X-Richtung wird festgelegt
 
  ServoX.write(ServoRoll);  // ServoX ist für "Roll", also für die Neigung nach Rechts und nach Links, verantwortlich 
  ServoY.write(ServoPitch); // ServoY ist für "Roll", also für die Neigung nach Vorne und nach Hinten, verantwortlich
 
 // Ausgabe der Werte im seriellen Monitor
  Serial.print("Pitch: "); 
  Serial.print(Pitch);
  Serial.print("\t");
  Serial.print("Roll: "); 
  Serial.print(Roll);
  Serial.print("\n");
 
}
 
void init_MPU(){
  Wire.begin();
  Wire.beginTransmission(MPU); // Beginnt Übertragung an MPU6050
  Wire.write(0x6B);            // Power-Managment
  Wire.write(0);               // = 0 , um MPU6050 zu starten
  Wire.endTransmission(true);
  delay(500);                  // Zeit, die benötigt wird, um 0 Punkt (0°), auf Servos zu übertragen
}

//Funktion für die Datenaufnahme der X,Y,Z-Achsen des MPU-6050
void FunctionsMPU(){
  Wire.beginTransmission(MPU); 
  Wire.write(0x3B);                 // Startet die Abfrage
  Wire.endTransmission(false);      // Wenn nichts passiert, dann wird auch nichts ausgegeben
  Wire.requestFrom(MPU,6,true);     // Anfrage der 14. Verzeichnisse
  AcX=Wire.read()<<8|Wire.read();   // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L), Speichert ersten 2 Bits in AcX
  AcY=Wire.read()<<8|Wire.read();   // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L), Speichert mittlere 2 Bits in AcY
  AcZ=Wire.read()<<8|Wire.read();   // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L), Speichert letzten 2 Bits in AcZ
}


//Funktion, die einmal "Pitch" und einmal "Roll" berechnen
double FunctionsPitchRoll(double A, double B, double C){
  double DatoA, DatoB, Value;                             
  DatoA = A;                  
  DatoB = (B*B) + (C*C);
  DatoB = sqrt(DatoB);
  
  Value = atan2(DatoA, DatoB); 
  Value = Value * 180/3.14;
  
  return (int)Value;
}
 
