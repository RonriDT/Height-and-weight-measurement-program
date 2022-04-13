#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#include <SD.h>                      
#include <TMRpcm.h>           
#include <SPI.h>

TMRpcm tmrpcm;   
// sd memory 
#define SD_ChipSelectPin 4   
 
// load cell
#include "HX711.h"
#define DOUT  2
#define CLK  5
HX711 scale(DOUT, CLK);
float calibration_factor = 12875; //-7050 worked for my 440lb max scale setup

// ultrasonik
int trigPin = 8; //Trig - green Jumper
int echoPin = 7; //Echo - yellow Jumper
long duration, cm, meter,Tinggi,berat;
int maximum = 183 ;



int data, ratus, puluh, sat; 
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() 
{
  
  Serial.begin (9600);
  
  // ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // tampilan lcd 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Tinggi Dan Berat Badan");
  lcd.setCursor(5,1);
  lcd.print("Badan"); 

  // load cell
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading

  // limit swicth
  pinMode(6, INPUT_PULLUP); // limmit switch
  
  // speker dan sd card
    tmrpcm.speakerPin = 9;
if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
}  


  lcd.clear();

}
void loop(){
if (digitalRead(6) == 0) // pada saat limit swicth 0 maka program akan membaca ultrasonik dan load cell 
  {
    lcd.clear ();
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    cm = (duration/2) / 29.1;
    Tinggi = maximum - cm ; // Pengukuran untuk mendapatkan tinggi sebenarnya 
    
    Serial.println(scale.get_units()*0.453592,1); // Rumus untuk mendapatkan data KG 
    berat =(scale.get_units()*0.453592);
    Serial.println("Sesungguhnya =  " );
    Serial.print( cm );
    Serial.println("cm" );
    
        // Tinggi tampilan lcd 
        lcd.setCursor(0,0);
        lcd.print("Tinggi = ");
        lcd.setCursor(9,0);
        lcd.print(Tinggi);
        lcd.setCursor(12,0);
        lcd.print(" Cm ");

        // tinggi tampilan di serial monitor
        Serial.println("Tinggi ="); 
        Serial.print(Tinggi); 
        Serial.println("cm");

        // speker akan berbunyi sesuai dengan pembacaan sensor 
        tmrpcm.play("tinggi.wav"); 
        delay(2000);
        suara ( Tinggi );
        tmrpcm.play("cm.wav");
        delay(2000);
    
        // Berat tampilan serial monitor 
        scale.set_scale(calibration_factor); //Adjust to this calibration factor
        Serial.println("Berat: ");
        Serial.println(scale.get_units()*0.453592,1); // Rumus untuk mendapatkan data KG 
        berat =(scale.get_units()*0.453592);
        Serial.println( berat);
        Serial.println("kg");

        //berat tampilan lcd
        lcd.setCursor(0,1);
        lcd.print("Berat  = ");
        lcd.setCursor(9,1);
        lcd.print(berat);
        lcd.setCursor(12,1);
        lcd.print(" Kg ");
        
        // speker akan berbunyi sesuai dengan pembacaan sensor 
        tmrpcm.play("berat.wav"); 
        delay(2000);
        suara ( berat );
        tmrpcm.play("kg.wav");
        delay(2000);
      }
else{ 
    lcd.clear ();
    lcd.setCursor(2, 0); 
    lcd.print("Dilla Taufik"); 
    Serial.println("Dilla Taufik"); 
    lcd.setCursor (3, 1); 
    lcd.print ("1501041004");
    Serial.println("1501041004");
    delay (150);
}
}



// UNTUK SUARA 
void suara(int data){
  int ratus = data / 100;
  int puluh = (data % 100) / 10;
  int sat = (((data % 100) % 10) / 1);
  
  Serial.print(ratus); 
  switch (ratus) {
      case 1: 
      tmrpcm.play("100.wav"); 
      break;
  }
  delay(1500);

  if ( (data % 100)> 10 && (data % 100) < 20){
    switch (data %100){
        case 11: 
        tmrpcm.play("11.wav"); 
        break;
        case 12: 
        tmrpcm.play("12.wav "); 
        break;
        case 13: 
        tmrpcm.play("13.wav "); 
        break;
        case 14: 
        tmrpcm.play("14.wav "); 
        break;
        case 15: 
        tmrpcm.play("15.wav ");
        break;
        case 16: 
        tmrpcm.play("16.wav"); 
        break;
        case 17: 
        tmrpcm.play("17.wav"); 
        break;
        case 18: 
        tmrpcm.play("18.wav"); 
        break;
        case 19: 
        tmrpcm.play("19.wav"); 
        break;
    }
  }
  else {
  Serial.print(puluh); 
  switch (puluh) {
        case 1: 
        tmrpcm.play("10.wav"); 
        break;
        case 2: 
        tmrpcm.play("20.wav"); 
        break;
        case 3: 
        tmrpcm.play("30.wav"); 
        break;
        case 4: 
        tmrpcm.play("40.wav"); 
        break;
        case 5: 
        tmrpcm.play("50.wav");
        break;
        case 6: 
        tmrpcm.play("60.wav"); 
        break;
        case 7: 
        tmrpcm.play("70.wav"); 
        break;
        case 8: 
        tmrpcm.play("80.wav"); 
        break;
        case 9: 
        tmrpcm.play("90.wav"); 
        break;
  }
  delay(1500);
  Serial.println(sat); 
  switch (sat) {
    case 1: 
        tmrpcm.play("1.wav");
        break; 
      case 2:
        tmrpcm.play("2.wav"); 
        break;
      case 3:
        tmrpcm.play("3.wav");
        break;
      case 4: 
        tmrpcm.play("4.wav"); 
        break;
      case 5: 
        tmrpcm.play("5.wav"); 
        break;
      case 6: 
        tmrpcm.play("6.wav"); 
        break;
      case 7: 
        tmrpcm.play("7.wav"); 
        break;
      case 8: 
        tmrpcm.play("8.wav"); 
        break;
      case 9: 
        tmrpcm.play("9.wav"); 
        break;
  }
  }
  delay(1000); 
  }
