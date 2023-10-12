#include <LiquidCrystal_I2C.h>
#include <SPI.h>       // Biblioteca SPI pentru transceiver-ul NRF24L01
#include <nRF24L01.h>  // Biblioteca pentru transceiver-ul NRF24L01
#include <RF24.h>      // Biblioteca pentru transceiver-ul NRF24L01
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
const byte address[6] = "00001";

RF24 radio(9, 10);  // Inițializarea transceiver-ului NRF24L01 pe pinii 9 și 10

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

void setup() {
  lcd.init();
  lcd.backlight();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setChannel(30);                      
  radio.setPALevel(RF24_PA_MAX);               
  radio.setDataRate(RF24_1MBPS);             
  radio.startListening();
  Serial.begin(9600);
  lcd.begin(20, 4);  // Inițializarea display-ului LCD 20x4
  lcd.clear();
}

void loop() {
  if (radio.available()) {  // Verificarea disponibilității datelor transmise prin transceiver
    uint8_t data[12];       // Buffer pentru
    radio.read(data, 12);   // Recepționarea unui pachet de 12 octeți de date prin transceiver
    int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
    memcpy(&AcX, data, 2);
    memcpy(&AcY, data + 2, 2);
    memcpy(&AcZ, data + 4, 2);
    memcpy(&GyX, data + 6, 2);
    memcpy(&GyY, data + 8, 2);
    memcpy(&GyZ, data + 10, 2);
    // Afișarea datelor de accelerație și rotație pe display-ul LCD
    lcd.setCursor(0, 0);
    lcd.print("Ax=");
    lcd.setCursor(3, 0);
    lcd.print(AcX);
    lcd.setCursor(0, 1);
    lcd.print("Ay=");
    lcd.setCursor(3, 1);
    lcd.print(AcY);
    lcd.setCursor(0, 2);
    lcd.print("Az=");
    lcd.setCursor(3, 2);
    lcd.print(AcZ);
    lcd.setCursor(10, 0);
    lcd.print("Gx=");
    lcd.setCursor(13, 0);
    lcd.print(GyX);
    lcd.setCursor(10, 1);
    lcd.print("Gy=");
    lcd.setCursor(13, 1);
    lcd.print(GyY);
    lcd.setCursor(10, 2);
    lcd.print("Gz=");
    lcd.setCursor(13, 2);
    lcd.print(GyZ);
  }
}