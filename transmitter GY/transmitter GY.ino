#include <Wire.h>      // Biblioteca I2C pentru senzorul GY-521
#include <SPI.h>       // Biblioteca SPI pentru transceiver-ul NRF24L01
#include <nRF24L01.h>  // Biblioteca pentru transceiver-ul NRF24L01
#include <RF24.h>      // Biblioteca pentru transceiver-ul NRF24L01
const int MPU = 0x68;  // Adresa I2C a senzorului GY-521
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
const byte address[6] = "00001";  // ID-ul canalului pentru transmisia datelor prin transceiver
RF24 radio(9, 10);  // Inițializarea transceiver-ului NRF24L01 pe pinii 9 și 10
void setup() {
  radio.begin();  // Inițializarea transceiver-ului NRF24L01
  radio.openWritingPipe(address);  // Deschiderea canalului de transmisie pentru transceiver
  radio.stopListening();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // Inițializarea comunicării I2C pentru senzorul GY-521
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  radio.setChannel(30);  // Setează canalul radio la 30                                  
  radio.setDataRate(RF24_1MBPS);  // Setarea vitezei de transfer a datelor la 1Mbps  
  radio.setPALevel(RF24_PA_MAX);  // Setarea amplificatorului de putere la maxim (0dB)
}
void loop() {
  // Citirea datelor de accelerație și rotație de la senzorul GY-521
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  int16_t AcX = Wire.read() << 8 | Wire.read();
  int16_t AcY = Wire.read() << 8 | Wire.read();
  int16_t AcZ = Wire.read() << 8 | Wire.read();
  int16_t GyX = Wire.read() << 8 | Wire.read();
  int16_t GyY = Wire.read() << 8 | Wire.read();
  int16_t GyZ = Wire.read() << 8 | Wire.read();
  // Vizualizarea datelor pe monitorul serial
  Serial.print("Accelerometer: ");
  Serial.print("X = ");
  Serial.print(AcX);
  Serial.print(" | Y = ");
  Serial.print(AcY);
  Serial.print(" | Z = ");
  Serial.println(AcZ);
  Serial.print("Gyroscope: ");
  Serial.print("X = ");
  Serial.print(GyX);
  Serial.print(" | Y = ");
  Serial.print(GyY);
  Serial.print(" | Z = ");
  Serial.println(GyZ);
  Serial.println(" ");
  // Transmiterea datelor de accelerație și rotație prin transceiver
  uint8_t data[12];
  memcpy(data, &AcX, 2);
  memcpy(data + 2, &AcY, 2);
  memcpy(data + 4, &AcZ, 2);
  memcpy(data + 6, &GyX, 2);
  memcpy(data + 8, &GyY, 2);
  memcpy(data + 10, &GyZ, 2);
  radio.write(data, 12);  // Transmiterea unui pachet de 12 octeți de date
  delay(1000);  // Așteptarea de 1 secundă între citirile de la senzor
}